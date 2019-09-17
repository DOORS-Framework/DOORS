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

$Log: port.C,v $
Revision 1.3  2002/08/14 09:31:41  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/07/23 14:25:22  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:25  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:21  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/port.h>

#include <doors/debug.h>

Port :: Port (EventTask *host) 
    : other (0), task (host)
{
  _MT_FORWARDLOCK_ASMONITOR(mon, task);
}

Port :: ~Port (void)
{
  _MT_FORWARDLOCK_ASMONITOR(mon, task);
  disconnect ();
}

Port *Port :: getOther (void) 
{ 
  _MT_FORWARDLOCK_ASMONITOR(mon, task);
  return other; 
}

EventTask *Port :: getTask (void) 
{
  _MT_FORWARDLOCK_ASMONITOR(mon, task); 
  return task; 
}

bool Port :: connect (Port *to)
{
    _MT_FORWARDLOCK_ASMONITOR(mon, task);
  
    bool ok = false;
    _DOORS_ERROR(to,"no port to connect to");
    Port *o = other;
    
    _MT_RELEASE_FL_MONITOR(mon);
    if (o == 0 && to->connectRequest (this) == true) {
	_MT_ACQUIRE_FL_MONITOR(mon);
	if (task == 0 || task->connect (this) == true) {
	    other = to;
	    ok = true;
	} else {
	    _MT_RELEASE_FL_MONITOR(mon);
	    to->disconnectRequest (this);
	    _MT_ACQUIRE_FL_MONITOR(mon);
	}
    }
    return ok;
}

bool Port :: connectRequest (Port *to)
{
  _MT_FORWARDLOCK_ASMONITOR(mon, task);

  bool rc = false;
  if (other == 0 && (task == 0 || task->connect (this) == true)) {
    other = to;
    rc = true;
  }
  return rc;
}

bool Port :: connected (void)
{
  _MT_FORWARDLOCK_ASMONITOR(mon, task);

  return (other) ? true : false;
}

bool Port :: disconnect (void)
{
  _MT_FORWARDLOCK_ASMONITOR(mon1, task);
  _MT_RELEASE_FL_MONITOR(mon1);
  _MT_FORWARDLOCK_ASMONITOR(mon2, (other ? other->getTask():0));
  _MT_ACQUIRE_FL_MONITOR(mon1);

  bool status = false;

  if (other) {
    if (task) {
      task->disconnect (this);
    }
    status = other->disconnectRequest (this);
    other = 0;
  }
  return status;
}

bool Port :: disconnectRequest (Port *)
{
  _MT_FORWARDLOCK_ASMONITOR(mon, task);

  // The parameter 'from' is useless in this function but it
  // must exists because the some inherited classes (MultiPort) 
  // may need it. Note: CC doesn't accept the Port *. 
  
  if (task) {
    task->disconnect (this);
  }
  other = 0;

  return true;
}

bool Port :: compareOther (Port *a, Port *b)
{
  _MT_FORWARDLOCK_ASMONITOR(mon, a->getTask());

  return (a->other == b) ? true : false;
}

bool Port :: getMessage (Message *message) 
{
    _MT_FORWARDLOCK_ASMONITOR(mon, task);
    _DOORS_ERROR(message,"no message");
    bool rc = false;
    if (task) {
	rc = task->save (message);
    } 
    return rc;
}

bool Port :: putMessage (Message *message) 
{
    _MT_FORWARDLOCK_ASMONITOR(mon1, (other ? (other->getTask()) : 0));
    _MT_FORWARDLOCK_ASMONITOR(mon2, task);
    _DOORS_ERROR(message,"no message");
    bool rc = false;
    message->mark (this, other);
    if (other) {
	rc = other->getMessage (message);
    }
    return rc;
}

bool SystemPort :: getMessage (Message *message) 
{
    _MT_FORWARDLOCK_ASMONITOR(mon, task);
    _DOORS_ERROR(message,"no message");
    bool rc = false; 
    if (task) {
	rc = task->save (message, SystemEvent);
    } 
    return rc;
}














