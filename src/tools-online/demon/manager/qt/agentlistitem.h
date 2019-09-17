#ifndef AGENTLISTITEM_H
#define AGENTLISTITEM_H

#include <qpixmap.h>
#include <qobject.h>
#include <qlistview.h>
#include <string>

class AgentWin;

class AgentListItem : public QObject, public QListViewItem
{
  Q_OBJECT
   
public:
  AgentListItem(std::string agent, std::string ior, QListView* parent);
  ~AgentListItem();
  
  void setDisconnected();
  void setConnected();
  void setWindow(AgentWin* window) { window_ = window; }
  void showWindow();
  AgentWin* getWindow() { return window_; }
  
  QListView* getParent() { return parent_; }
  std::string getAgent() { return agent_; }
  std::string getIOR() { return ior_; }
  unsigned long getID() { return id_; }

protected:
  virtual void customEvent(QCustomEvent* event);

private:
  QPixmap* redBall;
  QPixmap* greenBall;
  std::string agent_;
  std::string ior_;
  QListView* parent_;
  AgentWin* window_;
  unsigned long id_;
  static unsigned long idpool_;
};

#endif
