/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/

#include <qapplication.h>
#include <iostream>
#include "agentevent.h"
#include "tracewin.h"
#include "agentsymbol.h"

void AgentWin::traceButton()
{
  static TraceWin* trace = 0;

  std::cout << "void AgentWin::traceButton()" << std::endl;
  if (!trace && system_) { // System must be set before trace
    trace = new TraceWin(AgentEventList, "TraceWin", system_);
  }

  if (trace) {
    trace->show();
    trace->raise();
  }
}

void AgentWin::selectButton()
{
  std::cout << "void AgentWin::selectButton()" << std::endl;
  QListViewItem* list = AgentEventList->currentItem();
  raiseAgentSymbol(list);
}

void AgentWin::closeButton()
{
  // Just hide the window because we also keep the CORBA 
  // connection alive
  hide();
}

void AgentWin::clickedAgentList(QListViewItem* list, const QPoint& point,
				int row)
{
  raiseAgentSymbol(list);
  std::cout << "void AgentWin::clickedAgentList()" << std::endl;
}

void AgentWin::raiseAgentSymbol(QListViewItem* list)
{
  static AgentSymbol* symbol = 0;
  AgentEvent* event = (AgentEvent*)list;
  if (event) {
    if (symbol) {
      delete symbol;
      symbol = 0;
    }
    symbol = new AgentSymbol(event->getSymbol(), AgentSymbolList, true);
  }
}

void AgentWin::doubleclickedAgentList(QListViewItem* item)
{
  std::cout << "void AgentWin::doubleclickedAgentList()" << std::endl;
}

void AgentWin::selchangedAgentList(QListViewItem* item)
{
  std::cout << "selchangedAgentList()" << std::endl;
}

void AgentWin::clickedSymbolist(QListViewItem* list, const QPoint& point,
				int row)
{
  std::cout << "void AgentWin::clickedSymbolist() = ";
  std::cout << point.x() << std::endl;
}

void AgentWin::doubleclickedSymbolist(QListViewItem* item)
{
  std::cout << "void AgentWin::doubleclickedSymbolist()" << std::endl;
}

void AgentWin::systemSlot()
{
  if (!systemview_) {
    systemview_ = new SystemView(system_, this, 
				 (QObject*)corbaclnt_, agent_url_);
  }
  
  if (systemview_) {
    systemview_->show();
    systemview_->raise();
  }
}

