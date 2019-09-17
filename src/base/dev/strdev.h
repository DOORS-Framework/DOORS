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

$Log: strdev.h,v $
Revision 1.4  2002/08/14 09:31:37  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/23 14:25:16  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:48  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:03  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:26  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef STREAMDEV_H
#define STREAMDEV_H


#include <doors/library.h>
#include <doors/device.h>
#include <doors/util.h>
#include <doors/devif.h>
/** StreamDevice class gives general interface for the stream based devices.
  */
class StreamDevice : public Device {
 public:
  /** Initializes the PipeDevice's variables with the given values.
    @param ioh is the IoHandler for the device.
    @param t is the task the device is attached to.
    */
  StreamDevice (IoHandler *ioh, EventTask *t);
  /** Nothing is done in destructor.
   */
  virtual ~StreamDevice (void);  
  virtual void readBytes (Sint32 i) = 0;
  virtual Sint32 readBuffer (void);
  virtual Sint32 writeBuffer (void);
  /**
    @return The task the device is attached to.
    */
  EventTask *getTask (void); 
  _MT_DEVICERUN
    
  /** the interface port through which messages from device go to task.
   */
  DevIf :: Provider iFace;
  enum {immediate = -1};

 protected:
  /** Makes the device status readable or non-readable.
   */
  void setRead (bool r);

  EventTask *task;
};

#endif  // STREAMDEV_H


