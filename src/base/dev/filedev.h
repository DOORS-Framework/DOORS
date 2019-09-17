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

$Log: filedev.h,v $
Revision 1.4  2002/07/25 10:08:38  ik
Use std::string.

Revision 1.3  2002/07/23 14:25:15  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:47  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:29  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:13  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef FILEDEV_H
#define FILEDEV_H

#include <string>
#include <doors/library.h>
#include <doors/strdev.h>
#include <doors/util.h>
#include <doors/buffer.h>
#include <doors/etask.h>

/**      The device class for reading and writing to a file in
  UNIX operating system. It is used as interface for accessing
  files. 
  */

class FileDevice : public StreamDevice {
public:

  /** The constructor initializes the device and leaves it to
    an idle state waiting for the call of open member function,
    which finally opens the file. </p>
    The name is duplicated, i.e. the file device creates its
    own copy of the name. 
    @param ioh the  i/o handler of the system,
    @param t the task where the device port is connected
    @param fn the name of the file wanted to open
    @param size the size of in and out buffers of task. 
    @param m the mode, Device::READ, Device::WRITE (or their combination).
    */
  FileDevice (IoHandler *ioh, EventTask *t, std::string fn, Sint32 size = 256, int = Device::READ );
  FileDevice (IoHandler *ioh, EventTask *t, std::string fn, Sint32 size, int m, int filed);

  /** Destructor closes the file and informs i/o handler to forget this device.
   */
  virtual ~FileDevice (void);

  /** sets the name of this device to name and mode to m
    @param name the name of a file associated with 
    this device.
    @param m the mode of device.  
    */
  void setName (std::string fileName, int = Device::READ);

  /** @return the fd associated with device. */
  virtual int getFd (void) const;

  /** If the file is already open the file is closed. And if the
    file name is known, i.e. non-zero, the file is opened for 
    reading or writing (or for both) due to the mode given to
    the constructor in creation. 
    @return zero if the opening is successful. Otherwise, returns
    non-zero value. It is error to try to open the device without
    giving the name of the file.
    */
  virtual bool open (void);

  /** Closes the file.
    @return  zero if the closing was successful. Otherwise, returns
    non-zero value.*/
  virtual bool close (void);

  /** Sets up the device for writing. Sets current output buffer to
    frame given as parameter or combine it to old buffer.
    Note that this function will empty (destroy) the original frame.
    If an error happens during writing the Device::ERROR bit is 
    set in the status field and the frame is disposed.
    @param frm is the frame written, destroyed during writing.
    @return zero, always. */
  virtual bool write (Frame &frm);

  /** Callback routine called by the i/o handler when it notices
    that there is something to read for this device. The read
    request will be satisfied when there is even only one byte
    read. It must be kept in mind that the maximum amount of data
    that is read is the space in the last memory cell of the frame.
    The data is sent to the task when either packetlenght is zero
    or wsize is set for -1 (StreamDevice :: immediate). wsize is 
    set to zero after data is sent and it must be set with 
    devices readBytes function before every reading for the 
    sake of synchronising. */
  virtual void callbackRead (void);

  /** The callback routine is called by the i/o handler when it
    notices that writing could be done. When the whole frame
    is written the writeAction function is called and the
    Device::WRITE bit in the status field is cleared. When an 
    error has occurred during writing the Device::ERROR bit 
    is set and the frame is disposed. Writing can be done
    in many parts due to the operation of the operating system. */
  virtual void callbackWrite (void);

  /** The callback routine called by the i/o handler when it notices 
      that there is an execptional condition pending.
      */
  virtual void callbackException (void);
  
  /** Sets the size of buffer and makes device ready for reading.
    If i is -1 (StreamDevice :: immediate), the device sends default
    amount of data to the task. */
  void readBytes (Sint32 i);
  
  /** reading readBuffer size   
    @return amount of data in read buffer */
  virtual Sint32 readBuffer (void);
  
  /** reading writeBuffer size.
    @return amount of data in write buffer. */  
  virtual Sint32 writeBuffer (void);

 protected:
  int fd;
  int mode;
  int wsize;
  FrameSize packetLength;
  Name name;
  Buffer in;
  Buffer out; 
};

/** Function for forming string out of filedevice status.
  @return String of filedevice status.
 */
extern std::string form_status(int status);

#endif  // FILEDEV_H














