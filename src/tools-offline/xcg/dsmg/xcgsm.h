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

$Log: xcgsm.h,v $
Revision 1.1  2002/06/17 13:29:56  bilhanan
*** empty log message ***

Revision 1.1  2002/06/14 13:25:49  hartmanj
Renamed from sm.X

Revision 1.2  2002/06/07 12:19:09  hartmanj
Full licence clause added


*/


// Defines the StateMachine.class

#ifndef SM_HH
#define SM_HH

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <doors/structure.h>
#include <doors/state.h>
#include <doors/event.h>



class SM : public Structure
{
      
 public:
      
	SM( const std::string &name, const std::string &pTaskName );
	SM();
	virtual ~SM();
      
	// Add state ...
	void addState( State *s );
  
	// add inheritance relationship.
	virtual void addParent(const std::string &par );
  
	// Adds interface if not already exists
	void addIface(const std::string &from, const std::string &name);
	// Adds event to global list of events and to particular state.
	bool addEvent(const std::string &state, const std::string &iface, Event *e);

	std::string getName(void) const;
  
	virtual void printHeader(std::ostream &header);
	virtual void printImpl(std::ostream &impl);
  
 private:
      
	// If the event already exists in particular interface
	bool eventExists(std::string iface, Event *e);
  
	void addEventGlobal(const std::string &iface, Event *e);
	void addEventState(const std::string &state, Event *e);
	int getFreeEventID() const;
	bool checkIfInState(const std::string &state, Event *e);
	int checkIfInGlobal(const std::string &iface, Event *e);

	// name of the StateMachine class
	std::string name_;
  
	// ptaskname
	std::string pTaskName_;

	// vector of states in statemachine
	std::vector<State*> states_;
	// parent classes
	std::vector<std::string> parents_;
  
	struct stateInfo
	{
		std::string name;
		Event *events;
	};
  
	// Events classified by states... 
	std::vector<stateInfo> eventsByStates_;
  
	struct ifaceInfo
	{
		std::string from;
		std::string name;
		Event *events;
	};
  
	// has all the ifaces and events belonging to each of interfaces.
	std::vector<ifaceInfo> ifaces_;
  
	// Number of unique events in statemachine.
	int events_;
};

#endif
