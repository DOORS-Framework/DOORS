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

$Log: ttydev.C,v $
Revision 1.5  2003/11/20 10:44:34  bilhanan
name.get() -> name.c_str()

Revision 1.4  2002/08/14 09:31:37  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/23 14:25:16  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:48  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:30  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:27  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/ttydev.h>
#include <doors/filedev.h>

//
//Constructor: TtyDevice
//
//Parameters:
//      IoHandler *ioh, 
//      EventTask *t,
//      Type type, 
//      Sint32 size
//
//Description:
//      The constructor initializes the device.
//

TtyDevice :: TtyDevice (IoHandler *ioh, EventTask *t, Type type, Sint32 size) 
  :FileDevice (ioh, t, "/dev/tty", size)
{
  _MT_SYNCMONITOR;

  if (type == TtyDevice :: READ)
    mode = Device::READ | Device::WRITE;
  else
    mode = Device::WRITE;
}

//
//Destructor: TtyDevice
//
//Description:
//      Just to be.
//

TtyDevice :: ~TtyDevice (void) 
{
  _MT_SYNCMONITOR;
}

//Function: open
//
//Member-Of: TtyDevice
//
//Return: bool
//
//Description:
//      Checks the mode of device and sets the standard
//      file descriptors for either input or output.
//

bool TtyDevice :: open (void) 
{
  _MT_SYNCMONITOR;

  // this sucks
  if ((mode & Device::READ) && (mode & Device::WRITE)) {
    fd = 1;
  } else if (mode & Device::WRITE) {
      fd = 2;
  }

  DevIf::Open *op = (DevIf :: Open *) iFace.create (DevIf :: open);
  op->fd = fd;
  op->device = this;
  iFace.putMessage (op);

  _DOORS_DEBUG(0,"open, fd:" << getFd() << ", name:"
	    << name.c_str() 
	    << ", mode:" << form_status(mode)
	    << ", status:" << form_status(status));

  return true;
}

//Function: close
//
//Member-Of: TtyDevice
//
//Return: bool
//
//Description:
//      Clear the values of the device but does not
//      actually close the file descriptor. It cannot
//      be done because the device did not actually
//      open the file descriptor in the first place.
//

bool TtyDevice :: close (void) 
{
  _MT_SYNCMONITOR;

  _MT_KILLTHREAD(this);
  status = 0;
  fd = -1;
  
  DevIf::Close *cl = (DevIf :: Close *) iFace.create (DevIf :: close);
  cl->fd = fd;
  cl->device = this;
  iFace.putMessage (cl);

  return true;
}



