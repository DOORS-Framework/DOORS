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

$Log: xcgsm.C,v $
Revision 1.3  2002/07/25 09:53:57  hartmanj
Replaced const char * to std::string in the generation of stateName() method

Revision 1.2  2002/07/23 14:36:57  ik
OdBoolean -> bool

Revision 1.1  2002/06/17 13:29:56  bilhanan
*** empty log message ***

Revision 1.1  2002/06/14 13:25:49  hartmanj
Renamed from sm.X

Revision 1.2  2002/06/07 12:19:09  hartmanj
Full licence clause added


*/


// implements the stateMachine-class 

// NOTICE check the indexing.......

#include <doors/xmldocument.h> // for StrToLower()
#include <doors/xcgsm.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ostream;

// global variable used in indenting the code
extern int ilevel;

// global function that handles the code indentation
string TAB()
{
	string s;
	for(int i=0;i<ilevel;i++)
		s.append("  ");
	return s;
}

// constructor
SM::SM(const string &name, const string &pTaskName)
	: Structure(),
	  name_(name),
	  pTaskName_(pTaskName),
	  states_(),
	  parents_(),
	  eventsByStates_(),
	  ifaces_(),
	  events_(0)
{
	// StateMachine class is always inherited from StateMachine
	parents_.push_back("StateMachine");
}

// returns state machines name
string SM::getName() const
{
	return name_;
}

//destructor
SM::~SM()
{
	// 1. Empty the States_-vector
	// 2. Empty the eventsByStates-vector
	// 3. Empty the vector
  
	// variable used indexing vectors
	unsigned int idx = 0;
  
	// tmp-variable used to store temporary Event-objects.
	Event *tmp = 0;
  
	// 1. Empty the States_-vector
  
	while(idx < states_.size())
    {
		if(states_[idx] != 0)
		{
			delete states_[idx];
			states_[idx] = 0;
		}
		idx++;
    }
  
	idx = 0;
  
  // 2. Empty the eventsByStates_-vector
  
	while(idx < eventsByStates_.size())
    {
		tmp = eventsByStates_[idx].events; // first of the list.
      
		while( eventsByStates_[idx].events != 0)
		{
			tmp = tmp->getNextEvent();
			delete eventsByStates_[idx].events;
			eventsByStates_[idx].events = tmp;
		}
		tmp = 0;
		eventsByStates_[idx].events = 0;
		idx++;
    }
  
	idx = 0;
	tmp = 0;
  
	// 3. empty the ifaces_ -vector
  
	while(idx < ifaces_.size())
    {
		tmp = ifaces_[idx].events; // first of the list.
		while( ifaces_[idx].events != 0)
		{
			tmp = tmp->getNextEvent();
			delete ifaces_[idx].events;
			ifaces_[idx].events = tmp;
		}
		tmp = 0;
		ifaces_[idx].events = 0;
		idx++;
    }
}

// SM::addState(state) : adds new state to StateMachine
void SM::addState(State *s)
{
	states_.push_back(s);
	stateInfo tmp;
	tmp.name = s->getName();
	tmp.events = 0;
	eventsByStates_.push_back(tmp);
}

// SM::addParent(par) : adds inheritance parent to this class 
void SM::addParent(const string &par)
{
	parents_.push_back(par);
}

// SM::addIface(from, name) : adds new possible interface to get Events from
void SM::addIface(const string &from, const string &name)
{
	ifaceInfo tmp;
	tmp.from = from;
	tmp.name = name;
	tmp.events = 0;
	ifaces_.push_back(tmp);
}

// SM::addEvent(state, iface, event) : adds event to StateMachine
// returns true if adding succeeded otherwise returns false
bool SM::addEvent(const string &state, const string &iface, Event* e)
{
	// NOTE1  Each interface can have only one similar event!
	// NOTE2  EventID depend on interface and Event not in which state the SN is!
  
  // 1. CheckIfInState() -> true if that event already exists in that state
  //       FALSE -> CheckIfInGlobal() -> returns eventID if
  //                                     interface has this event allready, (-1 if not)
  //             -1-> 1. GetFreeEventID() -> returns unused EventID
  //                  2. AddEventGlobal() -> adds event to right interface.
  //                                         actually adds a copy of event to
  //                                         global event list
  //                  3. AddEventState() -> adds event to right state
  //             EventID-> 1. AddEventState()
  //       TRUE -> return false -> duplicate event in state : is illegal
  

	if(!checkIfInState(state, e))
    {
		int eventID = checkIfInGlobal(iface, e);
		if(eventID == -1)
		{
			e->setEventID(getFreeEventID());
			addEventGlobal(iface, e);
			addEventState(state, e);
		}
		else
		{
			e->setEventID(eventID);
			addEventState(state, e);
		}
    }
	else
    {
		return false;
    }
	return true;
}

// SM:: :
// SM::checkIfInState(state, Event : ->checks if Event allready exits
// in that state -> returns true if exists

bool SM::checkIfInState(const string &state, Event *e)
{
	unsigned int idx = 0;
  
	Event *tmp = 0;
	bool exists = false;
  
	while( idx < eventsByStates_.size())
    {
		if(eventsByStates_[idx].name == state)
		{
			tmp = eventsByStates_[idx].events;
	  
			while(tmp)
			{
	      
				// Events "primary-key" is pair of name and interface
				if( (tmp->getName() == e->getName()) && (tmp->getIface() == e->getIface()))
				{
					// there already is event e in this state
					exists = true;
					break;
				}
				else
				{
					tmp = tmp->getNextEvent();
				}
			}
	  
			break; // no sense to loop trought rest of the states.
		}
		else
		{
			idx++;
		}
    }
  
	if(exists)
    {
		return true;
    }
	else
    {
		return false;
    }
}

// SM::checkIfinGlobal : checks if the Event exists in globak event list
// returns -1 if doesn't exist (-1 isn't valid EventID)
// otherwise returns the EventID of Event found
int SM::checkIfInGlobal(const string &iface, Event *e)
{
	unsigned int idx = 0;
	Event *tmp = 0;
  
	int eventID = -1;
  
	//if( cursor != ifaces_.end())
	while(idx < ifaces_.size())
    {
		if( ifaces_[idx].name == iface)
		{
			// interface exists
	  
			// Loop trought the list of events in this interface and
			// if e is found return the corresponding eventID otherwise return -1
	  
			tmp = ifaces_[idx].events;

			while( tmp != 0)
			{
				// Events "primary-key" is pair of name and interface
				if( (tmp->getName() == e->getName()) && (tmp->getIface() == e->getIface()))
				{
					// store the correct event id
					return tmp->getEventID();
				}
				tmp = tmp->getNextEvent();
			}
			break;
		}
		else
		{
			idx ++;
		}
    }
	return eventID;
}

// SM:: :
// SM::addEventGlobal(iface, event) : adds copy of event to global list.

void SM::addEventGlobal(const string &iface, Event *e)
{
	// Find the right interface and add this as a first event in that interface
  
	unsigned int idx = 0;

  // we must create a new Event (copy of e) and add it to global-list
  // instead of e
  
	Event *newEvent = new Event( e->getIface(), e->getName());
	newEvent->setEventID(e->getEventID());
  
  
	while(idx < ifaces_.size())
    {
		if( ifaces_[idx].name == iface )
		{
			newEvent->setNextEvent(ifaces_[idx].events);
			ifaces_[idx].events = newEvent;
			/*
			  // interface exists
			  if(ifaces_[idx].events != 0)
			  {
			  tmp = ifaces_[idx].events->getNextEvent(); // first of the list.
	      
			  ifaces_[idx].events = newEvent;
			  newEvent->setNextEvent(tmp);
			  }
			  else
			  {
			  ifaces_[idx].events = newEvent;
			  }
			*/
		}
		else
		{
			// we must never come here
		}
		idx++;
    }

	// now we have one unique event more
	events_++;
}

// SM::addEventState(state, event) : adds Event to state
void SM::addEventState(const string &state, Event *e)
{
	// add event as a first event in correct state
  
	unsigned int idx = 0;
	//Event *tmp = 0;
  
	while( idx < eventsByStates_.size())
    {
		if(eventsByStates_[idx].name == state)
		{
			// this is the correct state where to add event e
	  
			e->setNextEvent(eventsByStates_[idx].events);
			eventsByStates_[idx].events = e;
			/*
	    
			  if(eventsByStates_[idx].events != 0)
			  { 
			  tmp = eventsByStates_[idx].events->getNextEvent(); // second of the list.
	    
			  eventsByStates_[idx].events = e;
			  e->setNextEvent(tmp);
			  }
			  else
			  {
			  eventsByStates_[idx].events = e;
			  }
			*/
	  
			break; // no sense to loop trought rest of the states.
		}
		else
		{
			idx++;
		}
    }
}

//SM::getFreeEventIF() : returns the smallest free eventId
int SM::getFreeEventID() const
{
	// events_ is the last EventID in use, smallest possible is EventID+1
	// this also ensures that EventID 0 is never given to any event.
	return events_+1;
}

//SM::printHeader(header) : print the header file
void SM::printHeader(ostream &header)
{
	// This method print the header-file of StateMachine class
   
	// Print's declaration of Ovops specifig classes.
	header << TAB() << "class " << pTaskName_ << ";" << endl << endl;
   
	// class name
	header << TAB() << "class " << name_;
   
	// print the public inheritance parents.
	// If multiple parents, then type, name pairs are separated by comma (C++)
   
	unsigned int idx = 0;
	if(!parents_.empty())
    {
		header << " : ";
		while(idx < parents_.size())
		{
			header << "public " << parents_[idx];
			if(idx+1 < parents_.size())
			{ 
				header << " , ";
			}
			idx++;
		}
		idx = 0;
    }
	header << endl;
	header << TAB() << "{" << endl;
	ilevel++;
	header << TAB() << "public:" << endl << endl;
	ilevel++;
   
	if(states_.size() != 0)
    {
		header << TAB() << "enum State { " << states_[0]->getName() << " = 0";
      
		unsigned int idx = 1;
		while(idx < states_.size())
		{
			header << ", " << states_[idx]->getName();
			idx++;
		}
		header << ", other };" << endl << endl;
    }
   
	// print the constructor and destructor
   
	header << TAB() << name_ << "(void);" << endl;
	header << TAB() << "virtual ~" << name_ << "(void);" << endl << endl;
   
	header << TAB() << "std::string stateName(PTask *h);" << endl;
	header << TAB() << "bool transit (EventType t, Message *msg, PTask *host);" 
		   << endl << endl;
   
	ilevel--;
   
	header << TAB() << "protected:" << endl;
	header << TAB() << "private:" << endl << endl;

	ilevel++;
	header << TAB() << "int states;" << endl;
	header << TAB() << "int events;" << endl;
      
	header << TAB() << "bool (" << pTaskName_ << " :: *execute [" << events_ + 2
		   << "][" << states_.size() << "]) (Message *msg);" << endl;
   
	ilevel--;
	ilevel--;
   
	header << TAB() << "};" << endl;
}

//SM::printImpl(impl) : prints the impl file
void SM::printImpl(ostream &impl)
{
	// Prints the implementation of StateMachine to output stream impl

	unsigned int idx =0 ;
	Event *tmp =0;
  
	while(idx < eventsByStates_.size())
    {
		tmp = eventsByStates_[idx].events; // first of the list.
      
		while( tmp != 0)
		{
			tmp = tmp->getNextEvent();
		}
		idx++;
    }
  
	idx = 0;
	tmp = 0;

	while(idx < ifaces_.size())
    {
		tmp = ifaces_[idx].events; // first of the list.
      
		while( tmp != 0)
		{
			tmp = tmp->getNextEvent();
		}
		idx++;
    }
  
	// start from the constructor
   
	impl << TAB() << name_ << " :: " << name_ << "(void)" << endl;
	impl << TAB() << ": states (" << states_.size() << "), events(" 
		 << events_+2 << ")" << endl;
   
	impl << TAB() << "{" << endl;
	ilevel++;
   
   // Next we must print the execute-array.
   // it is 2D array with size of states*events
   
   // for each state:
   // 1. what events are there(EventIDs), what are defAction and sysAction
   // 2. loop trought the global eventlist.
   // printing the row:
   // 1. EventID=0 is always defAction
   // 2. if eventID(n) exists in state's eventIds print correspondig action
   // 3. if in 2. not exists print defAction
   // 4. largestEventID +1 is always system ID
  
	unsigned int i = 0;
	int n = 0;

	tmp = 0;
   
	bool eventFound = false;
   
   // for each state:
	while(i < states_.size())
    {
		// for each event
		while(n <= events_+1) // 0 and all used EventID's, must be looped trough. 
		{
			if( n == 0)
			{
				// print the default action
	      
				impl << TAB() << "execute [" << n << "][" << i << "] = &" 
					 << pTaskName_ << " :: " << states_[i]->getDefaultAction() << ";" 
					 << endl;    	       
			}
			else if( n == (events_+1))
			{
				// print the system action
	      
				impl << TAB() << "execute [" << n << "][" << i << "] = &" 
					 << pTaskName_ << " :: " << states_[i]->getSystemAction() << ";" 
					 << endl;    	       
			}
			else
			{
				// print the Event, which eventID is n
				// if in state states_[i] is no event with eventID == n -> print default
				// else print events action...
	      
				tmp = eventsByStates_[i].events;
	      
				while(tmp != 0)
				{
					// Events "primary-key" in this case is the eventID
					if( tmp->getEventID() == n)
					{
						// There is spesified action for this Event.
						impl << TAB() << "execute [" << n << "][" << i << "] = &" 
							 << pTaskName_ << " :: " << tmp->getAction() << ";" 
							 << endl;
		      
						eventFound = true;
		       
						break; // can't be two event's with same EventID in same State
					}
					tmp = tmp->getNextEvent();
				}
				if(eventFound == false)
				{
					// state didn't have event with eventID == n
		  
					impl << TAB() << "execute [" << n << "][" << i << "] = &" 
						 << pTaskName_ << " :: " << states_[i]->getDefaultAction() << ";" 
						 << endl;    	       
				}
				eventFound = false;
			}
			n++;
		}
		n=0;
		i++;
		impl << TAB() << endl;
    }

	ilevel--;
	impl << TAB() << "}" << endl << endl;
  
	// destructor
  
	impl << TAB() << name_ << " :: ~" << name_ << "(void) {" << endl; 
	impl << TAB() << endl << "}" << endl << endl;
  
  // Transit-function
  
	impl << TAB() << "bool " << name_ << " :: transit (EventType t, Message *msg, PTask *h) {" << endl;
  
	ilevel++;
	impl << TAB() << endl;
	impl << TAB() << "int msgId = 0;" << endl << endl;
  
	impl << TAB() << "if (t == SystemEvent) {" << endl << endl;
  
	ilevel++;
  
	impl << TAB() << "msgId = " << events_+1 << ";" << endl << endl;
	ilevel--;
	impl << TAB() << "}";
   
	// 1. Loop trought the interfaces.
	// 2. for each interface print else if-condition
	// 3. for each Event in each interface print switch-case-statement
	// 3. give each Event right msgID
  
	idx = 0;
  
	tmp = 0;
	i = 1;

	while(idx < ifaces_.size())
    {
		impl << " else" << endl << endl;
		impl << TAB() << "if (msg->getTarget () == &((" 
			 << pTaskName_ << " *) h)->"
			 << ifaces_[idx].name << ") {" << endl;
		ilevel++;
		impl << TAB() << "switch(msg->getType ()) {" << endl; 
		ilevel++;
      
		// Loop trought the list of events in this interface and make each a case
      
		tmp = ifaces_[idx].events; // first of the list.
		while( tmp != 0)
		{
			impl << TAB() << "case " << ifaces_[idx].from << " :: " 
				 << StrToLower(tmp->getName()) << " :" << endl; 
			ilevel++;
			impl << TAB() << "msgId = " << tmp->getEventID() << ";" << endl;
			impl << TAB() << "break;" << endl << endl;
			ilevel--;
	 
			tmp = tmp->getNextEvent();
		}
		ilevel--;
      
		impl << TAB() << "}" << endl << endl;
		ilevel--;
		impl << TAB() << "}";
		idx++;
    }
	impl << TAB() << endl;
	impl << TAB() << "return (((" << pTaskName_ 
		 << "*) h)->*execute [msgId][h->getState ()]) (msg);" << endl;
	ilevel--;
	impl << TAB() << "}" << endl << endl;
  
  
  // printing the stateName - method
  
	impl << TAB() << "std::string " << name_ << " :: stateName (PTask *h) {" 
		 << endl << endl;
	ilevel++;
  
	impl << TAB() << "const char *rc = 0;" << endl << endl;
   
	idx = 0;
   
	if( states_.size() != 0)
    {
		impl << TAB() << "switch (h->getState ()) {" << endl;
		ilevel++;
      
		while(idx < states_.size())
		{
			impl << TAB() << "case " << idx << " : " << endl;
			ilevel++;
			impl << TAB() << "rc = \"" << states_[idx]->getName() << "\";" << endl;
			impl << TAB() << "break;" << endl;
			ilevel--;
			idx++;
		}
		impl << TAB() << "default:" << endl;
		ilevel++;
		impl << TAB() << "rc = 0;" << endl;
		ilevel--;
		ilevel--;
		impl << TAB() << "}" << endl;
		impl << TAB() << "return rc;" << endl;
		ilevel--;
		impl << TAB() << "}" << endl;
      
    }
}
