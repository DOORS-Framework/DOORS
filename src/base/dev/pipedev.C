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

$Log: pipedev.C,v $
Revision 1.7  2003/11/20 10:44:34  bilhanan
name.get() -> name.c_str()

Revision 1.6  2002/08/14 09:31:36  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.5  2002/07/25 10:08:38  ik
Use std::string.

Revision 1.4  2002/07/24 14:57:00  ik
Use std::string.

Revision 1.3  2002/07/23 14:25:15  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:47  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.3  2001/06/08 16:46:25  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:20  bilhanan
Full licence clause added into doors 0.1


*/

#include <string>
#include <fcntl.h>
#include <unistd.h>


#include <doors/pipedev.h>
#include <doors/pipeport.h>
#include <doors/util.h>

const int TTY_BUFFER_SIZE = 1024;

//
//Constructor: PipeDevice
//
//Parameters:
//      IoHandler *ioh, 
//      EventTask *t,
//      std::string fn, 
//
//Description:
//      The constructor initializes the device.
//
PipeDevice :: PipeDevice (IoHandler *ioh, EventTask *t, std::string fn): 
StreamDevice(ioh, t),
name (fn), 
in (256), 
out (256),
fd(-1), 
handler(ioh), 
task(t) 
{
  _MT_SYNCMONITOR;
}


//
//Destructor: PipeDevice
//
//Description:
//      Closes the device.
//

PipeDevice :: ~PipeDevice (void)
{
  _MT_SYNCMONITOR;

  if (handler) {
    handler->forget (this);
    handler = 0;
  } 
  close ();
  _MT_KILLTHREAD(this);
}


//Function: open
//
//Member-Of: PipeDevice
//
//Return:
//      bool
//
//Description:
//      Function creates a pipe and opens it. Two FileDevices
//      are created to handle writing and reading of two file
//      descriptors from pipe.
//

bool PipeDevice :: open (void)
{
  _MT_SYNCMONITOR;

  Message *bmsg;

  if (!name.empty ()) {
    int m = 0;
    
    if (pipe (p)<0 ) 
      {
      status = Device::ERROR;
      _DOORS_DEBUG(0,"open: cannot open " << name.c_str());
      DevIf::Close *cl = (DevIf :: Close *) iFace.create (DevIf :: close);
      cl->fd = -1;
      cl->device = this;
      bmsg = (Message *) cl;
      iFace.putMessage (bmsg);
      return false;
    }

  } else {
    DevIf::Close *cl = (DevIf :: Close *) iFace.create (DevIf :: close);
    status = Device::ERROR;
    cl->fd = -1;
    cl->device= this;
    bmsg = (Message *) cl;
    iFace.putMessage (bmsg);
    return false;
  }
  readPort = new PipePort ((Port *) &iFace, this);
  writePort = new PipePort((Port *) &iFace, this);
  piper = new FileDevice (handler, task, "piper", 256, Device :: READ, p[0]);
  _MT_SCHEDULE(piper);
  pipew = new FileDevice (handler, task, "pipew", 256, Device :: WRITE, p[1]); 
  _MT_SCHEDULE(pipew);
  if ((readPort->connect (&piper->iFace))&&(writePort->connect(&pipew->iFace)))
    {
      DevIf::Open *op = (DevIf :: Open *) iFace.create (DevIf :: open);
      op->fd = -1;
      op->device = this;
      bmsg = (Message *) op;
      iFace.putMessage (bmsg);
      _DOORS_DEBUG(0,"open, fd:" << getFd() << ", name:"
		<< name.c_str() 
		<< ", mode:" << form_status(mode)
		<< ", status:" << form_status(status));
      return true;
    }
  else 
    {
      DevIf::Close *cl = (DevIf :: Close *) iFace.create (DevIf :: close);
      status = Device::ERROR;
      cl->fd = -1;
      cl->device= this;
      bmsg = (Message *) cl;
      iFace.putMessage (bmsg);
      return false;
    }
}

//Function: portClosed
//
//Member-Of: PipeDevice
//
//Return:
//      bool
//
//Parameters:
//      Port *
//
//Description:
//      Updates the status of read and write ports and 
//      informs the task when both are closed.
//

bool PipeDevice :: portClosed (Port* cPort)
{    
  _MT_SYNCMONITOR;

  if (cPort == readPort)
    {
      piper = 0;
    }
  else if (cPort == writePort)
    {
      pipew = 0;
    }
  
  if (!pipew && !piper)
    {
      
      DevIf::Close *cl = (DevIf :: Close *) iFace.create (DevIf :: close);
      cl->fd = -1;
      cl->device = this;
      Message *bmsg = (Message *) cl;
      iFace.putMessage (bmsg);
    }
  
  return false; 
}

//
//Function: close
//
//Member-Of: PipeDevice
//
//Return:
//      bool
//
//Description:
//      Closes the device and informs task about that.
//

bool PipeDevice :: close (void)
{
  _MT_SYNCMONITOR;
  Message *bmsg;
  
  _DOORS_DEBUG(0,"PipeDevice-close:");
  if (piper)
    {
      delete readPort;
      delete piper;
      readPort = 0;
      piper = 0;
    }
  if (pipew)
    {
      delete writePort;
      delete pipew;
      writePort = 0;
      pipew = 0;
    }
      
  DevIf::Close *cl = (DevIf :: Close *) iFace.create (DevIf :: close);
  cl->fd = -1;
  cl->device = this;
  bmsg = (Message *) cl;
  iFace.putMessage (bmsg);
  return false;
}

//
//Function: closeRead
//
//Member-Of: PipeDevice
//
//Return:
//      bool
//
//Description:
//      Close the read side FileDevice. 
//

bool PipeDevice :: closeRead (void)
{
  _MT_SYNCMONITOR;

  if (piper->close()) {
    return true;
  }
  return false;
}

//
//Function: closeWrite
//
//Member-Of: PipeDevice
//
//Return:
//      bool
//
//Description:
//      Close the write side FileDevice. 
//

bool PipeDevice :: closeWrite (void)
{
  _MT_SYNCMONITOR;

  if (pipew->close()) {
    return true;
  }
  return false;
}

//
//Function: write
//
//Member-Of: PipeDevice
//
//Parameters:
//      Frame &frm
//
//Return:
//      bool
//
//Description:
//      Re-directs write command to write side FileDevice.
//       

bool PipeDevice :: write (Frame &frm)
{
  _MT_SYNCMONITOR;

  if (pipew)
    {
      return pipew->write (frm);
    }
  return false;
}


//
//Function: readBytes
//
//Member-Of: PipeDevice
//
//Parameters: Sint32
//
//Description:
//      Re-directs the command to read side FileDevice.       
//

void PipeDevice :: readBytes (Sint32 i)
{
  _MT_SYNCMONITOR;

  piper->readBytes (i);
}

//
//Function: getReadFd
//
//Member-Of: PipeDevice
//
//Return: int
//
//Description:
//      Returns the read side file descriptor of the pipe.
//

int PipeDevice :: getReadFd (void)
{
  _MT_SYNCMONITOR;

  return p[0];
}

//
//Function: getWriteFd
//
//Member-Of: PipeDevice
//
//Return: int
//
//Description:
//      Returns the write side file descriptor of the pipe.
//

int PipeDevice :: getWriteFd (void)
{
  _MT_SYNCMONITOR;

  return p[1];
}

//
//Function: getFd
//
//Member-Of: PipeDevice
//
//Return: int
//
//Description:
//      Returns the invalid file descriptor, because in
//      this device there are two separate file descriptors
//      that can be retrieved with their own functions.
//

int PipeDevice :: getFd (void) const
{
  _MT_SYNCMONITOR;

  return -1;
}



