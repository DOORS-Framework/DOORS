// 
//  Copyright 1999 
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
 
 
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

$Log: sockconn.C,v $
Revision 1.9  2005/02/21 09:40:42  bilhanan
MAX_PORT value increased

Revision 1.8  2004/08/13 08:45:59  bilhanan
ipv6 bind fixes

Revision 1.7  2004/05/28 12:39:37  bilhanan
initial support for TCP connections over IPv6.

Revision 1.6  2002/08/14 09:31:36  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.5  2002/07/23 14:25:16  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.4  2002/05/12 15:58:53  ik
setsockname conditional lines.

Revision 1.3  2002/05/12 15:31:12  ik
Conditional lines for setsockopt and getpeername.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:27  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:23  bilhanan
Full licence clause added into doors 0.1


*/

#define _DOORS_MAX_PORT 65000

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <doors/sockconn.h>
#include <doors/sockmsg.h>
#include <fcntl.h>
#include <sys/file.h>


//
//Constructor: SockConn
//
//Parameters:
//      IoHandler *ioh,
//      EventTask *t,
//
//Description:
//  Initializes the base class and own variables. Constructor is
//  used when instance is created without actually connecting to 
//  the peer entity. It can be done later with connect function call. 
//
//  The parameter ioh is the i/o handler of the system,
//  t is the task that holds the device.
//
SockConn :: SockConn (IoHandler *io, EventTask *t): 
SocketConnectionAC (io,t)
{
  _MT_SYNCMONITOR;
  setRead (false);
}


//
//Constructor: SockConn
//
//Parameters:
//      IoHandler *ioh,
//      EventTask *t,
//      int sd,
//
//Description:
//  Initializes the base class and own variables. Constructor is
//  used when instance is created with given file decriptor. This
//  usually happens in server side where descriptor is created by
//  SockEnt.
//
//  The parameter ioh is the i/o handler of the system,
//  t is the task that holds the device and sd is descriptor 
//  for connection to be communicate with peer entity.
//
SockConn :: SockConn (IoHandler *io, EventTask *t, Uint32 sd):
SocketConnectionAC (io,t) 
{
  _MT_SYNCMONITOR;
  setRead (false);
}

//
//Constructor: SockConn
//
//Parameters:
//      IoHandler *ioh,
//      EventTask *t,
//      Address *a,
//      NetworkHeader *hdr 
//
//Description:
//  Initializes the base class and own variables. Constructor is
//  used when instance is created with given peer entity address. 
//  This usually happens in client side where connection is established
//  by the instance itself.
//
//  The parameter ioh is the i/o handler of the system,
//  t is the task that holds the device and a is a address of 
//  the peer entity.
//
SockConn :: SockConn (IoHandler *io, EventTask *t, Address *a):
SocketConnectionAC (io,t) 
{
  _MT_SYNCMONITOR;
  setRead (false);
}


//
//Destructor: SockConn
//
//Description:
//

SockConn :: ~SockConn (void) {

  _MT_SYNCMONITOR;

  _MT_KILLTHREAD(this);
  close();

  if (handler) {
    handler->forget (this);
    handler = 0;
  } 
}


//
//Function: getRemoteAddress
//
//Member-Of: SockConn
//
//Returns: Address *
//
//Description:
//  Function wraps the system call getpeername and 
//  returns the address of the peer entity.
//
Address *SockConn :: getRemoteAddress (void) {

  _MT_SYNCMONITOR;

  Address *retAddr = 0;
  struct sockaddr *peer = 0;
  sockaddr sAddr;

#if HAVE_INT_PEERNAME
  int len = sizeof (struct sockaddr);
#elif HAVE_SIZE_T_PEERNAME
  size_t len = sizeof (struct sockaddr);
#elif HAVE_SOCKLEN_T_PEERNAME
  socklen_t len = sizeof (struct sockaddr);
#else
#error "No known 3rd parameter type for getsockname"
#endif

  getsockname (fd, &sAddr, &len);
  //  getsockname (fd, &sAddr, (socklen_t *)&len);

  switch (sAddr.sa_family) {
  case AF_INET: {
    sockaddr_in *iaddr = new sockaddr_in;
    peer = (struct sockaddr *) iaddr;
#if HAVE_INT_PEERNAME
    int i = sizeof(*peer); 
#elif HAVE_SIZE_T_PEERNAME
    size_t i = sizeof(*peer); 
#elif HAVE_SOCKLEN_T_PEERNAME
    socklen_t i = sizeof(*peer); 
#else
#error "No known 3rd parameter type for getpeername"
#endif
    getpeername (fd, peer, &i);

    retAddr = (Address *) new InetAddr(iaddr, sizeof(*iaddr)); 
    break;
  }
  case AF_INET6: {
    sockaddr_in6 *iaddr = new sockaddr_in6;
    peer = (struct sockaddr *) iaddr;
#if HAVE_INT_PEERNAME
    int i = sizeof(*peer); 
#elif HAVE_SIZE_T_PEERNAME
    size_t i = sizeof(*peer); 
#elif HAVE_SOCKLEN_T_PEERNAME
    socklen_t i = sizeof(*peer); 
#else
#error "No known 3rd parameter type for getpeername"
#endif
    getpeername (fd, peer, &i);

    retAddr = (Address *) new InetAddr6(iaddr, sizeof(*iaddr)); 
    break;
  }
  case AF_UNIX: {
    sockaddr_un *uaddr = new sockaddr_un;
    peer = (struct sockaddr *) uaddr;
#if HAVE_INT_PEERNAME
    int i = sizeof(*peer); 
#elif HAVE_SIZE_T_PEERNAME
    size_t i = sizeof(*peer); 
#elif HAVE_SOCKLEN_T_PEERNAME
    socklen_t i = sizeof(*peer); 
#else
#error "No known 3rd parameter type for getpeername"
#endif
    getpeername (fd, peer, &i);
    retAddr = (Address *) new UnixAddr(uaddr, sizeof(*uaddr)); 
    break;
  }
  }

  if (peer) {
    //   getpeername (fd, peer, sizeof(*peer));    
    return retAddr;
  }

  return 0;
}


//
//Function: connect
//
//Member-Of: SockConn
//
//Returns: bool
//
//Parameters:
//        Address *a
//
//Description:
//  Function is used for connecting to peer entity given as
//  a parameter a. The type of connection is checked from the
//  type of address and proper connector and stream is created.
//  The controlling task is informed both the success of failure
//  of the connection establishement through the DevIf.

bool SockConn :: connect (Address *a) {

  _MT_SYNCMONITOR;

  Message *bmsg;
  bool ok = false;
  struct sockaddr_in6 bind_addr_in6;

  if (a->getType() == AF_UNIX) {
    struct sockaddr *uaddr = (struct sockaddr *) a->getAddr();
    if (!((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0))
    {
      if (!(::connect(fd, uaddr, sizeof(*uaddr)) < 0)){
	ok = true;
      }
      fcntl(fd,F_SETFL,FNDELAY); 
    }
    
  } else if (a->getType() == AF_INET) {
    struct sockaddr *iaddr = (struct sockaddr *) a->getAddr();
    if (!((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0))
    {
      if(!((bindPort (fd, AF_INET)) < 0)){
	if (::connect(fd, iaddr, sizeof(*iaddr)) != -1)
	  {
	    ok = true;
	  }
      }

      fcntl(fd,F_SETFL,FNDELAY);
      
    }
  } else if (a->getType() == AF_INET6) {
    struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *) a->getAddr ();
    ::memset((void*) &bind_addr_in6, 0, sizeof(bind_addr_in6));
    bind_addr_in6.sin6_family = AF_INET6;
    bind_addr_in6.sin6_port = addr_in6->sin6_port;
    bind_addr_in6.sin6_addr = addr_in6->sin6_addr;
    if (!((fd = socket(AF_INET6, SOCK_STREAM, 0)) < 0))
    {
      if(!((bindPort (fd, AF_INET6)) < 0)){
	if (::connect(fd, (struct sockaddr*)&bind_addr_in6, sizeof(struct sockaddr_in6)) != -1){
	    ok = true;
	  }
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

  return false;
}


//Function: connect
//
//Member-Of: SockConn
//
//Returns: bool
//
//Parameters:
//        Uint32 sd
//
//Description:
//  Function is used for connecting to peer entity given as
//  a file descriptor sd. The type of connection is checked from the
//  type of file descriptor and proper stream is created.


bool SockConn :: connect (Uint32 sd) {

  _MT_SYNCMONITOR;

  fd = sd;
  _connected = true;
  return false;
}

//Function: callbackRead
//
//Member-Of: SockConn
//
//Description:
//  Function is runned when something is to be read from the
//  socket. 
//
//  If there is all data availeble then it is read without any breaks
//  and SocketData message is send to the controlling task. If there 
//  is not enough data availeble then partial reading is used  and reading 
//  is continued when there is new data availeble.
//  The data is sent in DevIF :: Read message to the task when either 
//  packetlenght is zero or wsize is set for -1 (StreamDevice :: immediate).
//  wsize is set to zero after data is sent and it must be set with 
//  devices readBytes function before every reading for the 
//  sake of synchronising.   
//
//  If for some reason error happens then controlling task is informed
//  with DevIf :: Close message.
//

void SockConn :: callbackRead (void) {

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
      
      nbytes = ::read (fd,(char*)in.buffer, (int) in.space);
      
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

//
//Function: callbackWrite
//
//Member-Of: SockConn
//
//Parameters:
//
//Return:
//
//Description:
//  Function is runned when something is to be writen into the
//  socket. 
//
//  If it is possible to write all data into the socket it is done without 
//  any breaks. If it is not possible then writing is done only partially
//  and it is continued when data can be sended.
//
//  If for some reason error happens then controlling task is informed
//  with DevIf :: Close message.
//
void SockConn :: callbackWrite (void) {

  _MT_SYNCMONITOR;

  int nbytes;
  Message *bmsg;
  FrameSize len = out.frame->length ();
  while (len) {
    out.buffer = out.frame->address () + 1;
    int length = out.frame->data ();
    if (fd != -1) {
      if (-1 == (nbytes = send (fd,(const char *)out.buffer, length,0))) {
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

int SockConn :: getFd (void) const{

  _MT_SYNCMONITOR;
  return fd;
}


int SockConn :: bindPort (int sock, int addrType){

  _MT_SYNCMONITOR;

  static int upper_limit = _DOORS_MAX_PORT;
  int lower_limit = IPPORT_RESERVED;
  int start = upper_limit;

  if (addrType == AF_INET) {
  struct sockaddr_in sin;
  
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  
  for (;;)
    {
      sin.sin_port = htons (upper_limit);
      
      if (bind (sock, (struct sockaddr *) &sin, sizeof(sin)) >= 0)
        return 0;
      if (errno != EADDRINUSE)
        return -1;
      else
        {
          upper_limit--;
	  
          // Wrap back around when we reach the bottom. 
          if (upper_limit <= lower_limit)
            upper_limit = _DOORS_MAX_PORT;
	  
          // See if we have already gone around once
          if (upper_limit == start)
            {
              return -1;
            }
        }
    }

  }
  else if (addrType == AF_INET6) {
  struct sockaddr_in6 sin6;
  ::memset((void*) &sin6, 0, sizeof(sin6));
 
  sin6.sin6_family = AF_INET6;
  sin6.sin6_addr = in6addr_any;
  
  for (;;)
    {
      sin6.sin6_port = htons (upper_limit);
      
      if (bind (sock, (struct sockaddr *) &sin6, sizeof(struct sockaddr_in6)) >= 0)
        return 0;
      perror ("Sockconn bind: ");
      if (errno != EADDRINUSE)
        return -1;
      else
        {
          upper_limit--;
	  
          // Wrap back around when we reach the bottom. 
          if (upper_limit <= lower_limit)
            upper_limit = _DOORS_MAX_PORT;
	  
          // See if we have already gone around once
          if (upper_limit == start)
            {
              return -1;
            }
        }
    }

  }


}

//
//Function: close
//
//Member-Of: SockConn
//
//Return: bool
//
//Description:
//      Close device and send proper message to the 
//      controlling task.

bool SockConn :: close (void) {

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

