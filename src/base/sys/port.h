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

$Log: port.h,v $
Revision 1.4  2002/08/09 07:41:04  hartmanj
#include <debug.h> to #include <doors/debug.h>

Revision 1.3  2002/07/23 14:25:22  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:54  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:51  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:21  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef PORT_H
#define PORT_H

#include <doors/debug.h>

#include <doors/library.h>
#include <doors/etask.h>
#include <doors/message.h>

class Message;

/** doc++ fix */

/**
  A port class is for constructing a communication channel
  between two tasks. One port is needed in both ends. This 
  is the basic port that is intended to be used inside one 
  operating system process. <p>

  Usually you want to inherit this port to your own port that
  you use in your task. The port of your own can be used 
  to hide the creation of messages by writing a function 
  interface for all messages that can be sent through the 
  port. <p>

  Before any messages can be transfered through the port it 
  must be connected to another port. And when you do not want
  to use the port any more you should call the disconnect 
  function or you should delete the port, if it is created
  dynamically. The port can be connected to one port at  
  time, only. <p> 

  The port can be given a table that consists of names of
  functions and function pointers to functions that create 
  the specific message. This feature is typically used by a 
  port that needs to build the received message by decoding
  it first from another form.
  */
class Port {
public:
    /** Initializes the port's variabled with the given values. 
      @param host The host parameter is a pointer to the task to which 
      this port belongs. 
      */
    Port (EventTask *host);

    /**
      Disconnects the port, so the other port knows that
      the channel is no longer in use. 
      */
    virtual ~Port (void);
    
    /** The connect function is used to connect the port to another port
      constructing a communication channel between two ports. The port 
      can be connected only if the port is not connected already. Use 
      the disconnect function to disconnect the port first. 

      When connecting two ports together the connectRequest function 
      of the other end port is called, which may refuse to accept
      the connection. The connectionRequest will call the connect
      function of the event task, which can be used to monitor all
      the connection requests the task gets. 

      @param to The parameter to is the port to which we want to be connected.
      @return Returns zero if the connection was successful. Otherwise
      returns non-zero value. 
      */
    virtual bool connect (Port *to);

    /** Returns the non zero value if the port is connected. Otherwise, 
      returns zero.
      */
    virtual bool connected (void);

    /**
      The disconnect function is called when the communication 
      channel is not needed any more. 

      The port will be disconnect whether the other end wants it
      or not. 

      When disconnecting the disconnect function of the event task 
      to which this port belongs is called. The task's disconnect
      function can be used to monitor all the connections from 
      the task. Also, the internal function, disconnectRequest, 
      of the other end is called to confirm the disconnection. 
      The disconnectRequest will also call the disconnect function
      of the event task. So, in the both ends (in event tasks) the 
      disconnect function is called. 

      @return Returns zero if the disconnection was accepted by the other
      end. Otherwise, returns non-zero value. Non-zero value is also
      returned if the port was not connected at all. 
      */
    virtual bool disconnect (void);
    
    /** <b>Sends</b> a message to the recipient's port, which takes care
      of storing it, or what ever it will do with it.

      Assigns sender's and target's port address in the message 
      and calls the getMessage function of the other end's port.

      @return If a message is received successfully zero is returned. 
      Otherwise non-zero value is returned. 
      */
    virtual bool putMessage(Message *msg);

    /**
      Receives the given message from other port. Called by 
      the putMessage function of other end's port when sending
      a message. The received message is stored into a message
      queue of the task to which this port belongs. Otherwise
      the message is deleted and the message reference is set
      to zero.

      @return If a message is received successfully zero is returned. 
      Otherwise non-zero value is returned. 
      */
    virtual bool getMessage (Message *msg);
    
    /** Returns non-zero value if the port a is connected to the port b. */
    static bool compareOther (Port *a, Port *b);

    /** Return other port. */
    Port *getOther (void);

    /** Return the task of port. */
    EventTask *getTask (void);

    /**
      The function, which is a protected member function, is called 
      when other port is disconnected. This may happen, for example, 
      when the port is been deleted. 
      */
    virtual bool disconnectRequest (Port *);

    /**
      The connectRequest is a protected member function of the port.
      It is used by the connect function to ask the other end to
      confirm the connection. The confirmation is done by the
      connect function of the event task, which is called also, 
      by default. 
      */
    virtual bool connectRequest (Port *to);

protected:
    Port *other;
    EventTask *task;
};


/**
  A Systemport class has same functionality as the Port class. Only 
  difference between these classes appears when the port gets
  a message. The system port saves the message into system message 
  queue of the task instead of user message queue.
  */
class SystemPort : public Port {
    public:
    SystemPort (EventTask *host = 0) 
	: Port (host) {};
    
    // impl Port
    virtual bool getMessage (Message *msg);
};

#endif  // PORT_H

