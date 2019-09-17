#include <CORBA.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <slp.h>
#include <doors/hsi.h>

#include "demon.h"
#include <doors/managertask.h>

// TODO: remove this
#include <fstream>

// #define IOR_TO_FILE

// Used by static SLP callback functions hence defined also as static
bool ManagerTask::pending_ = false;
std::vector<struct slpdatum> ManagerTask::slpdata_;
::CORBA::ORB_ptr ManagerTask::orb_ = 0;
std::string ManagerTask::ior_;

// The Instance
ManagerTask* ManagerTask::instance_ = 0;

ManagerTask*
ManagerTask::Instance()
{
  if (instance_ == 0) {
    instance_ = new ManagerTask;
  }
  return instance_;
}

ManagerTask::ManagerTask()
  : EventTask ("manager", Scheduler::Instance()), terminated_(false), 
    poa_(0), agent_obj_(0), timer_(this, OTime(0, 1)), 
    sending_(false), slp_(0), initial_(true), verbose_(0), dui_flow_(false),
    message_pending_(false)
{
  // Create ORB and POA
  char* argvector[] = { (char*)"manager_server", 0 };
  int argcount = 1;
  orb_ = ::CORBA::ORB_init(argcount, argvector, "mico-local-orb");
  ::CORBA::Object_ptr poaobj = orb_->resolve_initial_references("RootPOA");
  poa_ = PortableServer::POA::_narrow(poaobj);

  // Activate this object to receive the invokes
  PortableServer::ObjectId_var oid = poa_->activate_object (this);

  // Save reference
  CORBA::Object_ptr ref = poa_->id_to_reference (oid.in());
  ior_ = orb_->object_to_string (ref);

  // Activate POA
  poa_->the_POAManager()->activate();

  // TODO: Create handlers for these:
  //       HandlerCreator::createHandler(CORBA::ORB*, Symbol*)
  //       HandlerCreator::createHandler(PortableServer::POA*, Symbol*)
  //  DYNAMIC_SYMBOL(orb_, "ORB", this);
  //  DYNAMIC_SYMBOL(poa_, "POA", this);

  // Start timer
  timer_.start();

}

ManagerTask::~ManagerTask(void) 
{
  DELETE_SYMBOL (this);

  poa_->destroy(TRUE, TRUE);

#ifndef IOR_TO_FILE
  // Close SLP
  if (slp_) {
    SLPClose(slp_);
    slp_ = 0;
  }
#endif

}

SLPBoolean 
ManagerTask::SLPSrvCallback(SLPHandle handle, const char* srvtypes, 
			    unsigned short lifetime, SLPError error, 
			    void* cookie)
{
  std::cerr << "Entering SLPSrvCallback" << std::endl;
  if (error == SLP_OK) {
    struct slpdatum datum;
    datum.url = srvtypes;
    slpdata_.push_back(datum);
    std::cout << "srvtype = " << srvtypes << ", lifetime = ";
    std::cout << lifetime << std::endl;
    
    return SLP_TRUE;
  } else {
    if (error == SLP_LAST_CALL) {
      std::cerr << "Last call to server callback function." << std::endl;
    } else {
      std::cerr << "SLP server callback failed. error = " << error << std::endl;
    }
  }

  // No more servers to come; start pending for attributes
  pending_ = true;

  return SLP_FALSE;
}

SLPBoolean 
ManagerTask::SLPAttCallback(SLPHandle handle, const char* attrlist, 
			    SLPError error, void* cookie)
{
  if (error == SLP_OK) {
    int* index = (int*)cookie;
    // Check sanity of index
    if (*index < 0 || *index >= slpdata_.size()) {
      return SLP_FALSE;
    }
    std::cout << "ior = " << attrlist << std::endl;
    slpdata_[*index].attributes_set = true;
    slpdata_[*index].ior = attrlist;
    slpdata_[*index].obj = orb_->string_to_object(attrlist);
    // Set Manager for Agent
    slpdata_[*index].setManager(*index, ior_.c_str());
  } else {
    std::cerr << "SLP attribute callback failed." << std::endl;
  }
  pending_ = true;

  return SLP_FALSE; // No use calling this callback again
}

// EventTask execute() rewritten to run one task in the ORB if needed.
bool 
ManagerTask::execute (EventType type, Message *msg)
{
  std::ifstream file;
  SLPError slperr;

  // Executed once during the first call
  if (initial_) {
#ifndef IOR_TO_FILE
    // Open SLP
    //    slperr = SLPOpen("default", SLP_TRUE/*=async*/, &slp_);
    //    if (slperr != SLP_OK) {
    //      if (slperr == SLP_NOT_IMPLEMENTED) {
	// Asynchronous stuff possibly not implemented; try synchronous
	slperr = SLPOpen("default", SLP_FALSE/*=sync*/, &slp_);
	//      }
	//    }

    if (slperr == SLP_OK) {
      std::cerr << "Trying SLP..." << std::endl;
      slperr = SLPFindSrvs(slp_, "service:demon", "default", "", 
			   &ManagerTask::SLPSrvCallback, 0);
      if (slperr == SLP_OK) {
	std::cerr << "Success with SLP." << std::endl;
      } else {
	std::cerr << "No success with SLP." << std::endl;
      }
    }
#else
    // Open agent IOR file
    file.open("/tmp/agent.ref");
    std::string agent_ior;
    file >> agent_ior;
    std::cout << "agent ior = " << agent_ior << std::endl;
    struct slpdatum datum;
    datum.url = "service:demon";
    slpdata_.push_back(datum);
    slpdata_[0].url = "service:demon://unknown/1";
    slpdata_[0].attributes_set = true;
    slpdata_[0].ior = agent_ior;
    slpdata_[0].obj = orb_->string_to_object(agent_ior.c_str());
    slpdata_[0].setManager(0, ior_.c_str());
    file.close();
#endif
    // Open UI IOR file
    file.open("/tmp/ui.ref");
    std::string ui_ior;
    file >> ui_ior;
    std::cout << "ui ior = " << ui_ior << std::endl;
    ui_obj_ = orb_->string_to_object(ui_ior.c_str());
    (void)dui_setManager(ior_.c_str());
    file.close();
    initial_ = false;
  }

#ifndef IOR_TO_FILE
  // Pending for attributes?
  std::string url;
  bool find_attributes = false;
  static int index = 0;
  if (pending_) {
    pending_ = false;
    for (int i = 0; i < slpdata_.size(); i++) {
      if (!slpdata_[i].attributes_set) {
	// No attributes; start asking
	find_attributes = true;
	url = slpdata_[i].url;
	index = i;
	std::cout << "no attributes" << std::endl;
	break;
      }
    }
  }

  // Start searching for attributes
  if (find_attributes) {
    std::cout << "url = " << url << std::endl;
    slperr = SLPFindAttrs(slp_, url.c_str(), "default", "",
			  &ManagerTask::SLPAttCallback, &index);
    // Set this Agent for UI
    dui_setAgent(index, slpdata_[index].url.c_str());
  }
#endif

  if (message_pending_) {
    int size = messages_.size();
    if (size > 0) {
      dui_sendSymbol(messages_[size - 1].get_handle(),
		     messages_[size - 1].get_time(),
		     messages_[size - 1].get_symbol());
    }
    message_pending_ = false;
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


void
ManagerTask::invoke (CORBA::ServerRequest_ptr svreq)
{
  if (std::string(svreq->op_name()) == std::string("setSystem")) {
    // create argument list
    CORBA::NVList_ptr args;
    orb_->create_list (0, args);
    args->add(CORBA::ARG_IN);
    args->item(0)->value()->set_type(Demon::_tc_Symbol);

    svreq->params(args);

    Demon::Symbol system;
    *args->item(0)->value() >>= system;
    CORBA::Boolean b = setSystem(system);
    CORBA::Any *res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean(b);
    svreq->result(res);
  } else if (std::string(svreq->op_name()) == std::string("sendSymbol")) {
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
    CORBA::Boolean b = sendSymbol(handle, timestamp, symbol);
    CORBA::Any *res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean(b);
    svreq->result(res);
  } else if (std::string(svreq->op_name()) == std::string("dui_start")) {
    // create argument list
    CORBA::NVList_ptr args;
    orb_->create_list (0, args);
    args->add(CORBA::ARG_IN);
    args->item(0)->value()->set_type(CORBA::_tc_short);

    svreq->params(args);
    
    CORBA::Short verbose;
    *args->item(0)->value() >>= verbose;
    CORBA::Boolean b = dui_start(verbose);
    CORBA::Any *res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean(b);
    svreq->result(res);
  } else if (std::string(svreq->op_name()) == std::string("dui_stop")) {
    // create argument list
    CORBA::NVList_ptr args;
    orb_->create_list (0, args);
    // No arguments
    svreq->params(args);
    
    CORBA::Boolean b = dui_stop();
    CORBA::Any *res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean(b);
    svreq->result(res);
  } 
}

char* 
ManagerTask::_primary_interface(const PortableServer::ObjectId&, 
                              PortableServer::POA_ptr) 
{ 
  return CORBA::string_dup("IDL:Demon/Manager:1.0"); 
} 

CORBA::Boolean 
ManagerTask::setSystem (Demon::Symbol system)
{
  std::cout << "ManagerTask::setSystem called." << std::endl;

  return FALSE;
}

CORBA::Boolean
ManagerTask::sendSymbol (CORBA::Long handle, 
			 Demon::TimeStamp time, Demon::Symbol symbol)
{
  std::cout << "Time: " << time.seconds << "." << time.useconds << std::endl;

  messages_.push_back(demon_message(handle, time, symbol));
  message_pending_ = true;

  return FALSE;
}

CORBA::Boolean
slpdatum::setManager(CORBA::Long handle, const char* manager_ior)
{
  CORBA::Boolean b = FALSE;
  if (!CORBA::is_nil(obj)) {
    CORBA::Any any;
    CORBA::Request_var req = obj->_request ("setManager");
    req->add_in_arg() <<= handle;
    req->add_in_arg() <<= manager_ior;
    req->result()->value()->set_type (CORBA::_tc_boolean);
    req->invoke();
    if (req->env()->exception())
        mico_throw (*req->env()->exception());
    *req->result()->value() >>= CORBA::Any::to_boolean(b);
    std::cout << "setManager invoked." << std::endl;
  }
  return b;
}

CORBA::Boolean 
slpdatum::start()
{
  return FALSE;
}

CORBA::Boolean 
slpdatum::stop()
{
  return FALSE;
}

CORBA::Boolean 
ManagerTask::dui_start(CORBA::Short verbose)
{
  verbose_ = verbose;
  dui_flow_ = true;

  return FALSE;
}

CORBA::Boolean 
ManagerTask::dui_stop()
{
  dui_flow_ = false;

  return FALSE;
}

CORBA::Boolean
ManagerTask::dui_sendSymbol (CORBA::Long agent_handle, Demon::TimeStamp time, 
			     Demon::Symbol symbol)
{
  CORBA::Boolean b = FALSE;
  if (dui_flow_ && !CORBA::is_nil(ui_obj_)) {
    CORBA::Request_var req = ui_obj_->_request ("dui_sendSymbol");
    req->add_in_arg() <<= agent_handle;
    req->add_in_arg() <<= time;
    req->add_in_arg() <<= symbol;
    req->result()->value()->set_type (CORBA::_tc_boolean);
    req->invoke();
    *req->result()->value() >>= CORBA::Any::to_boolean(b);
    std::cout << "symbol sent to UI." << std::endl << std::flush;
  }
  return b;
}

CORBA::Boolean
ManagerTask::dui_setManager(const char* manager_ior)
{
  CORBA::Boolean b = FALSE;
  if (!CORBA::is_nil(ui_obj_)) {
    CORBA::Any any;
    CORBA::Request_var req = ui_obj_->_request ("dui_setManager");
    req->add_in_arg() <<= manager_ior;
    req->result()->value()->set_type (CORBA::_tc_boolean);
    req->invoke();
    if (req->env()->exception())
        mico_throw (*req->env()->exception());
    *req->result()->value() >>= CORBA::Any::to_boolean(b);
    std::cout << "dui_setManager invoked." << std::endl;
  }
  return b;
}

CORBA::Boolean
ManagerTask::dui_setAgent(CORBA::Long handle, const char* agent_url)
{
  CORBA::Boolean b = FALSE;
  if (!CORBA::is_nil(ui_obj_)) {
    CORBA::Any any;
    CORBA::Request_var req = ui_obj_->_request ("dui_setAgent");
    req->add_in_arg() <<= handle;
    req->add_in_arg() <<= agent_url;
    req->result()->value()->set_type (CORBA::_tc_boolean);
    req->invoke();
    if (req->env()->exception())
        mico_throw (*req->env()->exception());
    *req->result()->value() >>= CORBA::Any::to_boolean(b);
    std::cout << "dui_setAgent invoked." << std::endl;
  }
  return b;
}

CORBA::Boolean
ManagerTask::dui_unsetAgent(CORBA::Long handle)
{
  CORBA::Boolean b = FALSE;
  if (!CORBA::is_nil(ui_obj_)) {
    CORBA::Any any;
    CORBA::Request_var req = ui_obj_->_request ("dui_unsetAgent");
    req->add_in_arg() <<= handle;
    req->result()->value()->set_type (CORBA::_tc_boolean);
    req->invoke();
    if (req->env()->exception())
        mico_throw (*req->env()->exception());
    *req->result()->value() >>= CORBA::Any::to_boolean(b);
    std::cout << "dui_unsetAgent invoked." << std::endl;
  }
  return b;
}



