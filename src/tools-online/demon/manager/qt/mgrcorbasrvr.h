#ifndef MGRCORBASRVR_H
#define MGRCORBASRVR_H

#include <CORBA.h>
#include <qlistview.h>
#include <qobject.h>
#include <qthread.h>
#include <string>
#include "agentlistitem.h"
#include "demon.h"

class MgrCorbaSrvr : public QObject,
		     public PortableServer::DynamicImplementation
{
  Q_OBJECT

public:
  MgrCorbaSrvr(QObject* sigrec, AgentListItem* listitem);
  ~MgrCorbaSrvr();

  std::string getIOR() { return ior_; }

  CORBA::Object_ptr stringToObject(std::string ior);

  // CORBA standards
  virtual void invoke (CORBA::ServerRequest_ptr svreq);
  virtual char* _primary_interface(const PortableServer::ObjectId&, 
				   PortableServer::POA_ptr); 

  // Incoming CORBA calls
  CORBA::Boolean setSystem (const Demon::Symbol* system);
  CORBA::Boolean sendSymbol (CORBA::ULong event_id, CORBA::Long handle,
			     Demon::TimeStamp time, 
			     const Demon::Symbol* symbol);
  CORBA::Boolean disconnect ();

 public slots:
  virtual void update();

private:
  typedef enum { NotConnected, Connected } SrvrState;
  SrvrState state_;
  ::PortableServer::POA_ptr poa_;
  ::CORBA::ORB_ptr orb_;
  std::string ior_;
  QObject* sigrec_;
  AgentListItem* listitem_;
};

#endif
