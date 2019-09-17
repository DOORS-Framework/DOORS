#include <CORBA.h>
#include "slpevent.h"
#include "managerevent.h"
#include "agentlistitem.h"
#include "agentlist.h"
#include "agentlist.ui.h"
#include "agentlist.moc.h"

AgentList::AgentList(QWidget* parent, const char* name, WFlags f)
  : QListView(parent, name, f), manager_(0)
{
  addColumn( trUtf8( "ID" ) );
  addColumn( trUtf8( "Name" ) );
  addColumn( trUtf8( "State" ) );

  connect(this, SIGNAL(doubleClicked(QListViewItem*)), 
	  this, SLOT(listDoubleclicked(QListViewItem*)));
  connect(this, SIGNAL(clicked(QListViewItem*,const QPoint&,int)), 
	  this, SLOT(listClicked(QListViewItem*,const QPoint&,int)));
}

AgentList::~AgentList()
{

}

void
AgentList::addAgent(std::string agent, std::string ior)
{
  AgentListItem* item = new AgentListItem(agent, ior, this);
  // Send manager object pointer to the list item
  if (manager_) {
    QObject* sigrec = (QObject*)item->getWindow();
    if (sigrec) {
      ManagerEvent* event = new ManagerEvent(ManagerEvent::SetManager);
      event->setManager(manager_);
      QApplication::postEvent(sigrec, event);
    }
  }

}

void 
AgentList::customEvent(QCustomEvent* event)
{
  if (event->type() == QEVENT_SLP) {
    SLPEvent* slpevent = (SLPEvent*)event;
    if (slpevent->eventtype() == SLPEvent::NewAgent) {
      addAgent(slpevent->agent(), slpevent->IOR());
    }
  } else if (event->type() == QEVENT_MANAGER) {
    ManagerEvent* mevent = (ManagerEvent*)event;
    if (mevent->eventtype() == ManagerEvent::SetManager) {
      std::cout << "AgentList::customEvent: ManagerEvent::SetManager received.";
      std::cout << std::endl;
      manager_ = mevent->getManager();
    }
  }
}

