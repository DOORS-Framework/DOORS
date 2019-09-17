
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

$Log: udpsockconn6.C,v $
Revision 1.18  2004/07/16 09:06:12  hartmanj
In connect(): close socket, if it is open, so that re-opening works when moving to another subnet

Revision 1.17  2004/01/09 13:50:45  bilhanan
joins mcast group if necessary on successful bind in connect().

Revision 1.16  2003/07/01 13:09:37  ik
Policy change. Call to connect() without binding address causes the
use of in6addr_any (INADDR_ANY in IPv4).

Revision 1.15  2003/06/13 14:18:01  ik
Binds to any IPv6 address.

Revision 1.14  2003/04/10 09:04:02  ik
Bug in IN6_ARE_ADDR_EQUAL parameters.

Revision 1.13  2003/04/09 14:41:20  ik
Multicast bugfixes.

Revision 1.12  2002/11/13 14:55:54  kalliosa
minor bugfixes:
inetaddr6::getPortNumber: ntohs added
inetaddr::getPortNumber: ntohs added
inetAddr6::setPort function addes

Revision 1.11  2002/10/07 13:40:32  ik
htons was in a wrong place.

Revision 1.10  2002/09/13 12:03:35  kalliosa
cleaned couple of lines

Revision 1.9  2002/09/13 11:45:49  bilhanan
return values fixed for mcast_join, mcast_leave

Revision 1.8  2002/08/20 12:32:52  kalliosa
Added JoinMcast to Udp6Sap

Revision 1.7  2002/08/14 09:31:37  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.6  2002/07/23 14:25:17  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.5  2002/06/19 16:19:01  ik
Multicast join moved to setDest().

Revision 1.4  2002/06/19 13:44:39  ik
Bug fixed in setDest.

Revision 1.3  2002/06/17 13:28:54  ik
Useless code removed. bind() fixes.

Revision 1.2  2002/06/14 20:56:40  ik
Support for hoplimit and loopback.

Revision 1.1  2002/06/06 12:19:12  ik
IPv6 UdpSockConn.


*/

 
#define _DOORS_MAX_PORT 65535

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <doors/udpsockconn6.h>
#include <doors/sockmsg.h>
#include <fcntl.h>
#include <sys/file.h>
#include <netinet/in.h>

UdpSockConn6 :: UdpSockConn6 (IoHandler *io, EventTask *t,
			      unsigned int hoplimit, bool loopback): 
  SocketConnectionAC (io,t), hoplimit_(hoplimit), loopback_(loopback)
{
  _MT_SYNCMONITOR;

  // Clear addresses
  memset(&to_, 0, sizeof(to_));
  memset(&old_to_, 0, sizeof(old_to_));
  memset(&from_, 0, sizeof(from_));

  setRead (false);
}



UdpSockConn6 :: UdpSockConn6 (IoHandler *io, EventTask *t, Address *a,
			      unsigned int hoplimit, bool loopback):
  SocketConnectionAC (io,t), hoplimit_(hoplimit), loopback_(loopback)
{
  _MT_SYNCMONITOR;

  fd = -1;
  // Clear addresses
  memset(&to_, 0, sizeof(to_));
  memset(&old_to_, 0, sizeof(old_to_));
  memset(&from_, 0, sizeof(from_));

  setRead (false);
  (void)connect(a);
}



UdpSockConn6 :: ~UdpSockConn6 (void) {

  _MT_SYNCMONITOR;

  _MT_KILLTHREAD(this);
  close();

  if (handler) {
    handler->forget (this);
    handler = 0;
  } 
}

bool UdpSockConn6 :: connect (unsigned short port)
{
  InetAddr6 addr(port);
  return connect(&addr);
}

bool UdpSockConn6 :: connect (Address *a)
{

  _MT_SYNCMONITOR;
  
  Message *bmsg;
  bool ok = false;
  struct sockaddr *addr = 0;
  struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *) a->getAddr ();
  struct sockaddr_in6 bind_addr_in6;

  switch(a->getType()) {
  case AF_INET6 :
    if( fd != -1 )
      ::close (fd);

    ::memset((void*) &bind_addr_in6, 0, sizeof(bind_addr_in6));
    bind_addr_in6.sin6_family = AF_INET6;
    bind_addr_in6.sin6_port = addr_in6->sin6_port;
    bind_addr_in6.sin6_addr = addr_in6->sin6_addr;
    if (  !((fd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0)){
      if(!(bind(fd, (struct sockaddr*)&bind_addr_in6, 
		sizeof (struct sockaddr_in6)) < 0)){
	ok = true;
      }
      fcntl(fd,F_SETFL,FNDELAY);
    }
    break;
  default :
    ok = false;
    break;
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

  // Join to multicast group if this is a multicast address
  if (a->getType() == AF_INET6){
    InetAddr6 *testmc = (InetAddr6 *)a;
    struct sockaddr_in6 *testaddr = (struct sockaddr_in6 *) testmc->getAddr();
    if (IN6_IS_ADDR_MULTICAST(&(testaddr->sin6_addr))) {
      setDest (*testmc);
    }
  }
  
  return false;
}

void UdpSockConn6 :: callbackRead (void) {

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

void UdpSockConn6 :: callbackWrite (void) {

  _MT_SYNCMONITOR;

  int nbytes;
  Message *bmsg;
  FrameSize len = out.frame->length ();
  while (len) {
    out.buffer = out.frame->address () + 1;
    int length = out.frame->data ();
    if (fd != -1) {
      if (-1 == (nbytes = sendto (fd,(const char *)out.buffer, length, 0,
				  (struct sockaddr*)&to_, sizeof to_ ))) {
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

int UdpSockConn6 :: getFd (void) const{

  _MT_SYNCMONITOR;
  return fd;
}


bool UdpSockConn6 :: close (void) {

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

bool UdpSockConn6 :: setDest (InetAddr6 a) 
{
  ::memcpy((void*) &to_, 
	   (void*) a.getAddr(), 
	   sizeof(to_));
  // Join to multicast group if this is a multicast address
  if (IN6_IS_ADDR_MULTICAST(&to_.sin6_addr)) {
    // Check if we have already sent a JOIN for this address
    if (!IN6_ARE_ADDR_EQUAL(&to_.sin6_addr,&old_to_.sin6_addr)) {
      mcast_join(fd, &to_);
      mcast_set_hoplimit(fd, hoplimit_);
      mcast_set_loop(fd, loopback_);
      memcpy(&old_to_, &to_, sizeof(old_to_));
    }
  }
  return true;
}

InetAddr6* UdpSockConn6 :: getSource (void) {
  return new InetAddr6(&from_, sizeof(struct sockaddr_in6)); 
}

int UdpSockConn6::mcast_set_hoplimit(int sockfd, unsigned int hoplimit)
{
  if ((hoplimit < 1) || (hoplimit > 127)) {
    return -1;
  }
  unsigned int uivalue = hoplimit;
  return (setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, 
		     (char*)&uivalue, sizeof(uivalue)));
}

int UdpSockConn6::mcast_set_loop(int sockfd, int onoff)
{
  int result = -1;
  char flag = onoff;
  result = ::setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &flag, 
			sizeof(flag));
  return result;
}

int UdpSockConn6::mcast_join(int sockfd, const struct sockaddr_in6 *addr)
{
  int result = -1;

  if (!IN6_IS_ADDR_MULTICAST(&addr->sin6_addr)) {
    return -1;
  }

  // Enable REUSEADDR
  int one = 1;
  if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) &one,
		   sizeof(one)) == -1) {
    perror("setsockopt: reuseaddr");
  }
  struct ipv6_mreq v6mreq;
  v6mreq.ipv6mr_interface = 0;
  ::memcpy((void*) &v6mreq.ipv6mr_multiaddr,
	   (void*) &(addr->sin6_addr),
	   sizeof(struct in6_addr));
  result = ::setsockopt(sockfd, IPPROTO_IPV6, IPV6_JOIN_GROUP, 
			(const char*)&v6mreq, sizeof(v6mreq));
  
  return result;
}

int UdpSockConn6::mcast_leave(int sockfd, const struct sockaddr_in6 *addr)
{
  int result = -1;

  if (!IN6_IS_ADDR_MULTICAST(&addr->sin6_addr)) {
    return -1;
  }

  struct ipv6_mreq v6mreq;
  v6mreq.ipv6mr_interface = 0;
  ::memcpy((void*) &v6mreq.ipv6mr_multiaddr,
	   (void*) &(addr->sin6_addr),
	   sizeof(struct in6_addr));
  result = ::setsockopt(sockfd, IPPROTO_IPV6, IPV6_LEAVE_GROUP, 
			(const char*)&v6mreq, sizeof(v6mreq));
  
  return result;
}

int UdpSockConn6::mcast_join( const struct sockaddr_in6 *addr)
{
   return mcast_join( fd, addr );
}

int UdpSockConn6::mcast_leave( const struct sockaddr_in6 *addr)
{
   return mcast_leave( fd, addr ); 
}


bool
UdpSockConn6 :: isMulticast()
{
  sockaddr_in6* iaddr6 = (sockaddr_in6*)&to_;
  if (IN6_IS_ADDR_MULTICAST(&iaddr6->sin6_addr)) {
    return true;
  }

  return false;
}

