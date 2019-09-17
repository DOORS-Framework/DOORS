#include <CORBA.h>
#include "demon.h"
#include <iostream>
#include <qimage.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <qapplication.h>
#include "corbaevent.h"
#include "mgrcorbasrvr.h"
#include "mgrcorbasrvr.moc.h"

MgrCorbaSrvr::MgrCorbaSrvr(QObject* sigrec, AgentListItem* listitem)
  : sigrec_(sigrec), state_(NotConnected), listitem_(listitem)
{
  // Create ORB and BOA
  char* argvector[] = { (char*)"manager_client", 0 };
  int argcount = 1;
  orb_ = ::CORBA::ORB_init(argcount, argvector, "mico-local-orb");
  ::CORBA::Object_ptr poaobj = orb_->resolve_initial_references("RootPOA");
  poa_ = PortableServer::POA::_narrow(poaobj);

  // Activate this object to receive the invokes
  PortableServer::ObjectId_var oid = poa_->activate_object (this);

  // Activate POA
  poa_->the_POAManager()->activate();

  CORBA::Object_ptr ref = poa_->id_to_reference (oid.in());
  ior_ = orb_->object_to_string (ref);

  // Create timer that calls callORB() every time the Qt
  // system's event queue have been processed
  QTimer *t_orb = new QTimer(this);
  connect(t_orb, SIGNAL(timeout()), SLOT(update()));
  // TODO: Changed from zero to some amount because zero makes
  //       manager process to consume all the CPU time. Check this.
  t_orb->start(1000, FALSE);

}

MgrCorbaSrvr::~MgrCorbaSrvr()
{

}


// CORBA standards

void
MgrCorbaSrvr::invoke (CORBA::ServerRequest_ptr svreq)
{
  if (std::string(svreq->op_name()) == std::string("setSystem")) {
    // create argument list
    CORBA::NVList_ptr args;
    orb_->create_list (0, args);
    args->add(CORBA::ARG_IN);
    args->item(0)->value()->set_type(Demon::_tc_Symbol);

    svreq->params(args);

    Demon::Symbol* system = new Demon::Symbol;
    *args->item(0)->value() >>= *system;
    CORBA::Boolean b = setSystem(system);
    CORBA::Any *res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean(b);
    svreq->result(res);
  } else if (std::string(svreq->op_name()) == std::string("sendSymbol")) {
    // create argument list
    CORBA::NVList_ptr args;
    orb_->create_list (0, args);
    args->add(CORBA::ARG_IN);
    args->item(0)->value()->set_type(CORBA::_tc_ulong);
    args->add(CORBA::ARG_IN);
    args->item(1)->value()->set_type(CORBA::_tc_long);
    args->add(CORBA::ARG_IN);
    args->item(2)->value()->set_type(Demon::_tc_TimeStamp);
    args->add(CORBA::ARG_IN);
    args->item(3)->value()->set_type(Demon::_tc_Symbol);

    svreq->params(args);

    CORBA::ULong event_id;
    *args->item(0)->value() >>= event_id;
    CORBA::Long handle;
    *args->item(1)->value() >>= handle;
    Demon::TimeStamp timestamp;
    *args->item(2)->value() >>= timestamp;
    Demon::Symbol* symbol = new Demon::Symbol;
    *args->item(3)->value() >>= *symbol;
    CORBA::Boolean b = sendSymbol(event_id, handle, timestamp, symbol);
    CORBA::Any *res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean(b);
    svreq->result(res);
  } else if (std::string(svreq->op_name()) == std::string("disconnect")) {
    CORBA::Boolean b = disconnect();
    CORBA::Any *res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean(b);
    svreq->result(res);
  }
}

char* 
MgrCorbaSrvr::_primary_interface(const PortableServer::ObjectId&, 
				 PortableServer::POA_ptr) 
{ 
  return CORBA::string_dup("IDL:Demon/Manager:1.0"); 
} 

void
MgrCorbaSrvr::update()
{
  // Refresh the ORB if needed
  if (orb_ && orb_->work_pending()) { 
    orb_->perform_work(); 
  }
}

CORBA::Boolean 
MgrCorbaSrvr::setSystem (const Demon::Symbol* system)
{
  std::cout << "ManagerTask::setSystem called." << std::endl;

  if (sigrec_) {
    CorbaEvent* event = new CorbaEvent(CorbaEvent::SetSystem);
    event->setSystem(system);
    QApplication::postEvent(sigrec_, event);

    if (state_ != Connected) {
      // Set state to connected directly with QListViewItem class method.
      // NOTE: postEvent is not used because it causes segfault for some
      //       unknown reason. Check that later.
      listitem_->setConnected();
      state_ = Connected;
    }
  }

  return FALSE;
}

CORBA::Boolean
MgrCorbaSrvr::sendSymbol (CORBA::ULong event_id, CORBA::Long handle, 
			  Demon::TimeStamp time, const Demon::Symbol* symbol)
{
  std::cout << "Time: " << time.seconds << "." << time.useconds << std::endl;

  if (sigrec_) {
    CorbaEvent* event = new CorbaEvent(CorbaEvent::SendSymbol);
    event->setSymbol(event_id, handle, time, symbol);
    QApplication::postEvent(sigrec_, event);
  }

  return FALSE;
}

CORBA::Boolean
MgrCorbaSrvr::disconnect()
{
  std::cout << "disconnect() from agent." << std::endl;

  if (listitem_) {
    listitem_->setDisconnected();
  }

  return FALSE;
}

CORBA::Object_ptr
MgrCorbaSrvr::stringToObject(std::string ior)
{ 
  if ((orb_) && (ior.length() != 0)) {
    return CORBA::Object::_duplicate(orb_->string_to_object(ior.c_str()));
  }
  return 0;
}

