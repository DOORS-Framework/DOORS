#include <qlistview.h>
#include <qpixmap.h>
#include "managericons.h"
#include "managerevent.h"
#include "agentwin.h"
#include "agentlistitem.h"
#include "agentlistitem.moc.h"

// idpool_ servers as a global information about unique id's.
// It is increased by one every time a new AgentListItem is 
// created to make sure that all the id's are unique.
unsigned long AgentListItem::idpool_ = 0;

AgentListItem::AgentListItem(std::string agent, std::string ior, 
			     QListView* parent)
  : QObject(this, "AgentListItem"), QListViewItem(parent),
    agent_(agent), ior_(ior), parent_(parent), id_(++idpool_)
{
  // Remove SLP specific prefix from the name
  QString agentstr = QString(agent.c_str()).mid(16);

  redBall = new QPixmap(redball);
  greenBall = new QPixmap(greenball);

  setPixmap(0, *redBall);

  // Add ID
  setText(0, QString("%1").arg(id_));

  // Add Agent name
  setText(1, agentstr);

  // Add initial status
  setText(2, QString("Connecting..."));

  // Create window for this item
  window_ = new AgentWin(this, parent, ior, (const char*)agentstr);
}

AgentListItem::~AgentListItem()
{
  delete redBall;
  delete greenBall;

  // No need to delete window_ as it is deleted by Qt
}

void
AgentListItem::showWindow()
{
  if (window_) {
    window_->show();
    window_->raise();
  }
}

void
AgentListItem::setConnected() 
{
  setText(2, QString("Connected."));
  setPixmap(0, *greenBall);
}

void
AgentListItem::setDisconnected() 
{
  setText(2, QString("Disconnected."));
  setPixmap(0, *redBall);
}

void 
AgentListItem::customEvent(QCustomEvent* event)
{
  if (event->type() == QEVENT_MANAGER) {
    ManagerEvent* mevent = (ManagerEvent*)event;
    if (mevent->eventtype() == ManagerEvent::CorbaConnOk) {
      std::cout << "Conn ok" << std::endl;
    }
  }

}
