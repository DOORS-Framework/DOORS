#ifndef AGENTLIST_H
#define AGENTLIST_H

#include <string>
#include <qobject.h>
#include <qwidget.h>
#include <qlistview.h>
#include <qevent.h>
#include <doors/agentwin.h>

class AgentList : public QListView
{
  Q_OBJECT

 public:
  AgentList(QWidget* parent = 0, const char* name = 0, WFlags f = 0);
  ~AgentList();

 public slots:
  virtual void listClicked(QListViewItem* item, 
			   const QPoint& point,
			   int n);
  virtual void listDoubleclicked(QListViewItem* item);

 protected:
  virtual void customEvent(QCustomEvent* event);

 private:
  void addAgent(std::string agent, std::string ior);

 private:
  QObject* manager_;

};

#endif

