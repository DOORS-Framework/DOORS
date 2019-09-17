#ifndef MANAGEREVENT_H
#define MANAGEREVENT_H

#include <qobject.h>
#include <qevent.h>
#include <string>

#define QEVENT_MANAGER 366436

// Manager internal event class
class ManagerEvent : public QCustomEvent 
{
 public:
  typedef enum { CorbaConnOk, SetManager } ManagerEventType;

  ManagerEvent(ManagerEventType type) 
    : QCustomEvent (QEVENT_MANAGER), type_(type) { }

  ManagerEventType eventtype() const { return type_; };

  void setManager(QObject* manager) { manager_ = manager; }

  QObject* getManager() { return manager_; }

 private:
  ManagerEventType type_;
  QObject* manager_;
};

#endif
