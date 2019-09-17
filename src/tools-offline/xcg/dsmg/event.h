//Editor-Info: -*- C++ -*-

//  Copyright 2000
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.

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

$Log: event.h,v $
Revision 1.1  2002/06/17 13:29:56  bilhanan
*** empty log message ***

Revision 1.2  2002/06/07 12:19:09  hartmanj
Full licence clause added


*/


// Defines class that represents an event in statemachine

#ifndef EVENT_HH
#define EVENT_HH

#include <iostream>
#include <string>

class Event {
  
public:
  
  Event();
  // Constructor takes two parametres: interface to which the Event belongs and name of the event
  Event( const std::string &iface, const std::string &name );
  ~Event();
  
  // Copy constructor
  Event(const Event &oldEvent);

  // name of the event
  std::string getName() const;
  
  // sets unique eventID to event
  void setEventID(int id);
  
  // returns the eventID
  int getEventID() const;
  
  // set Action to be called in case of this event.
  // Action is State-spesific,
  void setAction(std::string &action);
  
  // get action currently assigned to this event 
  std::string getAction() const;
  
  // Methods to handle linked list of events...
  void setNextEvent( Event *);
  Event* getNextEvent();
  
  // returns the interface that this Event belongs.
  std::string getIface() const;

private:
  
  // name of the event
  std::string name_;
  
  // eventid
  int eventID_;
  
  // action to be called
  // This action is State-specific.
  // There should be considered different kind of solution...
  
  std::string action_;
  
  // interface that Event belongs.
  std::string iface_;
  
  // pointer to nextEvent
  Event* nextEvent_;
  
};

#endif
