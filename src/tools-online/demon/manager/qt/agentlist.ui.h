#include <qapplication.h>
#include "agentwin.h"
#include "agentlistitem.h"
#include "managerevent.h"

void AgentList::listClicked(QListViewItem* item, 
			   const QPoint& point,
			   int n)
{
  std::cout << "void AgentList::listClicked()" << std::endl;
}

void AgentList::listDoubleclicked(QListViewItem* item)
{
  if (!item) {
    // Not clicked on an item; ignore
    return;
  }
  // Get pointer to the agent list item
  AgentListItem* agent = (AgentListItem*)item;
  AgentWin* win = agent->getWindow();
  // If window is valid, signal it show and raise
  if (win) {
    win->show();
    win->raise();
  }
}

