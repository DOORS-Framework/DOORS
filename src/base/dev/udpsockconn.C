//Editor-Info: -*- C++ -*-

//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
 
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
//  THIS SOFTWARE IS PROVIDED BY TAMPERE UNIVERSITY OF TECHNOLOGY AND 
//  CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
//  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//  FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY 
//  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/*

$Log: udpsockconn.C,v $
Revision 1.15  2004/01/07 13:48:20  bilhanan
joins mcast group if necessary on successful bind in connect().

Revision 1.14  2003/07/01 13:09:37  ik
Policy change. Call to connect() without binding address causes the
use of in6addr_any (INADDR_ANY in IPv4).

Revision 1.13  2003/06/13 14:17:39  ik
htons() fixes.

Revision 1.12  2003/04/09 14:41:20  ik
Multicast bugfixes.

Revision 1.11  2002/09/13 11:45:49  bilhanan
return values fixed for mcast_join, mcast_leave

Revision 1.10  2002/08/20 12:32:52  kalliosa
Added JoinMcast to Udp6Sap

Revision 1.9  2002/08/14 09:31:37  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.8  2002/07/23 14:25:17  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.7  2002/06/19 16:19:20  ik
Multicast join moved to setDest.

Revision 1.6  2002/06/17 13:28:54  ik
Useless code removed. bind() fixes.

Revision 1.5  2002/06/14 14:55:32  ik
Multicast options moved to constructor.

Revision 1.4  2002/06/13 13:53:02  ik
Support for multicast join etc.

Revision 1.3  2002/05/12 16:38:03  ik
Tests for accept, bind and recvfrom.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:30  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.5  2001/06/08 08:39:28  bilhanan
Full licence clause added into doors 0.1


*/

 
#define _DOORS_MAX_PORT 65535

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <doors/udpsockconn.h>
#include <doors/sockmsg.h>
#include <fcntl.h>
#include <sys/file.h>
#include <errno.h>

UdpSockConn :: UdpSockConn (IoHandler *io, EventTask *t,
			    unsigned char ttl, bool loopback)
  : SocketConnectionAC (io,t), fd_(-1), ttl_(ttl), loopback_(loopback)
{
  _MT_SYNCMONITOR;

  // Clear addresses
  memset(&to_, 0, sizeof(to_));
  memset(&old_to_, 0, sizeof(old_to_));
  memset(&from_, 0, sizeof(from_));

  setRead (false);
  STATIC_SYMBOL(fd_, this);
  STATIC_SYMBOL(ttl_, this);
  STATIC_SYMBOL(loopback_, this);
}



UdpSockConn :: UdpSockConn (IoHandler *io, EventTask *t, Address *a,
			    unsigned char ttl, bool loopback)
  : SocketConnectionAC (io,t), fd_(-1), ttl_(ttl), loopback_(loopback)
{
  _MT_SYNCMONITOR;

  // Clear addresses
  memset(&to_, 0, sizeof(to_));
  memset(&old_to_, 0, sizeof(old_to_));
  memset(&from_, 0, sizeof(from_));

  setRead (false);
  (void)connect(a);
  STATIC_SYMBOL(fd_, this);
  STATIC_SYMBOL(ttl_, this);
  STATIC_SYMBOL(loopback_, this);
}



UdpSockConn :: ~UdpSockConn (void) {

  _MT_SYNCMONITOR;

  _MT_KILLTHREAD(this);

  if (fd_ >= 0) {
    sockaddr_in* iaddr = (sockaddr_in*)&to_;
    mcast_leave(fd_, iaddr);
  }

  close();

  if (handler) {
    handler->forget (this);
    handler = 0;
  } 

  DELETE_SYMBOL(&fd_);
  DELETE_SYMBOL(&ttl_);
  DELETE_SYMBOL(&loopback_);
}

bool UdpSockConn :: connect (unsigned short port)
{
  // Just create an address with any address and port number
  InetAddr addr(port);
  return connect(&addr);
}

bool UdpSockConn :: connect (Address* a)
{

  _MT_SYNCMONITOR;
  
  Message *bmsg;
  bool ok = false;
  struct sockaddr *addr = 0;

  if (a->getType() == AF_INET) {
    struct sockaddr_in *addr_in = (struct sockaddr_in *) a->getAddr ();
    struct sockaddr_in bind_addr_in;
    bind_addr_in.sin_family = AF_INET;
    bind_addr_in.sin_addr.s_addr = INADDR_ANY;
    bind_addr_in.sin_port = addr_in->sin_port;
    if (!((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)){
      if(!(bind(fd, (sockaddr *)&bind_addr_in, sizeof (bind_addr_in)) < 0)){
	ok = true;
      } else {
	perror("bind");
      }
      fcntl(fd,F_SETFL,FNDELAY);
    }
  }
  
  if (!ok) { 
    // error
    printf ("Unable to open socket.\n");
    DevIf::Close *cl = (DevIf :: Close *) iFace.create (DevIf :: close);
    cl->fd = fd;
    cl->device = this;
    bmsg = (Message *) cl;
    iFace.putMessage (bmsg);
    
    return true;
  }
  // success
  
  _connected = true;

  struct linger ling;
  ling.l_onoff = 0;
  ling.l_linger = 0;
  ::setsockopt (fd, SOL_SOCKET, SO_LINGER,(const char *) &ling, sizeof(ling));

  DevIf::Open *op = (DevIf :: Open *) iFace.create (DevIf :: open);
  op->fd = fd;
  op->device = this;
  bmsg = (Message *) op;
  iFace.putMessage (bmsg);

  // Save the file-descriptor used
  fd_ = fd;

  // Join to multicast group if this is a multicast address
  if (a->getType() == AF_INET){
    InetAddr *testmc = (InetAddr *)a;
    if (IN_MULTICAST(ntohl(testmc->getIpAddress()))) {
      setDest (*testmc);
    }
  }
	
  return false;
}

void UdpSockConn :: callbackRead (void) {

  _MT_SYNCMONITOR;

  int nbytes;
  Message *bmsg = 0;

  if (packetLength == 0) { 
      packetLength = getPacketSize();
      if (packetLength < 0) {
	  // read failed
	  printf ("read failed for %d, %s\n", fd, strerror (errno));
	  _MT_KILLTHREAD(this);
	  close();
	  _connected = false;
	  return;
      }
      if (packetLength == 0) {
	  // try again
	  return;
      }
  }

  if (packetLength != 0) { 
    do {
      // allocate some space, if necessary
      in.buffer = in.frame->endAddress (&in.space);
      if (in.space == 0) {
	in.frame->endSpace ();
	in.buffer = in.frame->endAddress (&in.space);
      }
      
      
      if (in.space > packetLength) {
	in.space = packetLength;
      }
#if HAVE_INT_RECVFROM
      int fromlen = sizeof from_;
#elif HAVE_SIZE_T_RECVFROM
      size_t fromlen = sizeof from_;
#elif HAVE_SOCKLEN_T_RECVFROM
      socklen_t fromlen = sizeof from_;
#else
#error "No known 6th parameter type for recvfrom"
#endif
      nbytes = ::recvfrom (fd,(char*)in.buffer, (int) in.space, 0,
			   (struct sockaddr *) &from_, &fromlen);
      // nbytes = ::read (fd,(char*)in.buffer, (int) in.space);
      
      if (nbytes == 0) {	
	// EOF
	_MT_KILLTHREAD(this);
	close();
	_connected = false;
	break;
      }
      
      if (nbytes < 0) {
	// read failed
	printf ("read failed for %d, %s\n", fd, strerror (errno));
	_MT_KILLTHREAD(this);
	close();
	_connected = false;
	break;
      }
      
      // read succeed
      in.frame->endUpdate (nbytes);
      packetLength -= nbytes;
      
      if (wsize < 0 || packetLength == 0) {
	// packet is complete
	  // setRead (false);
        DevIf::Read *rd = (DevIf :: Read *) iFace.create (DevIf :: read);
        rd->fd = fd;
        rd->data = *in.frame;
        rd->device = this;
        bmsg = (Message *) rd;
        iFace.putMessage (bmsg);
	packetLength = 0;
	wsize = 0;
	in.frame->destroy ();
	return;
      }
    
      in.buffer += nbytes;
      in.space -= nbytes;
      
      // repeat until read fails
    } while (in.space == 0);    
  }
  return;
}

void UdpSockConn :: callbackWrite (void) {

  _MT_SYNCMONITOR;

  int nbytes;
  Message *bmsg;
  FrameSize len = out.frame->length ();
  while (len) {
    out.buffer = out.frame->address () + 1;
    int length = out.frame->data ();
    if (fd != -1) {
      if (-1 == (nbytes = sendto (fd,(const char *)out.buffer, length, 0,
				  (struct sockaddr *) &to_, sizeof to_ ))) {
	if (errno!= EWOULDBLOCK) {
	  printf ("write failed for %d, %s\n", fd, strerror (errno));
	  DevIf::Close *cl = (DevIf :: Close *) iFace.create (DevIf :: close);
	  cl->fd = fd;
          cl->device = this;
          bmsg = (Message *) cl;
          iFace.putMessage (bmsg);
	  status = Device::ERROR;
	  return;
	} 
	nbytes = 0;
      }
    } else {
      // socket shot down.
      printf ("socket dead for %d, %s\n", fd, strerror (errno));
      DevIf::Close *cl = (DevIf :: Close *) iFace.create (DevIf :: close);
      cl->fd = fd;
      cl->device = this;
      bmsg = (Message *) cl;
      iFace.putMessage (bmsg);
      _connected = false;
      status = Device::ERROR;
      break;
    }

    len -= nbytes;
    out.frame->update (-nbytes);
    if (nbytes < length) {
      status |= Device::WRITE;
      return; // partial write! we will continue later
    }
  }
  
  // write completed
  status &= ~Device::WRITE;
}

int UdpSockConn :: getFd (void) const{

  _MT_SYNCMONITOR;
  return fd;
}


bool UdpSockConn :: close (void) {

  _MT_SYNCMONITOR;

  if (fd > 0) {
    ::close (fd);
    status &= ~Device::READ;
    status &= ~Device::WRITE;
    status = Device::IDLE;    

    DevIf::Close *cl = (DevIf :: Close *) iFace.create (DevIf :: close);
    cl->fd = fd;
    cl->device = this;
    iFace.putMessage (cl);    
    fd = -1;
}

  return true;
}

bool UdpSockConn :: setDest (InetAddr a) {
  InetAddr *iaddr = &a;
  to_.sin_addr.s_addr = iaddr->getIpAddress();
  to_.sin_port = htons(iaddr->getPortNumber());
  to_.sin_family = iaddr->getType();

  // Join to multicast group if this is a multicast address
  if (IN_MULTICAST(ntohl(to_.sin_addr.s_addr))) {
    // Check if we have already sent a JOIN for this address
    if (old_to_.sin_addr.s_addr != to_.sin_addr.s_addr) {
      mcast_join(fd, &to_);
      mcast_set_ttl(fd, ttl_);
      mcast_set_loop(fd, loopback_);
      old_to_.sin_addr.s_addr = to_.sin_addr.s_addr;
    }
  }
  return true;
}

InetAddr UdpSockConn :: getSource (void) {
  InetAddr *retAddr = 0;
  sockaddr_in *iaddr = &from_;
  retAddr = (InetAddr *) new InetAddr(iaddr, sizeof(*iaddr)); 
  return *retAddr;
}

int UdpSockConn::mcast_set_loop(int sockfd, bool onoff)
{
  char flag = 0;
  if (onoff) {
    flag = 1;
  }
  return (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &flag, 
		     sizeof(flag)));
}

int UdpSockConn::set_ttl(int sockfd, unsigned char ttl)
{
  int ivalue = ttl;
  return(setsockopt(sockfd, IPPROTO_IP, IP_TTL, (char*)&ivalue, 
		     sizeof(ivalue)));
}

int UdpSockConn::mcast_set_ttl(int sockfd, unsigned char ttl)
{
  if ((ttl < 1) || (ttl > 127)) {
    return -1;
  }
  unsigned char ucvalue = ttl;
  return (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ucvalue, 
		     sizeof(ucvalue)));
}


int UdpSockConn::mcast_join( const struct sockaddr_in *addr)
{
   return mcast_join( fd, addr );
}

int UdpSockConn::mcast_leave( const struct sockaddr_in *addr)
{
   return mcast_leave( fd, addr );
}


int UdpSockConn::mcast_join(int sockfd, const struct sockaddr_in *addr)
{
  int result = -1;

  // Check first that the address is really multicast
  if (!IN_MULTICAST(ntohl(addr->sin_addr.s_addr))) {
    return -1;
  }

  // Enable REUSEADDR if multicast is used
  int one = 1;
  if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) &one,
		   sizeof(one)) == -1) {
    perror("setsockopt: reuseaddr");
  }
  struct ip_mreq mreq;
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  ::memcpy((void*) &mreq.imr_multiaddr,
	   (void*) &(addr->sin_addr),
	   sizeof(struct in_addr));
  result = ::setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
			(const char*)&mreq, sizeof(mreq));
  
  return result;
}

int UdpSockConn::mcast_leave(int sockfd, const struct sockaddr_in *addr)
{
  int result = -1;

  // Check first that the address is really multicast
  if (!IN_MULTICAST(ntohl(addr->sin_addr.s_addr))) {
    return -1;
  }

  struct ip_mreq mreq;
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  ::memcpy((void*) &mreq.imr_multiaddr,
	   (void*) &(addr->sin_addr),
	   sizeof(struct in_addr));
  result = ::setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP,
			(const char*)&mreq, sizeof(mreq));
  
  return result;
}

bool
UdpSockConn::isMulticast()
{
  sockaddr_in* iaddr = (sockaddr_in*)&to_;
  if (IN_MULTICAST(iaddr->sin_addr.s_addr)) {
    return true;
  }

  return false;
}





