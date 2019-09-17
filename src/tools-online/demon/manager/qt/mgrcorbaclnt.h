#ifndef MGRCORBACLNT_H
#define MGRCORBACLNT_H

#include <CORBA.h>
#include <qobject.h>
#include <qevent.h>

class MgrCorbaClnt : public QObject
{
  Q_OBJECT

 public:
  MgrCorbaClnt(CORBA::Object_ptr agent_obj);
  ~MgrCorbaClnt();

  // Messages from Manager to Agent
  CORBA::Boolean setFilter(Demon::FilterList* filterlist);
  CORBA::Boolean setManager(CORBA::Long handle, const char* manager_ior);
  CORBA::Boolean start();
  CORBA::Boolean stop();
  CORBA::Boolean disconnect();

 protected:
  virtual void customEvent(QCustomEvent* event);

 private:
  CORBA::Object_ptr agent_obj_;             // Agent object pointer
};

#endif
