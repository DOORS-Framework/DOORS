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
//  Subject: DEMON Text User Interface
//  Author:  Ilkka Karvinen, TTKK/DMI

/*
$Log: textuitask.C,v $
Revision 1.5  2002/07/23 14:46:57  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.4  2002/05/24 09:57:54  ik
EventTask print handler.

Revision 1.3  2002/05/24 09:45:38  ik
SLP fixes.

Revision 1.2  2002/05/24 08:04:42  ik
Development

Revision 1.1.1.1  2002/05/13 14:54:56  bilhanan
Restructured

Revision 1.1  2002/05/10 10:45:35  ik
Text UI first commit.

*/

#include "demon.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include <unistd.h>

#include <doors/osched.h>
#include <doors/hsi.h>
#include <doors/textuitask.h>

// TODO: remove this
#include <fstream>

#define IOR_TO_FILE

// I would rather put this to private members of TextUITask if it worked
// there. Someone tell me why it doesn't work with SunCC compiler.
static std::map<CORBA::Long,std::string> agent_urls_;

TextUITask::TextUITask(const char* n, short verbose)
  : EventTask (n, Scheduler::Instance()), terminated_(false), orb_(0), poa_(0),
    manager_obj_(0), timer_(this, OTime(0, 1)),
    initial_(true), manager_set_(false), verbose_(verbose), id_(0)
{
  // Create ORB and POA
  char* argvector[] = { (char*)"textui_server", 0 };
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

  // Start timer
  timer_.start();

}

TextUITask::~TextUITask(void) 
{
  DELETE_SYMBOL (this);

  poa_->destroy(TRUE, TRUE);

}

CORBA::Boolean
TextUITask::dui_sendSymbol (CORBA::Long agent_handle, 
			    Demon::TimeStamp time, 
			    Demon::Symbol symbol)
{
  char strbuf[256];

  const char* agent_str = 0;
  if (agent_handle >= 0) {
    agent_str = agent_urls_[agent_handle].c_str();
  }

  sprintf(strbuf, "|%5d |%13d.%4d |%26s     |", ++id_, 
	          time.seconds, time.useconds,
	                                 agent_str);
  std::cout << "+-ID---+--------TIME---------+------------AGENT--------------+";
  std::cout << std::endl; 
  std::cout << strbuf << std::endl;
  std::cout << "+------+---------------------+-------------------------------+";
  std::cout << std::endl; 

  printSymbol(symbol);

  return FALSE;
}

CORBA::Boolean
TextUITask::dui_setAgent(CORBA::Long handle, const char* agent_url)
{
  std::cout << "dui_setAgent called with " << handle << " == ";
  std::cout << agent_url << std::endl;
  if ((handle >= 0) && (agent_url)) {
    agent_urls_[handle] = agent_url;
    std::cout << "saved = " << agent_urls_[handle] << std::endl;
    return FALSE;
  }

  return TRUE;
}

CORBA::Boolean
TextUITask::dui_unsetAgent(CORBA::Long handle)
{
  if (handle >= 0) {
    agent_urls_.erase(handle);
    return FALSE;
  }

  return TRUE;
}

CORBA::Boolean
TextUITask::dui_setManager(const char* manager_ior)
{
  manager_obj_ = orb_->string_to_object(manager_ior);
  if (CORBA::is_nil(manager_obj_)) {
    std::cerr << "Cannot bind to manager." << std::endl;
  }

  std::cout << "dui_setManager called." << std::endl;

  manager_set_ = true;
  id_ = 0;

  return TRUE;
}

// EventTask execute() rewritten to run one task in the ORB if needed.
bool 
TextUITask::execute (EventType type, Message *msg)
{
  // Executed once during the first call
  if (initial_) {
#ifdef IOR_TO_FILE
    // TODO: start using Naming, during testing IOR is written to a file
    std::ofstream file("/tmp/ui.ref");
    file << ior_;
    file.close();
#endif
    initial_ = false;
  }

  // Send start message to the manager
  if (manager_set_) {
    dui_start(verbose_);
    manager_set_ = false; // Should be called only once per manager
  }

  // We should only get system events (timer)
  if (type == SystemEvent) {
    // Refresh the ORB if needed
    if (orb_->work_pending()) { 
      orb_->perform_work(); 
    }
  }

  delete msg;

  // Restart the timer
  timer_.start();

  return true;
}

// CORBA standards

void
TextUITask::invoke (CORBA::ServerRequest_ptr svreq)
{
  if (std::string(svreq->op_name()) == std::string("dui_setManager")) {
    // create argument list
    CORBA::NVList_ptr args;
    orb_->create_list (0, args);
    args->add(CORBA::ARG_IN);
    args->item(0)->value()->set_type(CORBA::_tc_string);

    svreq->params(args);
    
    const char* manager_ior;
    *args->item(0)->value() >>= CORBA::Any::to_string(manager_ior, 0);
    std::cout << "manager ior = " << manager_ior << std::endl;
    CORBA::Boolean b = dui_setManager(manager_ior);

    CORBA::Any *res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean(b);
    svreq->result(res);
  } else if (std::string(svreq->op_name()) == std::string("dui_sendSymbol")) {
    // create argument list
    CORBA::NVList_ptr args;
    orb_->create_list (0, args);
    args->add(CORBA::ARG_IN);
    args->item(0)->value()->set_type(CORBA::_tc_long);
    args->add(CORBA::ARG_IN);
    args->item(1)->value()->set_type(Demon::_tc_TimeStamp);
    args->add(CORBA::ARG_IN);
    args->item(2)->value()->set_type(Demon::_tc_Symbol);

    svreq->params(args);
    
    CORBA::Long handle;
    *args->item(0)->value() >>= handle;
    Demon::TimeStamp timestamp;
    *args->item(1)->value() >>= timestamp;
    Demon::Symbol symbol;
    *args->item(2)->value() >>= symbol;
    CORBA::Boolean b = dui_sendSymbol(handle, timestamp, symbol);
    CORBA::Any *res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean(b);
    svreq->result(res);
  } else if (std::string(svreq->op_name()) == std::string("dui_setAgent")) {
    // create argument list
    CORBA::NVList_ptr args;
    orb_->create_list (0, args);
    args->add(CORBA::ARG_IN);
    args->item(0)->value()->set_type(CORBA::_tc_long);
    args->add(CORBA::ARG_IN);
    args->item(1)->value()->set_type(CORBA::_tc_string);

    svreq->params(args);

    CORBA::Long index;
    *args->item(0)->value() >>= index;
    const char* agent_url;
    *args->item(1)->value() >>= CORBA::Any::to_string(agent_url, 0);
    CORBA::Boolean b = dui_setAgent(index, agent_url);
    CORBA::Any *res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean(b);
    svreq->result(res);
  } else if (std::string(svreq->op_name()) == std::string("dui_unsetAgent")) {
    // create argument list
    CORBA::NVList_ptr args;
    orb_->create_list (0, args);
    args->add(CORBA::ARG_IN);
    args->item(0)->value()->set_type(CORBA::_tc_long);

    svreq->params(args);

    CORBA::Long index;
    *args->item(0)->value() >>= index;
    CORBA::Boolean b = dui_unsetAgent(index);
    CORBA::Any *res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean(b);
    svreq->result(res);
  } 
}

char* 
TextUITask::_primary_interface(const PortableServer::ObjectId&, 
                              PortableServer::POA_ptr) 
{ 
  return CORBA::string_dup("IDL:Demon/UI:1.0"); 
} 


CORBA::Boolean
TextUITask::dui_start(CORBA::Short verbose)
{
  CORBA::Boolean b = FALSE;
  if (!CORBA::is_nil(manager_obj_)) {
    CORBA::Any any;
    CORBA::Request_var req = manager_obj_->_request ("dui_start");
    req->add_in_arg() <<= verbose;
    req->result()->value()->set_type (CORBA::_tc_boolean);
    req->invoke();
    if (req->env()->exception())
        mico_throw (*req->env()->exception());
    *req->result()->value() >>= CORBA::Any::to_boolean(b);
    std::cout << "dui_start invoked." << std::endl;
  }
  return b;
}

CORBA::Boolean
TextUITask::dui_stop()
{
  CORBA::Boolean b = FALSE;
  if (!CORBA::is_nil(manager_obj_)) {
    CORBA::Any any;
    CORBA::Request_var req = manager_obj_->_request ("dui_stop");
    req->result()->value()->set_type (CORBA::_tc_boolean);
    req->invoke();
    if (req->env()->exception())
        mico_throw (*req->env()->exception());
    *req->result()->value() >>= CORBA::Any::to_boolean(b);
    std::cout << "dui_stop invoked." << std::endl;
  }
  return b;
}

std::string
TextUITask::typeName(const Demon::Symbol& symbol) const
{
  std::string tname;

  switch(symbol.value._d()) {
  case Demon::stPTask :
    tname = "PTask";
    break;
  case Demon::stMessage :
    tname = "Message";
    break;
  case Demon::stInetAddr4 :
    tname = "InetAddr4";
    break;
  case Demon::stLinkList :
    tname = "LinkList";
    break;
  case Demon::stState :
    tname = "State";
    break;
  case Demon::stPort :
    tname = "Port";
    break;
  case Demon::stIface :
    tname = "Iface";
    break;
  case Demon::stTimer :
    tname = "Timer";
    break;
  case Demon::stSint8 :
    tname = "Sint8";
    break;
  case Demon::stSint16 :
    tname = "Sint16";
    break;
  case Demon::stSint32 :
    tname = "Sint32";
    break;
  case Demon::stUint8 :
    tname = "Uint8";
    break;
  case Demon::stUint16 :
    tname = "Uint16";
    break;
  case Demon::stUint32 :
    tname = "Uint32";
    break;
  case Demon::stFrame :
    tname = "Frame";
    break;
  case Demon::stSystem :
    tname = "System";
    break;
  default :
    tname = "Unknown";
    break;
  }
  return tname;
}

void 
TextUITask::printSymbol(const Demon::Symbol& symbol, int level)
{
  std::string offset;
  char strbuf[256];

  for (int k = 0; k < level; k++) {
    offset += "  ";
  }

  std::cout << offset;
  Demon::InetAddr4 addr4;
  Demon::Timer timer;
  register int i;
  std::cout << (const char*)symbol.name << ": ";
  switch(symbol.value._d()) {
  case Demon::stEventTask :
    std::cout << "EventTask = { " << std::endl;
    for (i = 0; i < symbol.children.length(); i++) {
      printSymbol(symbol.children[i], level + 1);
    }
    std::cout << offset << "} -- " << (const char*)symbol.name << std::endl;
    break;
  case Demon::stPTask :
    std::cout << "PTask = { " << std::endl;
    for (i = 0; i < symbol.children.length(); i++) {
      printSymbol(symbol.children[i], level + 1);
    }
    std::cout << offset << "} -- " << (const char*)symbol.name << std::endl;
    break;
  case Demon::stMessage :
    std::cout << "Message from " << (const char*)symbol.value.message().source.parentname;
    std::cout << ":" << (const char*)symbol.value.message().source.otherport;
    std::cout << " to ";
    std::cout << (const char*)symbol.value.message().target.parentname;
    std::cout << ":" << (const char*)symbol.value.message().target.otherport;
    std::cout << " = { " << std::endl;
    for (i = 0; i < symbol.children.length(); i++) {
      printSymbol(symbol.children[i], level + 1);
    }
    std::cout << offset << "} -- " << (const char*)symbol.name << std::endl;
    break;
  case Demon::stInetAddr4 :
    addr4 = symbol.value.inetaddr4();
    std::cout << "InetAddr4 = ";
    std::cout << (addr4 & 0xff) << "." << ((addr4>>8) & 0xff) << ".";
    std::cout << ((addr4>>16) & 0xff) << "." << ((addr4>>24) & 0xff) << std::endl;
    break;
  case Demon::stLinkList :
    std::cout << "LinkList len = " << symbol.value.linklist() << std::endl;
    break;
  case Demon::stState :
    std::cout << "state (" << symbol.value.state() << ")" << std::endl;
    break;
  case Demon::stPort :
    std::cout << "Port" << std::endl;
    break;
  case Demon::stIface :
    std::cout << "Iface" << std::endl;
    break;
  case Demon::stTimer :
    timer = symbol.value.timer();
    std::cout << "Timer = ";
    if (timer.is_ticking) {
      std::cout << (double)timer.delta.seconds + 
	(double)timer.delta.useconds / 1000000.0 << " / ";
    }
    std::cout << (double)timer.initial.seconds +
      (double)timer.initial.useconds / 1000000.0 << std::endl;
    break;
  case Demon::stSint8 :
    std::cout << "Sint8 = " << (int)symbol.value.sint8() << std::endl;
    break;
  case Demon::stSint16 :
    std::cout << "Sint16 = " << (int)symbol.value.sint16() << std::endl;
    break;
  case Demon::stSint32 :
    std::cout << "Sint32 = " << (long)symbol.value.sint32() << std::endl;
    break;
  case Demon::stUint8 :
    std::cout << "Uint8 = " << (int)symbol.value.uint8() << std::endl;
    break;
  case Demon::stUint16 :
    std::cout << "Uint16 = " << (int)symbol.value.uint16() << std::endl;
    break;
  case Demon::stUint32 :
    std::cout << "Uint32 = " << (unsigned long)symbol.value.uint32() << std::endl;
    break;
  case Demon::stFrame :
    std::cout << "Frame = ";
    for (i = 0; i < symbol.value.frame().length(); i++) {
      std::cout << hex << (int)symbol.value.frame()[i] << dec << " ";
      if (i % 9 == 8) {
	//	std::cout << std::endl << "        ";
	std::cout << "... (" << symbol.value.frame().length();
	std::cout << " values)" << std::endl;
	break;
      }
    }
    break;
  case Demon::stSystem :
    for (i = 0; i < symbol.children.length(); i++) {
      printSymbol(symbol.children[i], level + 1);
    }
    break;
  default :
    std::cout << "+++ Unknown type" << std::endl;
    break;
  }
}


