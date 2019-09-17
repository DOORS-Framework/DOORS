#ifndef CORBAEVENT_H
#define CORBAEVENT_H

#include <CORBA.h>
#include <qevent.h>
#include <string>
#include "demon.h"

#define QEVENT_CORBA 366435

class CorbaEvent : public QCustomEvent 
{
 public:
  typedef enum { SetSystem, 
		 SendSymbol, 
		 SetManager, 
		 Start, 
		 Stop,
                 Disconnect,
                 SetFilter } CorbaEventType;

  CorbaEvent(CorbaEventType type) : QCustomEvent (QEVENT_CORBA), 
    type_(type), handle_(0), symbol_(0), system_(0), event_id_(0),
    filterlist_(0) { }

  CorbaEventType eventtype() const { return type_; };

  void setSystem(const Demon::Symbol* system) { system_ = system; }
  void setSymbol(CORBA::ULong event_id, CORBA::Long handle,
		 Demon::TimeStamp time,
 		 const Demon::Symbol* symbol) { event_id_ = event_id;
                                                handle_ = handle;
		                                timestamp_ = time;
		                                symbol_ = symbol; }
  void setManager(CORBA::Long handle, std::string manager_ior)
    { handle_ = handle; manager_ior_ = manager_ior; }
  void setFilter(Demon::FilterList* filterlist) { filterlist_ = filterlist; }

  CORBA::Long getHandle() const { return handle_; }
  const Demon::Symbol* getSymbol() const { return symbol_; }
  const Demon::Symbol* getSystem() const { return system_; }
  Demon::FilterList* getFilter() { return filterlist_; }
  Demon::TimeStamp getTimeStamp() const { return timestamp_; }
  std::string getManagerIOR() { return manager_ior_; }
  CORBA::ULong getEventId() { return event_id_; }

 private:
  CorbaEventType type_;
  CORBA::Long handle_;
  const Demon::Symbol* symbol_;
  const Demon::Symbol* system_;
  Demon::FilterList* filterlist_;
  Demon::TimeStamp timestamp_;
  std::string manager_ior_;
  CORBA::ULong event_id_;
};

#endif

