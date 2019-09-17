/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/

#include <CORBA.h>
#include "corbaevent.h"
#include <qapplication.h>
#include <qlistview.h>

void SystemView::changedFilterEdit(const QString& str)
{
  filterstr_ = str;
}

void SystemView::setFilter(Demon::Filter& filter, AgentSymbolCL* item)
{
  filter.index = item->getSymbol()->index;
  filter.enabled = item->isOn();

  filter.children.length(100);
  int index = 0;
  for (QListViewItem* i = item->firstChild(); i; i = i->nextSibling()) {
    // Accept only root symbols and not their attributes
    if (i->rtti() == AGENTSYMBOL_RTTI) {
      setFilter(filter.children[index++], (AgentSymbolCL*)i);
    }
  }
  filter.children.length(index);

}

void SystemView::clickedFilterButton()
{
  Demon::FilterList* fl = new Demon::FilterList;
  fl->length(1);
  setFilter((*fl)[0], symbol_);

  CorbaEvent* event = new CorbaEvent(CorbaEvent::SetFilter);
  event->setFilter(fl);
  QApplication::postEvent(corbaobj_, event);
}

void SystemView::checkChildren(QCheckListItem* item, bool enabled)
{
  item->setOn(enabled);

  for (QListViewItem* i = item->firstChild(); i; i = i->nextSibling()) {
    // Accept only root symbols and not their attributes
    if (i->rtti() == AGENTSYMBOL_RTTI) {
      checkChildren((QCheckListItem*)i, enabled);
    }
  }
}

void SystemView::checkParents(QListViewItem* item, bool enabled)
{
  if (!item) {
    return;
  }

  if (item->rtti() != AGENTSYMBOL_RTTI) {
    return;
  }

  QCheckListItem* citem = (QCheckListItem*)item;
  citem->setOn(enabled);

  // Check parent; parent() returns null if no parents left
  checkParents(citem->parent(), enabled);
}

void SystemView::selectedSystemItem(QListViewItem* item)
{
  if (!item) {
    return;
  }

#if 0
  if (item->rtti() == AGENTSYMBOL_RTTI) {
    QCheckListItem* citem = (QCheckListItem*)item;
    checkChildren(citem, citem->isOn());
    //    checkParents(citem, citem->isOn());
  }
#endif
}

void SystemView::returnFilterEdit()
{
  std::cout << "Filter string = " << filterstr_ << std::endl;

  if (corbaobj_) {
    CorbaEvent* event = new CorbaEvent(CorbaEvent::SetFilter);

    // Test with system filter
    Demon::FilterList* fl = new Demon::FilterList;
    fl->length(1);
    (*fl)[0].index = system_->index;
    (*fl)[0].enabled = FALSE;

    event->setFilter(fl);
    QApplication::postEvent(corbaobj_, event);
  }
}

