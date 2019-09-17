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

$Log: device.h,v $
Revision 1.3  2002/08/14 09:31:35  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/07/23 14:25:15  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:27  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:12  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef DEVICE_H
#define DEVICE_H

#include <doors/library.h>
#include <doors/handler.h>
#include <doors/util.h>
#include <doors/hsimacro.h>

/**
  The device is the base class of all devices in a DOORS
  system. It contains vital information that the i/o handler
  needs for its operations, for example, fd, status, and 
  time-out time.<p>

  Before a device is used for reading or writing, for example,
  it must be opened by calling the open function. And before
  deletion a device must be closed with a close function.
  Reading and writing is handled by the read and write 
  functions. They are both asyncronous. The i/o handler will 
  call afterwards the corresponding callback routines to 
  accomplish the read or write requests. The i/o handler will
  call the callback routines as long as the status bits are
  set for reading or writing. So, it is a device's duty to 
  clear the bits when requests are handled.<p>
  
  A driver task, which is the task that uses a device, can be 
  written by inheriting the device. So, the task itself is 
  a device. And by doing this the driver task can be written 
  to do, after the whole read or write request is served, send 
  data to the upper tasks. In this way we do not have to poll. <p>
  
  None of the routines of a device must not block.<p>

  */
class Device {
 public:
   enum Mode {  // status bits of a device
     IDLE      = 0x01,
     READ      = 0x02,
     WRITE     = 0x04,
     EXCEPTION = 0x08,
     TIMEOUT   = 0x10,
     ERROR     = 0x20
     };
   /** The constructor initializes all device's variables and tells
      the i/o handler about the existence of this device. Without 
      the i/o handler the device cannot operate. If it is not given
      to the constructor the user must herself call the inform 
      function of the i/o handler. 

      @param ioHandler The ioHandler parameter is a pointer to the i/o handler of 
      the system. 
      */
   Device (IoHandler *ioHandler);
   /** The destrcutor tells to the i/o handler that this device
     is no longer in use
     */
   virtual ~Device (void);

   virtual bool open (void) {return false;};
   virtual bool close (void) {return false;};
   virtual bool read (Frame &) {return false;};
   virtual bool write (Frame &) {return false;};
  
   virtual int getFd (void) const = 0;
   virtual OTime getTimeout (void) const;
   int getStatus (void) const;
   virtual void setRead (bool r);
   void setWrite (bool w);


   virtual void callbackRead (void) {return; };
   virtual void callbackWrite (void) {return; };
   virtual void callbackException (void) {return; };
   virtual void callbackTimeout (void) {return; };

   _MT_GETSYNCLOCK
   _MT_GETRUNLOCK 
   _MT_GETEXITBARRIER 
   _MT_ACTIVE_DEVICERUN
   _MT_DEVICERUN_NULL
   _MT_ISALIVE
   _MT_KILL
   _MT_DEVICESCHEDULE
   _MT_POLLDEVICE

 protected:
   int status;
   IoHandler *handler;
   static const OTime undefined;

   _MT_DEVICE_VARIABLES
   _MT_ALIVE
   _MT_SYNCLOCK
   _MT_RUNLOCK
   _MT_EXITBARRIER
};


//
//Inline-Functions:
//

//
//Function: open
//
//Member-Of: Device
//
//Return: bool
//
//Description:
//      This function is for opening the physical device or file.
//      After opening the device should be in state where it can be
//      used. 
//
//      This implementation is a dummy function. 
//
//Commentary: 
//      This function is defined because it is not needed always in
//      derived classes. Otherwise it must be always defined in 
//      every derived class. 
//

//
//Function: close
//
//Member-Of: Device
//
//Return: bool
//
//Description:
//      This function is for closing the physical device or file.
//      Every device should be closed after it is not in use any more.
//
//      This implementation is a dummy function. 
//
//      The function should return zero when it was successful,
//      otherwise non-zero.
//
//Commentary: 
//      This function is defined because it is not needed always in
//      derived classes. Otherwise it must be always defined in 
//      every derived class. 
//

//
//Function: read
//
//Member-Of: Device
//
//Return: bool
//
//Description:
//      This function is for reading from the device. Reading is 
//      asyncronous. Actual reading happens later when the i/o handler
//      notices that it can be done. 
//
//      This implementation is a dummy function. 
//
//      The function should return zero when it was successful,
//      otherwise non-zero.
//
//Commentary: 
//      This function is defined because it is not needed always in
//      derived classes. Otherwise it must be always defined in 
//      every derived class. 
//

//
//Function: write
//
//Member-Of: Device
//
//Return: bool
//
//Description:
//      This function is for writing from the device. Writing is 
//      asyncronous. Actual writing can happen later when the i/o handler
//      notices that it can be done. 
//
//      This implementation is a dummy function. 
//
//      The function should return zero when it was successful,
//      otherwise non-zero.
//
//Commentary: 
//      This function is defined because it is not needed always in
//      derived classes. Otherwise it must be always defined in 
//      every derived class. 
//

//
//Function: callbackRead
//
//Member-Of: Device
//
//Description:
//      This function is called by the i/o handler when it finds out
//      that the device can be read and there is a read request 
//      binding. The function should handle the reading of the device. 
//
//      This implementation is a dummy function. This is private 
//      member function and should be used only by the i/o handler.
//
//Commentary: 
//      This function is defined because it is not needed always in
//      derived classes. Otherwise it must be always defined in 
//      every derived class. 
//

//
//Function: callbackWrite
//
//Member-Of: Device
//
//Description:
//      This function is called by the i/o handler when it finds out
//      that the device can be written and there is a write request 
//      binding, for example when a partial write has happened. 
//      The function should handle the writing of the device. 
//
//      This implementation is a dummy function. This is private 
//      member function and should be used only by the i/o handler.
//
//Commentary: 
//      This function is defined because it is not needed always in
//      derived classes. Otherwise it must be always defined in 
//      every derived class. 
//

//
//Function: callbackException
//
//Member-Of: Device
//
//Description:
//      This function is called by the i/o handler when it finds out
//      that there is exceptional condition binding.
//
//      This implementation is a dummy function. This is private 
//      member function and should be used only by the i/o handler.
//
//Commentary: 
//      This function is defined because it is not needed always in
//      derived classes. Otherwise it must be always defined in 
//      every derived class. 
//

//
//Function: callbackTimeout
//
//Member-Of: Device
//
//Description:
//      This function is called by the i/o handler when it finds out
//      that a time-out has expired.
//
//      This implementation is a dummy function. This is private 
//      member function and should be used only by the i/o handler.
//
//Commentary: 
//      This function is defined because it is not needed always in
//      derived classes. Otherwise it must be always defined in 
//      every derived class. 
//

//
//Function: getStatus
//
//Member-Of: Device
//
//Return: int
//
//Description::
//      Returns the status field that contains the information about
//      what the i/o handler should do with this device.
//

inline int Device :: getStatus (void) const
{
    _MT_SYNCMONITOR;
    return status;
}

#endif  // DEVICE_H












