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

$Log: message.h,v $
Revision 1.5  2003/11/14 10:03:38  bilhanan
reintroduced unsignedness.

Revision 1.4  2003/11/12 17:46:38  bilhanan
added signedness.

Revision 1.3  2003/10/15 10:48:28  bilhanan
added broadcast message functionality.

Revision 1.2  2002/07/23 14:25:21  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:41  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.5  2001/06/08 08:39:17  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef MESSAGE_H
#define MESSAGE_H


#include <doors/util.h>
#include <doors/obl_dcl.h>

/* Basic types of message. */
enum SystemMessageType { M_TIMER, M_INPUT, M_OUTPUT };

/**
  The message is not usually used as it is, but it is inherited.
  So, it is the base class of all messages used in a DOORS
  system. 
  */
class Message : public Link {
public:
    /** Sets the all data members to zero. */
    Message (void);

    /** Copy constructor of Message */
    Message (const Message &msg);

    virtual ~Message (void);

    /**
      This is a dummy function. Returns always -1.
      The function should return the type of this message.
      This function must be implemented in the class inherited from
      this class, or wherever the type of the message is needed. 

      This version of the function exists just because otherwise
      it should be implemented in every message inherited from
      this class, although it is not needed in every case.
      */
    virtual MessageType getType (void) const { return -1; };

    /**
      Assigns source and target ports of the message by
      given parameters if the message is NOT in locked
      state. Otherwise the previous ports values are
      kept and the message returns back to the unlocked
      state.
      
      First parameter s is new source port. Second parameter
      t is new target port.
      */
    void mark (Port *s, Port *t);

    /**
      Moves the message into locked state. It means that when 
      the message travels through the port pair its source 
      and target ports will be not updated. Also, locked
      messages are never deleted after execution.
      
      Note: Initial state of the message is unlocked.
      */
    void lock (void);

    /**
      Unlocks the message. It means that when 
      the message travels through the port pair its source 
      and target ports will be updated.

      Note: Initial state of the message is unlocked, so
      you don't have to unlock every message explicitely but
      only after it
      */
    void unlock (void);

    /**
      Returns true if the message is in locked state. Otherwise
      the return value is false.
      */
    bool isLocked (void) const;

    /**
      Purpose of this function is to give possibility to 
      take a clone of any type of messages inherited from
      this class. To have that feature user should implement
      this function again in his inherited classes to return
      a copy instance of that class.  
      @return Returns a new instance of message which is a clone of
      this message.
      */
    virtual Message *clone (void) const;

    /** Return the sender port of the message. */
    Port *getSender (void) const {return sender;}

    /** Return the target port of the message. */
    Port *getTarget (void) const {return target;}

    /** Return whether this message is for single or multiple receipients. */
    bool isForMany (void) const {return MultipleTargets;}

    /** Sets message propagation type. If intended for multiple
	receipients, set the parameter to true.
    */
    void setMultipleTargets (bool Multiple_=true);

    /** Return number of receipients for this message */
    Uint16 get_rcpt_count (void) const {return rcpt_count;}

    /** Increment the number of receipients by one */
    void increment_rcpt_count (void);

    /** Decrement the number of receipients by one
        and return the new value                   
    */
    Uint16 decrement_rcpt_count (void);

private:
    bool freezed;
    Port *target;
    Port *sender;
    bool MultipleTargets;
    Uint16 rcpt_count;
};

/**
  The system message is used only for needs of the DOORS system.
  Such a need is, for example, the time-out message, which is 
  inherited from the system message. System messages are handled
  separetly from user messages.
  */
class SystemMessage : public Message {
public:
    /** Creates a system message of the given type. */
    SystemMessage (SystemMessageType);

    MessageType getType (void) const;

protected:
    SystemMessageType type;
};

inline SystemMessage :: SystemMessage (SystemMessageType t) : type (t)
{
}

inline MessageType SystemMessage :: getType (void) const
{
    return (MessageType)type;
}

#endif  // MESSAGE_H

