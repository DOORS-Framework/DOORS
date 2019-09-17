#ifndef AGENTSYMBOL_H
#define AGENTSYMBOL_H

#include <string>
#include <qpixmap.h>
#include <qlistview.h>
#include "demon.h"
#include "symboltools.h"

#define AGENTSYMBOL_RTTI 3

//TODO: AgentSymbol and AgentSymbolCL are almost identical.
//      Fix this someday.

// Symbol with check list items
class AgentSymbolCL : public QCheckListItem, public SymbolTools
{
public:
  AgentSymbolCL(const Demon::Symbol* symbol, QListView* parent,
		bool showvalues = true);
  AgentSymbolCL(const Demon::Symbol* symbol, QListViewItem* parent,
		bool showvalues = true);
  ~AgentSymbolCL();

protected:
  virtual void showAttribute(QString& str); 

private:
  void initialise();

public:
  // Inherited from QListViewItem
  virtual void setOpen (bool o);
  virtual int rtti() const { return AGENTSYMBOL_RTTI; }

public:
  const Demon::Symbol* getSymbol() { return symbol_; }

private:
  bool showvalues_;
  const Demon::Symbol* symbol_;
  QPixmap* folderOpen;
  QPixmap* folderClosed;
};

// Symbol without check list items
class AgentSymbol : public QListViewItem, public SymbolTools
{
public:
  AgentSymbol(const Demon::Symbol* symbol, QListView* parent,
	      bool showvalues = true);
  AgentSymbol(const Demon::Symbol* symbol, QListViewItem* parent,
	      bool showvalues = true);
  ~AgentSymbol();

protected:
  virtual void showAttribute(QString& str); 

private:
  void initialise();

public:
  // Inherited from QListViewItem
  virtual void setOpen (bool o);
  virtual int rtti() const { return AGENTSYMBOL_RTTI; }

public:
  const Demon::Symbol* getSymbol() { return symbol_; }

private:
  bool showvalues_;
  const Demon::Symbol* symbol_;
  QPixmap* folderOpen;
  QPixmap* folderClosed;
};


#endif
