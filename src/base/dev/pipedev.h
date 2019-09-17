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

$Log: pipedev.h,v $
Revision 1.5  2002/08/14 09:31:36  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.4  2002/07/25 10:08:38  ik
Use std::string.

Revision 1.3  2002/07/23 14:25:16  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:47  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:50  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:21  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef PILEDEV_H
#define PILEDEV_H

#include <string>
#include <doors/library.h>
#include <doors/strdev.h>
#include <doors/util.h>
#include <doors/buffer.h>
#include <doors/etask.h>
#include <doors/filedev.h>


class PipePort;

/**
  PipeDevice class is an interface to read and write to UNIX pipes. 
  */

class PipeDevice : public StreamDevice {
 public:
  /** Initializes the PipeDevice's variables with the given values.
    @param ioh is the IoHandler for the device.
    @param t is the task the device is attached to.
    @param fn is pointer to the name of pipe.
    */
  PipeDevice (IoHandler *ioh, EventTask *t, std::string fn);
  /**
    Closes the device and informs IoHandler to forget this device
    */
  virtual ~PipeDevice (void);

  void setName (std::string fileName, int = Device::READ);
  
  /** creates a pipe and opens it. Two filedevices are created.
    one for reading end of pipe and other for writing end of pipe.
    @return true is returned if pipe was created without problems.
    Otherwise false is returned.
    */
  bool open (void);
  /** Closes the pipedevice and informs task about it.
    @return false when closing is done.
    */
  bool close (void);
  /** Writes to the pipe.
    @return false if write fails. true is write is succesful.
    */
  bool write (Frame &frm);
  /**
    @return -1, since pipe has two filedescriptors. 
    Use getReadfd and getWriteFd.
    */
  virtual int getFd (void) const;
  /**
    @return FileDescriptor of read end of pipe.
    */
  int getReadFd(void);
  /**
    @return FileDescriptor of write end of pipe.
    */
  int getWriteFd(void);
  /** Closes the read end of pipe.
    @return true if succesful, else false.
    */
  bool closeRead (void);
  /** Closes the write end of pipe.
    @return true if succesful, else false.
    */
  bool closeWrite (void);
  /** This function is called from pipeport when it gets closed..
    */
  bool portClosed (Port* cPort);
  /** Defines the amount how much is wanted to read.
   */
  void readBytes (Sint32 i);
  _MT_KILLALL2(piper, pipew)
 protected:

  int p[2];
  int mode;
  int wsize;
  int fd;
  FrameSize packetLength;
  Name name;
  Buffer in;
  Buffer out;
  IoHandler* handler;
  EventTask* task;
  FileDevice* piper;
  FileDevice* pipew;
  PipePort *readPort;
  PipePort *writePort;

};

#endif  // PILEDEV_H














