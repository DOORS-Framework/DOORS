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

$Log: filedev.C,v $
Revision 1.6  2003/11/20 10:44:34  bilhanan
name.get() -> name.c_str()

Revision 1.5  2002/08/14 09:31:36  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.4  2002/07/25 10:08:38  ik
Use std::string.

Revision 1.3  2002/07/24 14:57:00  ik
Use std::string.

Revision 1.2  2002/07/23 14:25:15  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:20  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:13  bilhanan
Full licence clause added into doors 0.1


*/

#include <string>
#include <doors/filedev.h>
#include <doors/util.h>
#include <doors/debug.h>
#include <fcntl.h>
#include <unistd.h>

//Instance: TTY_BUFFER_SIZE
const int TTY_BUFFER_SIZE = 1024;


// Function for forming string out of 
// filedevice status.

std::string form_status(int status)
{
  static std::string buf;
  buf = "";
  if(status & Device::IDLE) {
    buf += "i";
  }
  if(status & Device::READ) {
    buf += "r";
  }
  if(status & Device::WRITE) {
    buf += "w";
  }
  if(status & Device::EXCEPTION) {
    buf += "x";
  }
  if(status & Device::TIMEOUT) {
    buf += "t";
  }
  if(status & Device::ERROR) {
    buf += "e";
  }
  return buf;
}


//
//Constructor: FileDevice
//
//Parameters:
//      IoHandler *ioh, 
//      EventTask *t,
//      std::string name, 
//      int m
//
//Description:
//      The constructor initializes the device and leaves it to
//      an idle state waiting for the call of open member function,
//      which finally opens the file. 
//
//      The name is duplicated, i.e. the file device creates its
//      own copy of the name. 
//
//      The parameter ioh is the i/o handler of the system, and
//      t is the task where the device port is connected, and
//      name is the name of the file wanted to open, and
//      m is mode, Device::READ, Device::WRITE (or their combination).
//
FileDevice :: FileDevice (IoHandler *ioh, EventTask *t, std::string fn,
			  Sint32 size, int m) 
: StreamDevice(ioh, t), mode (m), name (fn), fd (-1), in (size), out (size) 
{
  _MT_SYNCMONITOR;
  
  _MT_RESET_BLOCKTIME(0);
  _MT_SIGNAL_RUNLOCK;
}
//
//Constructor: FileDevice
//
//Parameters:
//      IoHandler *ioh, 
//      EventTask *t,
//      std::string name,
//      Sint32 size, 
//      int m,
//      int filed
//
//Description:
//      The constructor initializes the device and leaves it to
//      an idle state waiting for the call of open member function,
//      which finally opens the file. 
//
//      The name is duplicated, i.e. the file device creates its
//      own copy of the name. 
//
//      The parameter ioh is the i/o handler of the system, and
//      t is the task where the device port is connected, and
//      name is the name of the file wanted to open, and
//      m is mode, Device::READ, Device::WRITE (or their combination), and
//      filed is the filedescriptor wanted for filedevice.
//
FileDevice :: FileDevice (IoHandler *ioh, EventTask *t, std::string fn,
			  Sint32 size, int m, int filed) 
    : StreamDevice(ioh, t), mode (m), name (fn), fd (filed), in (size), out (size)  
{
 }

//
//Destructor: FileDevice
//
//Description:
//      Closes the file.
//
FileDevice :: ~FileDevice (void)
{
  _MT_SYNCMONITOR;

  if (handler) {
    handler->forget (this);
    handler = 0;
  } 
  close ();
}


//Function: getFd
//
//Member-Of: FileDevice
//
//Return: int
//
//Description:
//      Returns the fd associated with the device.
//

int FileDevice :: getFd (void) const 
{
  _MT_SYNCMONITOR;

  return fd;
}


//
//Function: setName
//
//Member-Of: FileDevice
//
//Parameters:
//      std::string name,
//      int mode
//
//Description:
//      Closes the file if it is open and creates a copy of the name,
//      if it is not zero. 
//
//      The parameter name is the name of a file associated with 
//      this device. Mode is also updated.
//
void FileDevice :: setName (std::string n, int m)
{
  _MT_SYNCMONITOR;

  name = n;
  mode = m;
}


//
//Function: open
//
//Member-Of: FileDevice
//
//Return:
//      bool
//
//Description:
//      If the file is already open the file is closed. And if the
//      file name is known, i.e. non-zero, the file is opened for 
//      reading or writing (or for both) due to the mode given to
//      the constructor in creation. 
//
//      Returns zero if the opening is successful. Otherwise, returns
//      non-zero value. It is error to try to open the device without
//      giving the name of the file.
//
bool FileDevice :: open (void)
{
  Message *bmsg;

  _MT_SYNCMONITOR;

  if (-1 != fd) {
    close ();
  }    
  if (!name.empty()) {
    int m = 0;
    if ((mode & Device::READ) && (mode & Device::WRITE)) {
      m = O_RDWR | O_CREAT | O_TRUNC;
    } else if (mode & Device::READ) {
      m = O_RDONLY;
    } else if (mode & Device::WRITE) {
      m = O_WRONLY | O_CREAT | O_TRUNC;

    }

    if (-1 == (fd = ::open (name.c_str(), m, 0666))) {
      status = Device::ERROR;
      _DOORS_DEBUG(0,"open: cannot open " << name.c_str());
      DevIf::Close *cl = (DevIf :: Close *) iFace.create (DevIf :: close);
      cl->fd = fd;
      cl->device = this;
      bmsg = (Message *) cl;
      iFace.putMessage (bmsg);
      return false;
    }
  } else {
    DevIf::Close *cl = (DevIf :: Close *) iFace.create (DevIf :: close);
    status = Device::ERROR;

    cl->fd = fd;
    cl->device= this;
    bmsg = (Message *) cl;
    iFace.putMessage (bmsg);
    return false;
  }
  DevIf::Open *op = (DevIf :: Open *) iFace.create (DevIf :: open);
  op->fd = fd;
  op->device = this;
  bmsg = (Message *) op;
  iFace.putMessage (bmsg);
  _DOORS_DEBUG(0,"open, fd:" << getFd() << ", name:"
	    << name.c_str() 
	    << ", mode:" << form_status(mode)
	    << ", status:" << form_status(status));
  return true;
  
}




//
//Function: close
//
//Member-Of: FileDevice
//
//Return:
//      bool
//
//Description:
//      Closes the file.
//
//      Returns zero if the closing was successful. Otherwise, returns
//      non-zero value.
//
bool FileDevice :: close (void)
{
  Message *bmsg;
  _MT_SYNCMONITOR;

  _MT_KILLTHREAD(this);

  _DOORS_DEBUG(0,"FileDevice-close, fd:" << getFd());
  if(getFd() >= 0) {

    if (-1 == ::close (fd)) {
      _DOORS_DEBUG(0,"FileDevice-close, error on closing");
      return false;
    }

    status &= ~Device::EXCEPTION;
    status &= ~Device::READ;
    status &= ~Device::WRITE;
    status &= ~Device::EXCEPTION;
    status &= ~Device::TIMEOUT;
    status = Device::ERROR;

    DevIf::Close *cl = (DevIf :: Close *) iFace.create (DevIf :: close);
    cl->fd = fd;
    cl->device = this;
    bmsg = (Message *) cl;
    iFace.putMessage (bmsg);

    fd = -1;
    
  } else {
    _DOORS_DEBUG(0,"FileDevice-close: closing non-opened file descriptor");    
  }

  return true;
}

//
//Function: write
//
//Member-Of: FileDevice
//
//Parameters:
//      Frame &frm
//
//Return:
//      bool
//
//Implicit:
//      The frame, given as a parameter, is destroyed.
//
//Description:
//      Sets up the device for writing. Sets current output buffer to
//      frame given as parameter or combine it to old buffer.
//      Note that this function will empty (destroy) the original frame.
//      If an error happens during writing the Device::ERROR bit is 
//      set in the status field and the frame is disposed.
//
//      The paramter frm is the frame written, destroyed during writing.
//
//      Returns zero, always.
//
bool FileDevice :: write (Frame &frm)
{
  _MT_SYNCMONITOR;

  out.frame->combine (frm);
  status |= Device::WRITE;
  
  // write will be performed later in callback function

  _MT_SIGNAL_RUNLOCK;
  return true;
}


//
//Function: callbackRead
//
//Member-Of: FileDevice
//
//Description:
//      Callback routine called by the i/o handler when it notices
//      that there is something to read for this device. The read
//      request will be satisfied when there is even only one byte
//      read. It must be kept in mind that the maximum amount of data
//      that is read is the space in the last memory cell of the frame.
//      The data is sent to the task when either packetlenght is zero
//      or wsize is set for -1 (StreamDevice :: immediate). wsize is 
//      set to zero after data is sent and it must be set with 
//      devices readBytes function before every reading for the 
//      sake of synchronising.   
//

void FileDevice :: callbackRead (void)
{
  _MT_SYNCMONITOR;
  int nbytes;
  Message *bmsg = 0;

  wsize = in.getSize();
  packetLength = in.space;

  do {
    // allocate some space, if necessary
    in.buffer = in.frame->endAddress (&in.space);
    if (in.space == 0) {
      in.frame->endSpace ();
      in.buffer = in.frame->endAddress (&in.space);
    }

    if (0 < (nbytes = ::read (fd, (char*) in.buffer, (int) in.space))) {
      in.frame->endUpdate (nbytes);
    }

    if (nbytes == 0) {
    // nothing to read;
    //This may also mean end of file.  
      close();
      break;
    }
    
    if (nbytes < 0) {
      close();
      break;
    }
    
    // read succeed
    packetLength -= nbytes;
    
    if (wsize < 0 || packetLength == 0) {
      // packet is complete
      DevIf::Read *rd = (DevIf :: Read *) iFace.create (DevIf :: read);
      rd->fd = fd;
      rd->data = *in.frame;
      rd->device = this;
      bmsg = (Message *) rd;
      iFace.putMessage (bmsg);
      setRead (false);
      packetLength = 0;
      in.frame->destroy ();
      return;
    }
    
    in.buffer += nbytes;
    in.space -= nbytes;
    
    // repeat until read fails
  } while (in.space == 0);    
  
  return;

}

//
//Function: callbackWrite
//
//Member-Of: FileDevice
//
//Description:
//      The callback routine is called by the i/o handler when it
//      notices that writing could be done. When the whole frame
//      is written the writeAction function is called and the
//      Device::WRITE bit in the status field is cleared. When an 
//      error has occurred during writing the Device::ERROR bit 
//      is set and the frame is disposed. Writing can be done
//      in many parts due to the operation of the operating system.
//
void FileDevice :: callbackWrite (void)
{
  _MT_SYNCMONITOR;

  int nbytes;
  Message *bmsg;
  _DOORS_ERROR(&out.frame,"write without data");
  FrameSize len = out.frame->length ();
  while (len) {
    out.buffer = out.frame->address () + 1;
    out.space = out.frame->data ();


    if (-1 == (nbytes = ::write (fd,(const char *)out.buffer, (int) out.space))) {
      _DOORS_DEBUG(0,"write error: " << strerror(errno));
      status = Device::ERROR;
      DevIf::Close *cl = (DevIf :: Close *) iFace.create (DevIf :: close);
      cl->fd = fd;
      cl->device = this;
      bmsg = (Message *) cl;
      iFace.putMessage (bmsg);
      return;
    }
   

    len -= nbytes;
    out.frame->update (-nbytes);
    if (nbytes < out.space) {
      status |= Device::WRITE;
      out.frame->destroy();
      return; // partial write! we will continue later
    }
  }
  
  // write completed
  status &= ~Device::WRITE;
  out.frame->destroy();
  return;
}


//
//Function: callbackException
//
//Member-Of: FileDevice
//
//Description:
//      The callback routine called by the i/o handler when it notices 
//      that there is an execptional condition pending.
//
//      Not fully implemented.
//
void FileDevice :: callbackException (void)
{
  _MT_SYNCMONITOR;
  _DOORS_FAULT(0,"FileDevice: exception handling not implemented");
  status &= ~Device::EXCEPTION;
  return;
}

//
//Function: readBytes
//
//Member-Of: FileDevice
//
//Parameters:
//      Sint32
//
//Description:
//      Sets the size of buffer and makes device ready for reading.
//      If i is -1 (StreamDevice :: immediate), the device sends data
//      immediately to the task.

void FileDevice :: readBytes (Sint32 i)
{
  _MT_SYNCMONITOR;
  in.setSize(i);
  setRead (true);
}


//Function: readBuffer
//
//Member-Of: FileDevice
//
//Return: Sint32
//
//Description:
//  Return amount of data in read buffer.

Sint32 FileDevice :: readBuffer (void) 
{
  _MT_SYNCMONITOR;

  return in.frame->length();
}


//Function: writeBuffer
//
//Member-Of: FileDevice
//
//Return: Sint32 
//
//Description:
//  Return amount of data in write buffer.  
//  

Sint32 FileDevice :: writeBuffer (void) 
{
  _MT_SYNCMONITOR;

  return out.frame->length();
}
