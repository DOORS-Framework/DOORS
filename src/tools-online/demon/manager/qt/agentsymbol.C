#include <stdio.h>
#include <netinet/in.h>
#include "agentsymbol.h"
#include "managericons.h"

AgentSymbol::AgentSymbol(const Demon::Symbol* symbol, 
				     QListView* parent,
				     bool showvalues)
  : symbol_(symbol), QListViewItem(parent, ""),
    folderOpen(0), folderClosed(0), showvalues_(showvalues)
{
  initialise();
}

AgentSymbol::AgentSymbol(const Demon::Symbol* symbol, 
				     QListViewItem* parent,
				     bool showvalues)
  : symbol_(symbol), QListViewItem(parent, ""),
    folderOpen(0), folderClosed(0), showvalues_(showvalues)
{
  initialise();
}

AgentSymbolCL::AgentSymbolCL(const Demon::Symbol* symbol, 
				     QListView* parent,
				     bool showvalues)
  : symbol_(symbol), QCheckListItem(parent, "", QCheckListItem::CheckBox),
    folderOpen(0), folderClosed(0), showvalues_(showvalues)
{
  initialise();
}

AgentSymbolCL::AgentSymbolCL(const Demon::Symbol* symbol, 
				     QListViewItem* parent,
				     bool showvalues)
  : symbol_(symbol), QCheckListItem(parent, "", QCheckListItem::CheckBox),
    folderOpen(0), folderClosed(0), showvalues_(showvalues)
{
  initialise();
}

AgentSymbol::~AgentSymbol()
{
  delete folderOpen;
  delete folderClosed;
}

AgentSymbolCL::~AgentSymbolCL()
{
  delete folderOpen;
  delete folderClosed;
}

void 
AgentSymbol::setOpen (bool o)
{
  if (o) {
    setPixmap(0, *folderOpen);
  } else {
    setPixmap(0, *folderClosed);
  }
  QListViewItem::setOpen(o);
}

void 
AgentSymbolCL::setOpen (bool o)
{
  if (o) {
    setPixmap(0, *folderOpen);
  } else {
    setPixmap(0, *folderClosed);
  }
  QListViewItem::setOpen(o);
}

void
AgentSymbolCL::initialise()
{
  folderOpen = new QPixmap(folder_open);
  folderClosed = new QPixmap(folder);

  // Initially closed
  setPixmap(0, *folderClosed);

  // Set type name as root of the list item
  std::string tname = typeName(symbol_);
  setText(0, QString(tname.c_str()));

  // Show attributes according to type
  show_attributes(symbol_, showvalues_);

  // Recurse to children
  for (int i = 0; i < symbol_->children.length(); i++) {
    const Demon::Symbol* child = &symbol_->children[i];
    (void)new AgentSymbolCL(child, this, showvalues_);
  }

  setOpen(true);
  setOn(true);
}

void
AgentSymbol::initialise()
{
  folderOpen = new QPixmap(folder_open);
  folderClosed = new QPixmap(folder);

  // Initially closed
  setPixmap(0, *folderClosed);

  // Set type name as root of the list item
  std::string tname = typeName(symbol_);
  setText(0, QString(tname.c_str()));

  // Show attributes according to type
  show_attributes(symbol_, showvalues_);

  // Recurse to children
  for (int i = 0; i < symbol_->children.length(); i++) {
    const Demon::Symbol* child = &symbol_->children[i];
    (void)new AgentSymbol(child, this, showvalues_);
  }

  setOpen(true);
}

void 
AgentSymbol::showAttribute(QString& str)
{ 
  (void)new QListViewItem(this, str); 
}

void 
AgentSymbolCL::showAttribute(QString& str)
{
#if 0 
  QCheckListItem* item = 
    new QCheckListItem(this, str, QCheckListItem::CheckBox); 
  if (item) {
    item->setOn(true);
  }
#endif
  (void)new QListViewItem(this, str); 
}

  
