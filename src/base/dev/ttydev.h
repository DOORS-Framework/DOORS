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

$Log: ttydev.h,v $
Revision 1.3  2002/07/23 14:25:17  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:48  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:06  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:27  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef TTYDEV_H
#define TTYDEV_H

#include <doors/filedev.h>

/** The device class for reading and writing to a ttydevice in
  UNIX operating system. It is used as interface for accessing
  terminal. */
  
class TtyDevice : public FileDevice {
 public:
  enum Type {READ = 1, WRITE = 2};

  /**The constructor initializes the device.
    @param ioh i/o handler of system.
    @param t task the TyDevice is attached to.
    @param type the type of TtyDevice. */
  TtyDevice (IoHandler *ioh, EventTask *t, Type type, Sint32 size = 256); 

  /** destructor */
  virtual ~TtyDevice (void);

  /** Checks the mode of device and sets the standard
    file descriptors for either input or output. */
  virtual bool open (void);

  /** Clear the values of the device but does not
    actually close the file descriptor. It cannot
    be done because the device did not actually
    open the file descriptor in the first place. */
  virtual bool close (void);

};

#endif  // TTYDEV_H














