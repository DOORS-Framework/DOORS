/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/

#include "agentlistitem.h"

void ManagerMain::fileExit()
{
}

void ManagerMain::helpAbout()
{

}

void ManagerMain::selectSlot()
{
  AgentListItem* current = (AgentListItem*)agentList->currentItem();
  if (current) {
    current->showWindow();
  }
}

