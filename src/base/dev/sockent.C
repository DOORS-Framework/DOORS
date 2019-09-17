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

$Log: sockent.C,v $
Revision 1.7  2004/08/13 08:45:59  bilhanan
ipv6 bind fixes

Revision 1.6  2004/05/28 12:39:37  bilhanan
initial support for TCP connections over IPv6.

Revision 1.5  2002/08/14 09:31:36  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.4  2002/07/23 14:25:16  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.3  2002/05/12 16:38:03  ik
Tests for accept, bind and recvfrom.

Revision 1.2  2002/05/11 20:34:40  ik
HP-UX fixes for socklen_t.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:27  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:24  bilhanan
Full licence clause added into doors 0.1


*/

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <doors/core.h>
#include <doors/sockent.h>
#include <doors/sockmsg.h>
#include <doors/ocoremac.h>
#include <fcntl.h>
#include <sys/file.h>
#include <netinet/in.h>

//
//Constructor: SockEnt
//
//Parameters:
//      IoHandler *ioh,
//      EventTask *t,
//
//Description:
//  Initializes the base class and own variables. 
//
//  The parameter ioh is the i/o handler of the system
//  and t is the task that holds the device. 

SockEnt :: SockEnt (IoHandler *io, EventTask *t)
: SocketEntityAC (io, t)
{
  _MT_SYNCMONITOR;
}

//
//Destructor: SockEnt
//
//Description:
//

SockEnt :: ~SockEnt (void) {
  
  _MT_SYNCMONITOR;

  close();

  if (handler) {
    handler->forget (this);
    handler = 0;
  }

}

//
//Function: callbackRead
//
//Member-Of: SockEnt
//
//Returns: 
//
//Parameters:
//
//Description:
//  Function is runned when connection request is
//  noticed. Created descriptor is returned for the
//  controlling task by sending the DevIf :: Open 
//  message to it.
//

void SockEnt :: callbackRead (void) {

  _MT_SYNCMONITOR;

  Message *bmsg;
  sockaddr addr;

  DevIf::Open *op = (DevIf :: Open *) iFace.create (DevIf :: open);  
#if HAVE_INT_ACCEPT
  int i = sizeof (addr);
#elif HAVE_SIZE_T_ACCEPT
  size_t i = sizeof (addr);
#elif HAVE_SOCKLEN_T_ACCEPT
  socklen_t i = sizeof (addr);
#else
#error "No known 3rd parameter type for accept"
#endif
  op->fd = accept (fd, &addr, &i);
  op->device = this;
  bmsg = (Message *) op;
  iFace.putMessage (bmsg);

}

int SockEnt :: getFd (void) const {

  _MT_SYNCMONITOR;
  return fd;
}

//
//Function: connect
//
//Member-Of: SockEnt
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

void SockEnt :: connect (Address *a)
{
  _MT_SYNCMONITOR;
  Message *bmsg;

  bool ok = false;
  struct sockaddr *addr = 0;
  struct sockaddr_in6 bind_addr_in6;
  
  if (a->getType () == AF_UNIX) {
    struct sockaddr_un *addr2 =  (struct sockaddr_un *) a->getAddr ();
    addr = (struct sockaddr *) addr2;
    if (!((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0))
      {
	if(!(bind(fd, (sockaddr *)addr2, sizeof (*addr)) < 0)){
	  listen(fd, 5);
	  ok = true;
	}
	if (fcntl(fd,F_SETFL,FNDELAY)<0){
	  ok = false;
	}
      }
  }
  else if (a->getType () == AF_INET) {
    addr = (struct sockaddr *) a->getAddr ();
    if (!((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0))
      {
	if(!(bind(fd, addr, sizeof (*addr)) < 0)){
	  listen(fd, 5);
	  ok = true;
	}
	fcntl(fd,F_SETFL,FNDELAY);
      }
  }
  else if (a->getType () == AF_INET6) {
    if( fd != -1 )
      ::close (fd);

    struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *) a->getAddr ();
    ::memset((void*) &bind_addr_in6, 0, sizeof(bind_addr_in6));
    bind_addr_in6.sin6_family = AF_INET6;
    bind_addr_in6.sin6_port = addr_in6->sin6_port;
    bind_addr_in6.sin6_addr = addr_in6->sin6_addr;

    if (!((fd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)) {
      if(!(bind(fd, (struct sockaddr*)&bind_addr_in6, 
		sizeof (struct sockaddr_in6)) < 0)){
	listen(fd, 5);
	ok = true;
      } 
      fcntl(fd,F_SETFL,FNDELAY);
    }
  }
  
  if (!ok)
    {
      printf ("Unable to open server socket.\n");
      DevIf::Close *cl = (DevIf :: Close *) iFace.create (DevIf :: close);
      cl->fd = fd;
      cl->device = this;
      bmsg = (Message *) cl;
      iFace.putMessage (bmsg);
      
    }

  // success
  status |= Device::READ;
}

//
//Function: close
//
//Member-Of: SockEnt
//
//Return: bool
//
//Description:
//      Close device and send proper message to the 
//      controlling task.

bool SockEnt :: close (void) {

  _MT_SYNCMONITOR;

  if (!fd == -1) {
    ::close (fd);
    status &= ~Device::READ;
    status = Device::IDLE;    

    DevIf::Close *cl = (DevIf :: Close *) iFace.create (DevIf :: close);
    cl->fd = fd;
    cl->device = this;
    iFace.putMessage (cl);    
    fd = -1;
  }

  return true;
}




