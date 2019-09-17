//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
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
//  Subject: DEMON AgentTask
//  Author:  Ilkka Karvinen, TTKK/DMI

/*
$Log: agenttask.C,v $
Revision 1.13  2003/06/04 13:00:57  ik
Syntax fix.

Revision 1.12  2002/12/04 16:16:41  ik
Development.

Revision 1.11  2002/11/22 17:18:52  ik
Support for setFilter() message.

Revision 1.10  2002/11/19 16:21:28  ik
Added event ID parameter to sendSymbol().

Revision 1.9  2002/11/11 16:28:44  ik
disconnect() message.

Revision 1.8  2002/10/30 17:34:24  ik
Bug fixes.

Revision 1.7  2002/09/26 11:34:17  ik
Fix to reduce CPU usage on Agent's thread. Bug fix: the newly sent
message was not removed from messages_ buffer.

Revision 1.5  2002/07/23 14:46:56  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.4  2002/05/24 10:23:17  ik
Removed debug outputs.

Revision 1.3  2002/05/24 09:45:37  ik
SLP fixes.

Revision 1.2  2002/05/24 08:04:41  ik
Development

Revision 1.1.1.1  2002/05/13 14:54:40  bilhanan
Restructured

Revision 1.2  2002/05/10 10:49:17  ik
Text UI.

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.8  2002/02/25 17:06:07  ik
sockaddr.* split to inetaddr.* and unixaddr.* and the depending
files changed accordingly.

Revision 1.7  2002/01/21 17:19:01  ik
Changes to remove CORBA::Any from demon.idl.

Revision 1.6  2002/01/14 17:37:51  ik
Changes to make two-direction CORBA calls to work.

Revision 1.5  2002/01/10 17:12:18  ik
DII-calls added.

Revision 1.4  2002/01/05 19:37:55  ik
SLP library called directly now.

Revision 1.3  2001/12/12 13:49:57  ik
Demon modified to use Scheduler::Instance().

Revision 1.2  2001/12/12 13:15:39  ik
Licence to demon header files. Changes to make demon compile in DOORS.

Revision 1.1  2001/12/11 15:21:55  ik
Directory structure changed due to problems in autoconf.

*/

#include "demon.h"
#include "demonhand.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>

#include <doors/osched.h>
#include <doors/hsi.h>
#include <slp.h>
#include <doors/agenttask.h>

// TODO: remove this
#include <fstream>

// #define IOR_TO_FILE

std::vector<unsigned long> AgentTask::tsvector_;
AgentTask* AgentTask::instance_ = 0;
static void* AgentTask_Thread(void* pthis);

AgentTask*
AgentTask::Instance() 
{
  if (instance_ == 0) {
    instance_ = new AgentTask("agent");
  }
  return instance_;
}

void
AgentTask::Create() 
{
  if (instance_ == 0) {
    instance_ = new AgentTask("agent");
  }
}

void
AgentTask::InsertTimestamps(Frame& frame)
{
  for (int i = 0; i < tsvector_.size(); i++) {
    unsigned long ts = htonl(tsvector_[i]);
    frame.putFirst(ts>>24&0xff);
    frame.putFirst(ts>>16&0xff);
    frame.putFirst(ts>>8&0xff);
    frame.putFirst(ts&0xFF);
  }
}

void
AgentTask::RemoveTimestamps(Frame& frame)
{
  for (int i = 0; i < tsvector_.size(); i++) {
    unsigned long tmp = 0;
    tmp  = (unsigned long)frame.getFirst()<<24&0xff000000;
    tmp |= (unsigned long)frame.getFirst()<<16&0x00ff0000;
    tmp |= (unsigned long)frame.getFirst()<<8 &0x0000ff00;
    tmp |= (unsigned long)frame.getFirst()    &0x000000ff;
    tsvector_[i] = ntohl(tmp);
  }
}

AgentTask::AgentTask(const char* n)
  : terminated_(false), orb_(0), poa_(0),
    manager_obj_(0), sending_(false), 
    slp_(0), manager_set_(false), handle_(0), event_id_(0)
{
  char buf[64];
  char hostname[64];
  if (!gethostname(hostname, 64)) {
    sprintf(buf, "service:demon://%s/%d", hostname, (int)getpid());
  } else {
    sprintf(buf, "service:demon://unknown/%d", (int)getpid());
  }
  slp_name_ = buf;

  std::cout << "Agent ID: " << slp_name_ << std::endl;

  // Create ORB and BOA
  char* argvector[] = { (char*)"agent_server", 0 };
  int argcount = 1;
  orb_ = ::CORBA::ORB_init(argcount, argvector, "mico-local-orb");
  ::CORBA::Object_ptr poaobj = orb_->resolve_initial_references("RootPOA");
  poa_ = PortableServer::POA::_narrow(poaobj);

  // Activate this object to receive the invokes
  PortableServer::ObjectId_var oid = poa_->activate_object (this);

  // Activate POA
  poa_->the_POAManager()->activate();

  // TODO: Create handlers for these:
  //       HandlerCreator::createHandler(CORBA::ORB*, Symbol*)
  //       HandlerCreator::createHandler(PortableServer::POA*, Symbol*)
  //  DYNAMIC_SYMBOL(orb_, "ORB", this);
  //  DYNAMIC_SYMBOL(poa_, "POA", this);

  CORBA::Object_ptr ref = poa_->id_to_reference (oid.in());
  ior_ = orb_->object_to_string (ref);

  // Create manager for Demon::System
  system_ = new DemonSystemHandler(symbolTable.getStaticTree (), "system");

  // Initialise the locks
  pthread_mutex_init(&buffer_lock_, NULL);
  pthread_mutex_init(&terminated_lock_, NULL);

  // Launch our worker thread
  if (pthread_create(&thread_, NULL, AgentTask_Thread, (void*)this)) {
    std::cout << "Thread failed." << std::endl;
  }
}

AgentTask::~AgentTask(void) 
{
  DELETE_SYMBOL (this);

  delete system_;

  poa_->destroy(TRUE, TRUE);

#ifndef IOR_TO_FILE
  // Deregister from SLP
  SLPError error;
  (void)SLPDereg(slp_, slp_name_.c_str(), &AgentTask::SLPRegCallback, &error);

  // Close SLP
  if (slp_) {
    SLPClose(slp_);
    slp_ = 0;
  }
#endif
  
  // Signal the thread to terminate
  pthread_mutex_lock(&terminated_lock_);
  terminated_ = true;
  pthread_mutex_unlock(&terminated_lock_);

  pthread_join(thread_, 0);

  // Destroy message buffer lock
  pthread_mutex_destroy(&terminated_lock_);
  pthread_mutex_destroy(&buffer_lock_);
}

CORBA::Boolean
AgentTask::setSystem (Demon::Symbol system)
{
  CORBA::Boolean b = FALSE;
  if (!CORBA::is_nil(manager_obj_)) {
    CORBA::Any any;
    CORBA::Request_var req = manager_obj_->_request ("setSystem");
    req->add_in_arg() <<= (Demon::Symbol)system;
    req->result()->value()->set_type (CORBA::_tc_boolean);
    req->invoke();
    if (req->env()->exception())
        mico_throw (*req->env()->exception());
    *req->result()->value() >>= CORBA::Any::to_boolean(b);
    std::cout << "AgentTask::setSystem invoked." << std::endl;

    // Demo:
    sending_ = true;
  }
  return b;
}

CORBA::Boolean
AgentTask::sendSymbol (Demon::TimeStamp time, 
		       Demon::Symbol symbol)
{
  CORBA::Boolean b = FALSE;
  // Put message to the end of deque to be sent later inside execute()
  pthread_mutex_lock(&buffer_lock_);
  messages_.push_back(demon_message(time, symbol));
  pthread_mutex_unlock(&buffer_lock_); 
 return b;
}

CORBA::Boolean 
AgentTask::disconnect()
{
  CORBA::Boolean b = FALSE;
  if (!CORBA::is_nil(manager_obj_)) {
    CORBA::Any any;
    CORBA::Request_var req = manager_obj_->_request ("disconnect");
    req->result()->value()->set_type (CORBA::_tc_boolean);
    req->invoke();
    if (req->env()->exception())
        mico_throw (*req->env()->exception());
    *req->result()->value() >>= CORBA::Any::to_boolean(b);
    std::cout << "disconnect invoked." << std::endl;
  }
  return b;
}

void 
AgentTask::SLPRegCallback(SLPHandle slp, SLPError error, void* cookie)
{
  *(SLPError*)cookie = error; // return error code to the caller
}

void* 
AgentTask_Thread(void* pthis)
{
  if (pthis) {
    ((AgentTask*)pthis)->execute ();
  }

  pthread_exit(0);

  return 0;
}

// EventTask execute() rewritten to run one task in the ORB if needed.
void
AgentTask::execute ()
{
  // TODO: start using SLP, during testing IOR is read from a file
#ifndef IOR_TO_FILE
  // Open SLP
  SLPError slperr = SLPOpen("default", SLP_FALSE/*=sync*/, &slp_);
  if (slperr == SLP_OK) {
    SLPError error;
    slperr = SLPReg(slp_, slp_name_.c_str(), 65535, "default", ior_.c_str(),
		    SLP_TRUE, &AgentTask::SLPRegCallback, &error);
    if ((slperr != SLP_OK) || (error != SLP_OK)) {
      std::cerr << "Could not register to SLP" << std::endl;
    } else {
      std::cerr << "Registered to SLP" << std::endl;
    }
  }
#else
  std::ofstream file("/tmp/agent.ref");
  file << slp_name_ << std::endl;
  file << ior_ << std::endl;
  file.close();
#endif

  // Endless loop for sending buffered symbols to Manager and
  // checking for Mico ORB for incoming messages.
  for ( ;; ) {

    // Take a short nap or this process will use too much CPU
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 5000;
    select(0, NULL, NULL, NULL, &tv);

    pthread_mutex_lock(&terminated_lock_);
    if (terminated_) {
      pthread_mutex_unlock(&terminated_lock_);
      break;
    }
    pthread_mutex_unlock(&terminated_lock_);

    // Executed once during the first call
    // Send our system to the manager
    if (manager_set_) {
      std::cout << "system_->update()" << std::endl;
      system_->update();
      manager_set_ = false; // Should be called only once per manager
    }
    
    // Send symbols to Manager if sending_ flag is set and there are
    // messages in messages_ buffer. The buffer is protected with
    // buffer_lock_ because sendSymbol() may be called by another
    // thread. Sends only one message from the front of the buffer.
    pthread_mutex_lock(&buffer_lock_);
    int len = messages_.size();
    pthread_mutex_unlock(&buffer_lock_);
    if ((sending_) && (len > 0) && (!CORBA::is_nil(manager_obj_))) {
      for (int i = 0; i < len; i++) {
	//      sendSymbol(messages_[i].get_time(), messages_[i].get_symbol());
	CORBA::Request_var req = manager_obj_->_request ("sendSymbol");
	req->add_in_arg() <<= (CORBA::ULong)++event_id_;
	req->add_in_arg() <<= handle_;
	pthread_mutex_lock(&buffer_lock_);
	req->add_in_arg() <<= messages_.front().get_time();
	req->add_in_arg() <<= messages_.front().get_symbol();
	messages_.pop_front();
	pthread_mutex_unlock(&buffer_lock_);
	req->result()->value()->set_type (CORBA::_tc_boolean);
	req->invoke();
      }
    }

    // Refresh the ORB if needed
    // ORB calls AgentTask::invoke()'s if there are incoming CORBA messages
    if (orb_->work_pending()) { 
      orb_->perform_work(); 
    }
  } // for ( ;; )

}

// CORBA standards

void
AgentTask::invoke (CORBA::ServerRequest_ptr svreq)
{
  if (std::string(svreq->op_name()) == std::string("setManager")) {
    // create argument list
    CORBA::NVList_ptr args;
    orb_->create_list (0, args);
    args->add(CORBA::ARG_IN);
    args->item(0)->value()->set_type(CORBA::_tc_long);
    args->add(CORBA::ARG_IN);
    args->item(1)->value()->set_type(CORBA::_tc_string);

    svreq->params(args);

    CORBA::Long handle;
    *args->item(0)->value() >>= handle;
    const char* manager_ior;
    *args->item(1)->value() >>= CORBA::Any::to_string(manager_ior, 0);
    std::cout << "manager ior = " << manager_ior << std::endl;
    CORBA::Boolean b = setManager(handle, manager_ior);

    CORBA::Any *res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean(b);
    svreq->result(res);
  } else if (std::string(svreq->op_name()) == std::string("setFilter")) {
    CORBA::NVList_ptr args;
    orb_->create_list (0, args);
    args->add(CORBA::ARG_IN);
    args->item(0)->value()->set_type(Demon::_tc_FilterList);

    svreq->params(args);
    
    Demon::FilterList filterlist;
    *args->item(0)->value() >>= filterlist;
    CORBA::Boolean b = setFilter(filterlist);
    CORBA::Any *res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean(b);
    svreq->result(res);
  } else if (std::string(svreq->op_name()) == std::string("start")) {
    CORBA::Boolean b = stop();
    CORBA::Any *res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean(b);
    svreq->result(res);
  } else if (std::string(svreq->op_name()) == std::string("stop")) {
    CORBA::Boolean b = start();
    CORBA::Any *res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean(b);
    svreq->result(res);
  } else if (std::string(svreq->op_name()) == std::string("disconnect")) {
    CORBA::Boolean b = disconnect_in();
    CORBA::Any *res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean(b);
    svreq->result(res);
  }
}

char* 
AgentTask::_primary_interface(const PortableServer::ObjectId&, 
                              PortableServer::POA_ptr) 
{ 
  return CORBA::string_dup("IDL:Demon/Agent:1.0"); 
} 


CORBA::Boolean
AgentTask::setFilter(Demon::FilterList& filterlist)
{

  int len = filterlist.length();
  std::cout << "AgentTask::setFilter: " << len << std::endl;
  if ((system_) && (len > 0)) {
    for (int i = 0; i < len; i++) {
      Demon::Filter& filter = filterlist[i];
#if 0
      if (filter.type == Demon::stSystem) {
	// Set enable/disable on the whole system
	system_->setEnabledRecursive(filter.enabled);
	continue; // Advance to the next filter
      } else {
	system_->setFilter(filter);
      }
#endif
      system_->setFilter(filter);
    }
  }
 
  return TRUE;
}

CORBA::Boolean
AgentTask::setManager(CORBA::Long handle, const char* manager_ior)
{
  manager_obj_ = orb_->string_to_object(manager_ior);
  if (CORBA::is_nil(manager_obj_)) {
    std::cerr << "Cannot bind to manager." << std::endl;
  }

  std::cout << "setManager called." << std::endl;

  handle_ = handle;
  manager_set_ = true;

  return TRUE;
}

CORBA::Boolean
AgentTask::start()
{
  sending_ = true;

  return TRUE;
}

CORBA::Boolean
AgentTask::stop()
{
  sending_ = false;

  return TRUE;
}

CORBA::Boolean
AgentTask::disconnect_in()
{
  handle_ = 0;
  manager_set_ = false;
  manager_obj_ = 0;

  std::cout << "disconnected." << std::endl;

  return TRUE;
}

