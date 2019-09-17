#include <CORBA.h>
#include "demon.h"
#include <qpixmap.h>
#include "managericons.h"
#include "agentevent.h"
#include "agentevent.moc.h"

AgentEvent::AgentEvent(unsigned long id, CORBA::Long handle, 
		       Demon::TimeStamp timestamp, 
		       const Demon::Symbol* symbol, QListView* parent)
  : handle_(handle), timestamp_(timestamp), symbol_(symbol), 
    QObject(this, "AgentEvent"), QListViewItem(parent), 
    parent_(parent), id_(id)
{
  // Set pixmap
  setPixmap(0, QPixmap(pix_file));

  // ID
  setText(0, QString("%1").arg(id_));

  // Time
  setText(1, QString("%1.%2").arg(timestamp.seconds).arg(timestamp.useconds));

  // Name
  setText(2, QString(symbol->name));

  // Type
  setText(3, QString(typeName(symbol).c_str()));
}

AgentEvent::~AgentEvent()
{
}

void
AgentEvent::raiseMe()
{
  parent_->setSelected(this, true);
  parent_->ensureItemVisible(this);
  parent_->setCurrentItem(this);

  //TODO: Symbol should be shown in AgentSymbol list but how?
}


