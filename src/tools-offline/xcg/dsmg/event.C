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

$Log: event.C,v $
Revision 1.1  2002/06/17 13:29:56  bilhanan
*** empty log message ***

Revision 1.3  2002/06/14 13:27:20  hartmanj
Reordering includefiles

Revision 1.2  2002/06/07 12:19:09  hartmanj
Full licence clause added


*/


// File that implements the Event-class

#include <iostream>
#include <string>

#include <doors/event.h>

using std::cout;
using std::endl;
using std::string;
using std::ostream;

// Constructor of Event-class
Event::Event(const string &iface, const string &name) 
  : name_(name),
    eventID_(-1),
    action_("alldefault"),
    iface_(iface),
    nextEvent_(0)
{
  // Not much to do after member initialization
}

// Destructor
Event::~Event()
{
  // set the nextEvent_ pointer to NULL
  nextEvent_ = 0;
}
// Copy constructor
Event::Event(const Event &oldEvent) 
  : name_(oldEvent.name_),
    eventID_(oldEvent.eventID_),
    action_(oldEvent.action_),
    iface_(oldEvent.iface_),
    nextEvent_(0)
{
  // Nothing more to do..
}

// Event::getName() : returns Event's name
string Event::getName() const
{
  return name_;
}

//Event::setEventID(id) : set eventId to event
void Event::setEventID(int id)
{
  eventID_ = id;
}

//Event::getEventID(id) : get event's ID
int Event::getEventID() const
{
  return eventID_;
}

//Event::setAction(action) : set action to this event
void Event::setAction(string &action)
{
  action_ = action;
}

//Event::getAction() : get Action assigned to this event
string Event::getAction() const
{
  return action_;
}

// //Event::setNextEvent(Event) : set Next event in list
void Event::setNextEvent(Event * next)
{
  nextEvent_ = next;
}

//Event::getNextEvent() : get next Event in list
Event* Event::getNextEvent()
{
   return nextEvent_;
}

//Event::getIface() : get Interface that Event belongs to
string Event::getIface() const
{
   return iface_;
}
