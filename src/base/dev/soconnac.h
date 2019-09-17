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

$Log: soconnac.h,v $
Revision 1.3  2002/07/23 14:25:16  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:48  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.3  2002/02/25 17:06:05  ik
sockaddr.* split to inetaddr.* and unixaddr.* and the depending
files changed accordingly.

Revision 1.2  2001/06/08 16:46:02  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:26  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef SOCONNAC_H
#define SOCONNAC_H

//#include <doors/macroif.h>
#include <doors/hsi.h>
#include <doors/strdev.h>
#include <doors/buffer.h>
#include <doors/unixaddr.h>
#include <doors/inetaddr.h>


/** SocketConnectionAC is Abstract Class that different types 
  SocketConnections inherit. */

class SocketConnectionAC : public StreamDevice {
  public:
  /** Initializes the base class and own variables. This constructor is
    used when instance is created without actually connecting to 
    the peer entity. It can be done later with connect function call. 
    @param ioh the i/o handler of the system.
    @param t the task that holds the device. */
    SocketConnectionAC (IoHandler *io, EventTask *t);

    /** Initializes the base class and own variables. This constructor is
    used when instance is created and connected to  the peer entity. 
    @param ioh the i/o handler of the system.
    @param t the task that holds the device.
    @param sd descriptor for connection to be communicate with peer entity.*/
    SocketConnectionAC (IoHandler *io, EventTask *t, Uint32 sd);

    /** Initializes the base class and own variables. This constructor is
    used when instance is created and connected to  the peer entity. 
    @param ioh the i/o handler of the system.
    @param t the task that holds the device.
    @param a a address of the peer entity.*/
    SocketConnectionAC (IoHandler *io, EventTask *t, Address *a);

    /** destructor */
    ~SocketConnectionAC (void);

    /** Writes the given frame to the socket. Writing is asyncronous,
      that is the function returns immediately. The actual writing
      is done later by interoperation of callback routines
      of the device and the i/o handler. */
    bool write (Frame &frm);

    /** @return whether instance is connected or not. */
    bool connected (void);

    /** Sets the size of buffer and makes device ready for reading.
      @param i the size of buffer. If i -1 (StreamDevice :: immediate),
      the device sends data immediately to the task.  */
    virtual void readBytes (Sint32 i);

    /** Set the size of cell in buffers. 
      @param i wanted size of the buffers */
    virtual void setPacketSize (Sint32 i);

    /** @return the length of in buffers frame*/
    virtual Sint32 readBuffer (void);

    /** @return the length of out buffers frame*/
    virtual Sint32 writeBuffer (void);

  protected:
    bool _connected;

    FrameSize packetLength;
    int wsize;
    Buffer in;
    Buffer out;

    /** Determines the size of cell based to the size of in buffer.
      @return the size of cell */
    virtual Sint32 getPacketSize(void);
};


#endif
