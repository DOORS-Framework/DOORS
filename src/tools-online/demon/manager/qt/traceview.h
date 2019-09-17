#ifndef TRACEVIEW_H
#define TRACEVIEW_H

#include "demon.h"
#include <qlistview.h>
#include <qscrollview.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qlist.h>
#include "agentevent.h"
#include <map>
#include <list>

// Data container class for task related information
class TraceTask {
 public:
  TraceTask() { } // Needed for std::map
  TraceTask(const Demon::Symbol* symbol, QLabel* label, int x) 
    : symbol_(symbol), label_(label), x_(x) { }
  const Demon::Symbol* getSymbol() { return symbol_; }
  int getX() { return x_; }
  QLabel* getLabel() { return label_; }
 private:
  const Demon::Symbol* symbol_;
  QLabel* label_;
  int x_;
};


class TraceView : public QScrollView
{
public:
  TraceView(QWidget* parent, QListView* listview, 
	    const char * name, const Demon::Symbol* system, WFlags f = 0);
  ~TraceView();

  void reTrace();

protected:
  void drawContents (QPainter * p, int clipx, int clipy, 
		     int clipw, int cliph);

private:
  // Data container class for messages related information
  class TraceMessage {
  public:
    TraceMessage() // This dummy is needed for std::list 
      : symbol_(0), listitem_(0), label_(0),
	srctask_(0), targtask_(0), 
	yoffset_(0), fromleft_(true) { }
    TraceMessage(const Demon::Symbol* symbol,
		 AgentEvent* listitem, QLabel* label,
		 TraceTask* srctask, 
		 TraceTask* targtask, 
		 int yoffset, bool fromleft = true)
      : symbol_(symbol), listitem_(listitem), label_(label),
	srctask_(srctask), targtask_(targtask), 
	yoffset_(yoffset), fromleft_(fromleft) { }
    const Demon::Symbol* getSymbol() { return symbol_; }
    AgentEvent* getAgentEvent() { return listitem_; }
    QLabel* getLabel() {return label_; }
    TraceTask* getSrcTask() { return srctask_; }
    TraceTask* getTargTask() { return targtask_; }
    int getY() { return yoffset_; }
    bool isFromLeft() { return fromleft_; }
    bool operator==(const TraceMessage& other) {
      if (symbol_ == other.symbol_) {
	return true;
      }
      return false;
    }
    bool operator<(const TraceMessage& other) {
      if (symbol_ < other.symbol_) {
	return true;
      }
      return false;
    }

  private:
    const Demon::Symbol* symbol_;
    AgentEvent* listitem_;
    QLabel* label_;
    TraceTask* srctask_;
    TraceTask* targtask_;
    int yoffset_;
    bool fromleft_;
  };

private:
  void addVertical(std::string name, int x, int y, int height);
  void searchTasks();
  void drawTrace();
  QListView* listview_;
  const Demon::Symbol* system_;
  std::map<std::string, TraceTask> tasks_;
  std::list<TraceMessage> messages_;
  QList<QLabel> labels_;
  int height_;
};

#endif

