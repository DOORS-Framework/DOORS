/****************************************************************************
** Form interface generated from reading ui file 'systemview.ui'
**
** Created: Wed Nov 20 12:13:26 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef SYSTEMVIEW_H
#define SYSTEMVIEW_H

#include "demon.h"
#include <string>
#include <qobject.h>
#include <qvariant.h>
#include <qdialog.h>
#include <qstring.h>
#include <qlistview.h>
#include "agentsymbol.h"

class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class QLineEdit;
class QListView;
class QListViewItem;
class QPushButton;

class SystemView : public QDialog
{ 
    Q_OBJECT

public:
    SystemView( const Demon::Symbol* system, QWidget* parent, 
		QObject* corbaobj, std::string agenturl);
    ~SystemView();

    QListView* SystemList;
    QPushButton* FilterButton;
    QLabel* FilterLabel;
    QLineEdit* FilterEdit;
    QPushButton* CloseButton;

public slots:
    virtual void changedFilterEdit(const QString& str);
    virtual void clickedFilterButton();
    virtual void selectedSystemItem(QListViewItem* item);
    virtual void returnFilterEdit();

protected:
    void closeEvent(QCloseEvent* event);
    void keyPressEvent(QKeyEvent* event);

protected:
    QVBoxLayout* SystemViewLayout;
    QHBoxLayout* Layout3;
    QVBoxLayout* Layout2;
    QHBoxLayout* Layout1;
    QHBoxLayout* Layout4;

private:
    void checkChildren(QCheckListItem* item, bool enabled = true);
    void checkParents(QListViewItem* item, bool enabled = true);
    void setFilter(Demon::Filter& filter, AgentSymbolCL* item);
    AgentSymbolCL* symbol_;
    const Demon::Symbol* system_;
    QString filterstr_;
    QObject* corbaobj_;
    std::string agent_url_;
};

#endif // SYSTEMVIEW_H
