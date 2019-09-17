//Editor-Info: -*- C++ -*-
//  Copyright 2000 
//  	Telecoms Lab, Tampere University of Technology.  All rights reserved.

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
//
//  Subject: DEMON/ManagerTask header file
//  Author:  Ilkka Karvinen, TTKK/DMI

/*
$Log: managertask.h,v $
Revision 1.4  2003/05/10 18:22:56  bilhanan
Obsolete references to OVOPS removed.

Revision 1.3  2002/07/23 14:46:57  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/05/24 08:04:42  ik
Development

Revision 1.1.1.1  2002/05/13 14:54:56  bilhanan
Restructured

Revision 1.2  2002/05/10 10:49:17  ik
Text UI.

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.9  2002/01/21 17:19:00  ik
Changes to remove CORBA::Any from demon.idl.

Revision 1.8  2002/01/14 17:37:50  ik
Changes to make two-direction CORBA calls to work.

Revision 1.7  2002/01/10 17:12:16  ik
DII-calls added.

Revision 1.6  2002/01/05 19:38:22  ik
SLP library called directly now.

Revision 1.5  2001/12/14 15:26:51  ik
Finding of SLP attributes (IOR).

Revision 1.4  2001/12/13 15:06:35  ik
Manager sends and receives SLP messages.

Revision 1.3  2001/12/12 13:19:41  ik
Log tag added.

*/

#ifndef MANAGERTASK_H
#define MANAGERTASK_H

#include <CORBA.h>
#include <vector>
#include <string>
#include <slp.h>

#include <doors/etask.h>
#include <doors/timer.h>
#include "demon.h"

/** Container class for an SLP datum
 */
class slpdatum {
public:
  slpdatum() : obj(0), attributes_set(false) { }
  slpdatum& operator=(const slpdatum& src) {
    url = src.url; ior = src.ior; obj = src.obj; 
    attributes_set = src.attributes_set;       
    return *this;
  }
  std::string url;
  std::string ior;
  CORBA::Object_ptr obj;
  bool attributes_set;
  
  // Agent-specific CORBA calls
  CORBA::Boolean setManager(CORBA::Long handle, const char* manager_ior);
  CORBA::Boolean start();
  CORBA::Boolean stop();
  
};

/** Manager task. This object follows Singleton Design Pattern.
    Singleton is used because SLP would not work if there were many
    instances of ManagerTask. No reason to have many instances of this
    in any case.
    EventTask and CORBA PortableServer::DynamicImplementation
    are inherited to the class and the object implements both of them.
 */
class ManagerTask : public EventTask,
		    public PortableServer::DynamicImplementation
{
 public:
  // Object instance
  static ManagerTask* Instance();

  // CORBA standards
  virtual void invoke (CORBA::ServerRequest_ptr svreq);
  virtual char* _primary_interface(const PortableServer::ObjectId&, 
                                   PortableServer::POA_ptr); 

  // outgoing
  //  CORBA::Boolean setManager(const char* manager_ior);
  //  CORBA::Boolean start();
  //  CORBA::Boolean stop();
  CORBA::Boolean dui_setAgent(CORBA::Long index, const char* agent_url);
  CORBA::Boolean dui_unsetAgent(CORBA::Long index);
  CORBA::Boolean dui_setManager(const char* manager_ior);
  CORBA::Boolean dui_sendSymbol (CORBA::Long agent_handle, 
				 Demon::TimeStamp time, 
				 Demon::Symbol symbol);

  // incoming
  CORBA::Boolean setSystem (Demon::Symbol system);
  CORBA::Boolean sendSymbol (CORBA::Long handle,
			     Demon::TimeStamp time, Demon::Symbol symbol);
  CORBA::Boolean dui_start(CORBA::Short verbose);
  CORBA::Boolean dui_stop();

  // EventTask executor
  virtual bool execute (EventType type, Message *msg);

  bool isTerminated() { return terminated_; }

  // Callback functions inside the class
  static
  SLPBoolean SLPSrvCallback(SLPHandle handle, const char* srvtypes, 
			    unsigned short lifetime, SLPError error, 
			    void* cookie);
  static
  SLPBoolean SLPAttCallback(SLPHandle handle, const char* attrlist, 
			    SLPError error, void* cookie);

  static ::CORBA::ORB_ptr orb_;             // The ORB

protected:
  ManagerTask ();
  virtual ~ManagerTask(void);

private:
  class demon_message {
  public:
    demon_message() { } // Needed by SunCC for some reason that beats me
    demon_message(CORBA::Long h, Demon::TimeStamp t, Demon::Symbol s)
      : handle(h), time(t), symbol(s) { }
    const Demon::TimeStamp& get_time() { return time; }
    const Demon::Symbol& get_symbol() { return symbol; }
    const CORBA::Long get_handle() { return handle; }
  private:
    CORBA::Long handle;
    Demon::TimeStamp time;
    Demon::Symbol symbol;
  };
  std::vector<class demon_message> messages_;

  static vector<struct slpdatum> slpdata_;  // Vector for SLP data
  static bool pending_;                     // Pending for SLP attributes
  bool terminated_;                         // Task termination flag
  ::PortableServer::POA_ptr poa_;           // The POA
  CORBA::Object_ptr agent_obj_;             // Agent objects 
  CORBA::Object_ptr ui_obj_;                // UI object
  static std::string ior_;                         // Agent IOR
  Timer timer_;                             // Timer for execute()
  bool sending_;                            
  SLPHandle slp_;                           // Handle to SLP functions
  bool initial_;                            // Initial call to execute()?
  static ManagerTask* instance_;            // Pointer to ManagerTask instance
  int verbose_;                             // UI verbose value
  bool dui_flow_;                           // UI receiving symbols?
  bool message_pending_;                    // Message pending for UI

}; 

#endif // MANAGERTASK_H
