#ifndef AGENTEVENT_H
#define AGENTEVENT_H

#include <CORBA.h>
#include <string>
#include <doors/demon.h>
#include <qlistview.h>
#include <qobject.h>
#include <doors/symboltools.h>

class AgentEvent : public QObject, public QListViewItem, public SymbolTools
{
  Q_OBJECT

public:
  AgentEvent(unsigned long id, CORBA::Long handle, Demon::TimeStamp timestamp, 
	     const Demon::Symbol* symbol, QListView* parent);
  ~AgentEvent();

  const Demon::Symbol* getSymbol() const { return symbol_; }
  unsigned long getId() const { return id_; }
  CORBA::Long getHandle() const { return handle_; }
  Demon::TimeStamp getTimeStamp() const { return timestamp_; }

public slots:
  void raiseMe();

protected:
  virtual void showAttribute(QString& str) 
               { (void)new QListViewItem(this, str); }

private:
  QListView* parent_;
  CORBA::Long handle_;
  Demon::TimeStamp timestamp_;
  const Demon::Symbol* symbol_;
  unsigned long id_;
};

#endif

