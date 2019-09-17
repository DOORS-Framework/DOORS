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
//  Subject: DEMON/AgentTask header
//  Author:  Ilkka Karvinen, TTKK/DMI

/*
$Log: agenttask.h,v $
Revision 1.10  2004/01/19 10:44:09  bilhanan
#include fixed.

Revision 1.9  2002/11/22 17:18:52  ik
Support for setFilter() message.

Revision 1.8  2002/11/19 16:21:28  ik
Added event ID parameter to sendSymbol().

Revision 1.7  2002/11/11 16:28:44  ik
disconnect() message.

Revision 1.6  2002/10/30 17:34:24  ik
Bug fixes.

Revision 1.5  2002/09/26 11:34:17  ik
Fix to reduce CPU usage on Agent's thread. Bug fix: the newly sent
message was not removed from messages_ buffer.

Revision 1.4  2002/09/25 14:32:41  ik
Thread commands to AgentTask.

Revision 1.3  2002/07/23 14:46:56  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/05/24 08:04:42  ik
Development

Revision 1.1.1.1  2002/05/13 14:54:40  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.8  2002/02/25 17:06:04  ik
sockaddr.* split to inetaddr.* and unixaddr.* and the depending
files changed accordingly.

Revision 1.7  2002/01/21 17:18:59  ik
Changes to remove CORBA::Any from demon.idl.

Revision 1.6  2002/01/14 17:37:50  ik
Changes to make two-direction CORBA calls to work.

Revision 1.5  2002/01/05 19:38:22  ik
SLP library called directly now.

Revision 1.4  2001/12/12 13:49:56  ik
Demon modified to use Scheduler::Instance().

Revision 1.3  2001/12/12 13:19:40  ik
Log tag added.

*/

#ifndef AGENTTASK_H
#define AGENTTASK_H

#include <string>
#include <deque>
#include <vector>
#include <pthread.h>
#include <slp.h>
#include <doors/etask.h>
#include <doors/timer.h>
#include <doors/demon.h>
#include <doors/demonhand.h>

/** Agent task
 */
class AgentTask : public PortableServer::DynamicImplementation
{
 public:
  // The instance
  static AgentTask* Instance();
  static void Create();

  // CORBA standards
  virtual void invoke (CORBA::ServerRequest_ptr svreq);
  virtual char* _primary_interface(const PortableServer::ObjectId&, 
                                   PortableServer::POA_ptr); 

  // outgoing
  CORBA::Boolean setSystem (Demon::Symbol system);
  CORBA::Boolean sendSymbol (Demon::TimeStamp time, Demon::Symbol symbol);
  CORBA::Boolean disconnect();

  // incoming
  CORBA::Boolean setFilter(Demon::FilterList& filterlist);
  CORBA::Boolean setManager(CORBA::Long handle, const char* manager_ior);
  CORBA::Boolean start();
  CORBA::Boolean stop();
  CORBA::Boolean disconnect_in();

  // EventTask executor
  virtual void execute ();
  
protected:
  AgentTask (const char* n);
  virtual ~AgentTask(void);

private:
  class demon_message {
  public:
    demon_message() { } // Needed by SunCC for some reason that beats me
    demon_message(Demon::TimeStamp t, Demon::Symbol s)
      : time(t), symbol(s) { }
    const Demon::TimeStamp& get_time() { return time; }
    const Demon::Symbol& get_symbol() { return symbol; }
  private:
    Demon::TimeStamp time;
    Demon::Symbol symbol;
  };
  std::deque<class demon_message> messages_;
  static void SLPRegCallback(SLPHandle, SLPError error, void* cookie);

  void InsertTimestamps(Frame& frame);
  void RemoveTimestamps(Frame& frame);

  static std::vector<unsigned long> tsvector_;
  bool terminated_;
  ::PortableServer::POA_ptr poa_;
  ::CORBA::ORB_ptr orb_;
  CORBA::Object_ptr manager_obj_;
  Timer timer_;
  bool sending_;
  std::string slp_name_;
  std::string ior_;
  SLPHandle slp_;
  bool manager_set_;                        // Manager has sent its IOR
  CORBA::Long handle_;
  DemonSystemHandler* system_;
  static AgentTask* instance_;
  pthread_mutex_t buffer_lock_;
  pthread_mutex_t terminated_lock_;
  pthread_t thread_;
  CORBA::ULong event_id_;
}; 

#endif // AGENTTASK_H
