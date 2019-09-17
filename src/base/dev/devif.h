// -*- C++ -*-
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

$Log: devif.h,v $
Revision 1.3  2003/05/10 18:22:54  bilhanan
Obsolete references to OVOPS removed.

Revision 1.2  2002/06/03 09:19:47  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:27  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:12  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef _devif_h_
#define _devif_h_
#include <doors/iface.h>
#include <doors/sap.h>

class StreamDevice;

/** Interface for the messages from device to task.
  */
class DevIf {
public:

    /** Service provider use interface for requests. */
    class Provider : public IndIface {
    public:
	/** create base interface */
        Provider (EventTask *t);
        ~Provider (void);
    };
    
    /** Service user upcall interface for indications. */
    class User : public ReqIface {
    public:
	/** create base interface */
        User (EventTask *t, Peer *p = 0);
        ~User (void);
    };
    
    /** Messages types for request interface. */
    enum ReqType { dummy };

    /** Message types for indication interface. */
    enum IndType { open = 1, close, timeout, read };

    /** Open indication message primitive. */
    class Open : public Message {
    public:
        Open (void);
        Open (const Open &msg);
        ~Open (void);
	
        Message *clone (void) const;
	
        MessageType getType (void) const;

        int fd;
        StreamDevice *device;
    };
    
    /** Close indication message primitive. */
    class Close : public Message {
    public:
	Close (void);
        Close (const Close &msg);
        ~Close (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        int fd;
        StreamDevice *device;
    };

    /** Timeout indication message primitive. */
    class Timeout : public Message {
    public:
        Timeout (void);
        Timeout (const Timeout &msg);
        ~Timeout (void);
	
        Message *clone (void) const;

        MessageType getType (void) const;

        int fd;
        StreamDevice *device;
    };

    /** Read indication message primitive. */
    class Read : public Message {
    public:
        Read (void);
        Read (const Read &msg);
        ~Read (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        int fd;
        StreamDevice *device;
        Frame data;
    };
};


inline MessageType DevIf :: Open :: getType (void) const {

  return open;
}


inline MessageType DevIf :: Close :: getType (void) const {

  return close;
}


inline MessageType DevIf :: Timeout :: getType (void) const {

  return timeout;
}


inline MessageType DevIf :: Read :: getType (void) const {

  return read;
}




#endif
