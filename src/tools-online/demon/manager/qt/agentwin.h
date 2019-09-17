/****************************************************************************
** Form interface generated from reading ui file 'agentview.ui'
**
** Created: su syys   29 17:50:53 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef AGENTWIN_H
#define AGENTWIN_H

#include <CORBA.h>
#include <string>
#include "demon.h"
#include <qvariant.h>
#include <qdialog.h>

#include "agentlistitem.h"
#include "mgrcorbasrvr.h"
#include "mgrcorbaclnt.h"
#include "systemview.h"

class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QListView;
class QListViewItem;
class QPushButton;
class QScrollBar;
class QSplitter;

class AgentWin : public QDialog
{ 
    Q_OBJECT

public:
    AgentWin(AgentListItem* listitem, QWidget* parent, std::string ior,
	     std::string agenturl);
    ~AgentWin();

    QSplitter* Splitter;
    QListView* AgentEventList;
    QListView* AgentSymbolList;
    QPushButton* Select;
    QPushButton* Trace;
    QPushButton* Close;
    QPushButton* SystemButton;

public slots:
    virtual void traceButton();
    virtual void selectButton();
    virtual void closeButton();
    virtual void clickedAgentList(QListViewItem* list, const QPoint& point,
				  int row);
    virtual void doubleclickedAgentList(QListViewItem* item);
    virtual void clickedSymbolist(QListViewItem* list, const QPoint& point,
				  int row);
    virtual void doubleclickedSymbolist(QListViewItem* item);
    virtual void selchangedAgentList(QListViewItem* item);
    virtual void systemSlot();

protected:
    QVBoxLayout* AgentWinLayout;
    QVBoxLayout* Layout8;
    QHBoxLayout* Layout5;
    QHBoxLayout* Layout6;
    QHBoxLayout* Layout7;
    virtual void customEvent(QCustomEvent* event);

protected:
    void closeEvent(QCloseEvent* event);
  
private:
    MgrCorbaSrvr* corbasrvr_;
    MgrCorbaClnt* corbaclnt_;
    std::string agent_ior_;
    std::string agent_url_;
    AgentListItem* listitem_;
    const Demon::Symbol* system_;

private:
    void raiseAgentSymbol(QListViewItem* list);
    void addSymbol(CORBA::ULong event_id, CORBA::Long handle, 
		   Demon::TimeStamp timestamp, const Demon::Symbol* symbol);

private:
    SystemView* systemview_;
};

#endif // AGENTWIN_H
