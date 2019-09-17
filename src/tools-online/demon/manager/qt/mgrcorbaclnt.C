#include <CORBA.h>
#include <mico/throw.h>
#include <string>
#include "corbaevent.h"

#include "mgrcorbaclnt.h"
#include "mgrcorbaclnt.moc.h"

MgrCorbaClnt::MgrCorbaClnt(CORBA::Object_ptr agent_obj)
  : agent_obj_(agent_obj)
{

}

MgrCorbaClnt::~MgrCorbaClnt()
{

}

CORBA::Boolean
MgrCorbaClnt::setManager(CORBA::Long handle, const char* manager_ior)
{
  CORBA::Boolean b = FALSE;
  if (!CORBA::is_nil(agent_obj_)) {
    CORBA::Any any;
    CORBA::Request_var req = agent_obj_->_request ("setManager");
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
MgrCorbaClnt::start()
{
  CORBA::Boolean b = FALSE;
  if (!CORBA::is_nil(agent_obj_)) {
    CORBA::Any any;
    CORBA::Request_var req = agent_obj_->_request ("start");
    req->result()->value()->set_type (CORBA::_tc_boolean);
    req->invoke();
    if (req->env()->exception())
        mico_throw (*req->env()->exception());
    *req->result()->value() >>= CORBA::Any::to_boolean(b);
    std::cout << "start invoked." << std::endl;
  }
  return b;
}

CORBA::Boolean 
MgrCorbaClnt::stop()
{
  CORBA::Boolean b = FALSE;
  if (!CORBA::is_nil(agent_obj_)) {
    CORBA::Any any;
    CORBA::Request_var req = agent_obj_->_request ("stop");
    req->result()->value()->set_type (CORBA::_tc_boolean);
    req->invoke();
    if (req->env()->exception())
        mico_throw (*req->env()->exception());
    *req->result()->value() >>= CORBA::Any::to_boolean(b);
    std::cout << "stop invoked." << std::endl;
  }
  return b;
}

CORBA::Boolean 
MgrCorbaClnt::disconnect()
{
  CORBA::Boolean b = FALSE;
  if (!CORBA::is_nil(agent_obj_)) {
    CORBA::Any any;
    CORBA::Request_var req = agent_obj_->_request ("disconnect");
    req->result()->value()->set_type (CORBA::_tc_boolean);
    req->invoke();
    if (req->env()->exception())
        mico_throw (*req->env()->exception());
    *req->result()->value() >>= CORBA::Any::to_boolean(b);
    std::cout << "disconnect invoked." << std::endl;
  }
  return b;
}

CORBA::Boolean
MgrCorbaClnt::setFilter(Demon::FilterList* filterlist)
{
  CORBA::Boolean b = FALSE;
  if (!CORBA::is_nil(agent_obj_)) {
    CORBA::Any any;
    CORBA::Request_var req = agent_obj_->_request ("setFilter");
    req->add_in_arg() <<= (Demon::FilterList)*filterlist;
    req->result()->value()->set_type (CORBA::_tc_boolean);
    req->invoke();
    if (req->env()->exception())
        mico_throw (*req->env()->exception());
    *req->result()->value() >>= CORBA::Any::to_boolean(b);
    std::cout << "setFilter invoked." << std::endl;
  }
  delete filterlist; // Used; throw away
  return b;
}

void 
MgrCorbaClnt::customEvent(QCustomEvent* event)
{
  if (event->type() == QEVENT_CORBA) {
    CorbaEvent* corbaevent = (CorbaEvent*)event;
    switch(corbaevent->eventtype()) {
    case CorbaEvent::SetManager :
      std::cout << "MgrCorbaClnt::customEvent: SetManager received." << std::endl;
      setManager(corbaevent->getHandle(), corbaevent->getManagerIOR().c_str());
      break;
    case CorbaEvent::Start :
      start();
      break;
    case CorbaEvent::Stop :
      stop();
      break;
    case CorbaEvent::SetFilter :
      setFilter(corbaevent->getFilter());
      break;
    default :
      // Shouldn't happen; ignore
      break;
    }
  }
}

