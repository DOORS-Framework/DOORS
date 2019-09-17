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

$Log: pipeport.h,v $
Revision 1.3  2002/07/23 14:25:22  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:54  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:50  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:21  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef PIPEPORT_H
#define PIPEPORT_H

#include <debug.h>

#include <doors/library.h>
#include <doors/etask.h>
#include <doors/message.h>
#include <doors/port.h>
#include <doors/pipedev.h>
class Message;

/** doc++ fix */

/**
  A PipePort class is for constructing communication channel between 
  task and the ends of pipe. The incoming messages from pipes end are 
  copied to the tasks device port. This is used in PipeDevice.
  */
class PipePort : public Port { 
  public:
    /** Initializes the PipePort's variabled with the given values. 
    @param pdevport The pdevport parameter is a pointer to the port of 
    the task the device is connected. 
    @param pdev is pointer to the PipeDevice this port belongs to.
    */
    PipePort (Port *pdevport, PipeDevice *pdev);
    /** Disconnects the port, so the other port knows that the channel
    is no longer in use. 
    */
    virtual ~PipePort ();
    /** receives the message from FileDevice representing the end of pipe
    and copies the message to the tasks device interface port.
    @return If a message is received succesfully zero is returned. 
    Otherwise non-zero value is returned.
     */
    virtual bool getMessage (Message *msg);
    
  protected:
    Port* pPort; 
    PipeDevice* device;
};


#endif  // PIPEPORT_H


