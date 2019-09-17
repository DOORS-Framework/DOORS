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

$Log: vstream.h,v $
Revision 1.7  2002/08/14 09:31:42  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.6  2002/07/25 22:41:10  ik
Fixes for std::string changes.

Revision 1.5  2002/07/25 15:38:44  ik
Use std::string.

Revision 1.4  2002/07/25 10:08:40  ik
Use std::string.

Revision 1.3  2002/07/23 14:25:23  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:54  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:46:08  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:29  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef VSTREAM_H
#define VSTREAM_H

#include <string>
#include <doors/library.h>
#include <doors/util.h>
#include <doors/etask.h>
#include <doors/port.h>
#include <doors/handler.h>
#include <doors/message.h>
#include <doors/ocoremac.h>

//Macro: LBUFSIZE
//Description: local buffer size
#define LBUFSIZE 256

//Macro: LBUFSIZE
//Description: global buffer size
#define GBUFSIZE 1024


class VstreamMessage;
class VostreamProxy;

enum VstreamCtrl {
    endv,
    vend,
    vflush
};


/** Vostream forms an abstract interface to the user of logging
  messages. The interface is abstract in means the logging
  device in not known or considered, and as weel as the
  format of log as abstract. */
  
class Vostream : public EventTask {
    public:
        enum Type {
	    EMPTY, // in any case empty log-message
	    END,   // ending log-message
	    VAR,   // general variable data e.g. frame
	    MSG,   // log-message from the message
	    TASK,  // log-message of task
	    INFO,  // informationnal log
	    USER,  // user data log
	    ERROR, // error log-messages
	    INPUT, // input from user
            DIRE,  // direct output to stdout with printf
	    NROF   // number of log-message-types
        };
#if _MT_ENABLED
        Vostream (Vostream::Type t, VostreamProxy *proxy);
#else
        Vostream (Vostream::Type t, Scheduler *s = 0);
#endif
        ~Vostream (void);
        // primitive members
        Vostream &operator<< (Vostream::Type);
        Vostream &operator<< (VstreamCtrl); 
        Vostream &operator<< (const bool);
        Vostream &operator<< (const char);
        Vostream &operator<< (const unsigned char);
        Vostream &operator<< (const int);
        Vostream &operator<< (const unsigned int);
        Vostream &operator<< (const long);
        Vostream &operator<< (const unsigned long);
        Vostream &operator<< (const float);
        Vostream &operator<< (const double);
        Vostream &operator<< (const char *);
        Vostream &operator<< (std::string);
        Vostream &spaces (int);
        Vostream &levelIncrement (void);
        Vostream &levelDecrement (void);
        Vostream &levelSet (int);
        // complex
        Vostream &operator<< (Frame &);
        Vostream &operator<< (OTime &);
        // functional
        void putOutput (Port *);            // attach port to broadcast list
        void getOutput (Port *);            // deattach port to broadcast list
        void propagate (VstreamMessage *m); // send msg to every port
    private:

#if _MT_ENABLED
        TQueue<Port> &oports;
#else
        TQueue<Port> oports;
#endif

        Type type;
        Type def;
        int level;
        std::string scope;
        char buf[GBUFSIZE+1];
};




/** VstreamMessage is a kind of message that Vostream sends to the
  Logger. VstreamMessage contains the the logical information
  related to the one logging event. */


class VstreamMessage : public Message {
public: 

  /** An instance of VstreamMessage is created.
    @param t Vostream messagetype.
    @param l the message level.
    @param d the message data 
    @param s  the scope definition. */
  VstreamMessage (Vostream::Type, int, std::string, std::string);

  /** Data is copied from the VstreamMessage
   @param m is the message data is copied from to this message*/
  VstreamMessage (const VstreamMessage &m);

  /** Data and scope are deleted */
  virtual ~VstreamMessage (void);
    
  Vostream::Type type;
  int level;
  std::string data;
  std::string scope;
};


inline VstreamMessage::VstreamMessage (Vostream::Type t, int l,
                                       std::string d, std::string s)
    : type (t), level (l), data (d), scope (s)
{
}


struct VostreamItem {
  int threadId;
  Vostream *stream;
};


/**      VostreamProxy forms an abstract interface to the user of 
  logging messages in multithreaded DOORS. The interface is
  same as in Vostream. It holds thread specific Vostream instances 
  and re-directs the output to the proper instance. */


class VostreamProxy : public EventTask {
    public:
  /** constructor 
    @param t is type of Vostream
    @param s is the system scheduler */

  VostreamProxy (Vostream::Type t, Scheduler *s = 0);

  /** destrcutor */
  ~VostreamProxy (void);

  // primitive members
  Vostream &operator<< (Vostream::Type);
  Vostream &operator<< (VstreamCtrl); 
  Vostream &operator<< (const bool);
  Vostream &operator<< (const char);
  Vostream &operator<< (const unsigned char);
  Vostream &operator<< (const int);
  Vostream &operator<< (const unsigned int);
  Vostream &operator<< (const long);
  Vostream &operator<< (const unsigned long);
  Vostream &operator<< (const float);
  Vostream &operator<< (const double);
  Vostream &operator<< (const char *);
  Vostream &spaces (int);
  Vostream &levelIncrement (void);
  Vostream &levelDecrement (void);
  Vostream &levelSet (int);
  // complex
       Vostream &operator<< (Frame &);
  Vostream &operator<< (OTime &);
  // functional
  /** Attach the given port to broadcast list.
   @param port the port to be attached*/
        void putOutput (Port *); 
  /** Deattach the given port to boradcast list
    @param port the port to be deattached */
  void getOutput (Port *);           
  /** send message to all ports 
    @param m the message to be sent */
  void propagate (VstreamMessage *m); 
    /** @return the list of ports */
    TQueue<Port> *getPorts (void);

    Vostream :: Type type; 
    Vostream &getStream (void);
    void removeStream (Vostream *s);
    
private:
    TQueue<Port> oports;
    TQueue<VostreamItem> streamList;
};



#if _MT_ENABLED
extern VostreamProxy vout;
extern VostreamProxy verr;
#else
extern Vostream vout;
extern Vostream verr;
#endif

#endif // VSTREAM_H 






