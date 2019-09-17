#include "demon.h"
#include <qobjectlist.h>
#include <algorithm>
#include <qpainter.h>
#include <qcolor.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qwidget.h>
#include <qrect.h>
#include "agentevent.h"
#include "traceview.h"
#include "linewidget.h"
#include "messagelabel.h"

TraceView::TraceView(QWidget* parent, QListView* listview,
		     const char* name, const Demon::Symbol* system, WFlags f)
  : QScrollView(parent, name, f), listview_(listview), system_(system),
    height_(0)
{
  setFrameStyle( Panel | Sunken );
  setBackgroundMode( PaletteBase );
  setPaletteBackgroundColor(white);
  setPaletteForegroundColor(black);

  // Automatically delete the label list
  labels_.setAutoDelete(true);

  //  reTrace();
  searchTasks();
  drawTrace();
}

TraceView::~TraceView()
{
}

void
TraceView::reTrace()
{
  labels_.clear();

  searchTasks();
  drawTrace();

  viewport()->repaint();
}


void
TraceView::searchTasks()
{
  int x = 0;
  const int OFFSET = 40;
  const int DELTA = 150;
  const int YOFFSET = 80;

  tasks_.clear();

  x = OFFSET;
  for (QListViewItem* item = listview_->firstChild(); item; 
       item = item->nextSibling()) {
    AgentEvent* event = (AgentEvent*)item;
    const Demon::Symbol* symbol = event->getSymbol();
    switch(symbol->value._d()) {
    case Demon::stEventTask :
    case Demon::stEventTaskBefore :
    case Demon::stEventTaskAfter :
    case Demon::stPTask :
      if (!tasks_.count(std::string(symbol->name))) {
	// Not found in the list; add symbol name there
	// Create label
	QLabel* label = new QLabel((const char*)symbol->name, viewport());
	label->setAlignment(AlignCenter);
	label->adjustSize();
	label->show();
	addChild(label, x - label->width() / 2, YOFFSET);
	labels_.append(label);
	tasks_[std::string(symbol->name)] = TraceTask(symbol, label, x);
	x += DELTA;
      }
    }
  }

}

void
TraceView::addVertical(std::string name, int x, int y, int height)
{
  // Add line first
  std::string linename = name;
  linename += ".line";
  LineWidget* line = new LineWidget(viewport(), linename.c_str(), height);
  addChild(line, x - 1, y);

  // Then label
  QLabel* label = new QLabel(name.c_str(), viewport());
  label->setAlignment(AlignCenter);
  label->adjustSize();
  //  QRect rect = label->rect();
  addChild(label, x - label->width() / 2, y);
  labels_.append(label);
}

void 
TraceView::drawContents (QPainter * p, int clipx, int clipy, 
			 int clipw, int cliph)
{
  const int YOFFSET = 80;

  p->setPen(QPen(blue, 1));
  std::list<TraceMessage>::iterator itermsg;
  for (itermsg = messages_.begin(); itermsg != messages_.end(); itermsg++) {
    int xsrc = (*itermsg).getSrcTask()->getX();
    int xdest = (*itermsg).getTargTask()->getX();
    int y = (*itermsg).getY();
    p->moveTo(xsrc, y);
    p->lineTo(xdest, y);
    if ((*itermsg).isFromLeft()) {
      p->moveTo(xdest + 5, y - 5);
      p->lineTo(xdest, y);
      p->lineTo(xdest + 5, y + 5);
    } else {
      p->moveTo(xsrc - 5, y - 5);
      p->lineTo(xsrc, y);
      p->lineTo(xsrc - 5, y + 5);
    }
  }

  p->setPen(QPen(black, 2));
  std::map<std::string, TraceTask>::iterator itertask;
  for (itertask = tasks_.begin(); itertask != tasks_.end(); itertask++) {
    int x = (*itertask).second.getX();
    p->moveTo(x, YOFFSET);
    p->lineTo(x, YOFFSET + height_);
  }
}

void
TraceView::drawTrace()
{
  const int XOFFSET = 40;
  const int XDELTA = 150;
  const int YOFFSET = 80;
  const int YDELTA = 20;
  int height = YDELTA * tasks_.size();

  messages_.clear();

  std::map<std::string, TraceTask>::iterator iter;

  int yoffset = YOFFSET + YDELTA;
  int eventcount = 0;
  for (QListViewItem* item = listview_->firstChild(); item; 
       item = item->nextSibling()) {
    AgentEvent* event = (AgentEvent*)item;
    const Demon::Symbol* symbol = event->getSymbol();
    
    if (symbol->value._d() == Demon::stMessage) {
      std::string srcname = 
	(const char*)symbol->value.message().source.parentname;
      if (!tasks_.count(srcname)) {
	continue; // Not found in the task list; ignore this message
      }
      std::string targetname =
	(const char*)symbol->value.message().target.parentname;
      if (!tasks_.count(targetname)) {
	continue; // Not found in the task list; ignore this message
      }
      int srcxoffset = tasks_[srcname].getX();
      int targxoffset = tasks_[targetname].getX();
      bool fromleft = true;
      if (targxoffset < srcxoffset) {
	fromleft = false;
	// Swap offsets to set direction from left to right
	int tmp = srcxoffset;
	srcxoffset = targxoffset;
	targxoffset = tmp;
      }
      std::cout << srcname << ", " << targetname << ",";
      std::cout << srcxoffset << ", " << targxoffset << ",";
      std::cout << yoffset << std::endl;
#if 0
      // Draw line
      LineWidget* line = new LineWidget(viewport(), "line", 
					targxoffset - srcxoffset, false);
      addChild(line, srcxoffset - 1, yoffset + eventcount * YDELTA);
#endif
      MessageLabel* label = 
	new MessageLabel(QString(symbol->name), viewport());
      label->setAlignment(AlignCenter);
      label->adjustSize();
      label->show();
      // Connect label-clicked signal to the raiseMe() in AgentEvent
      QObject::connect(label, SIGNAL(clicked()), event, SLOT(raiseMe()));
      labels_.append(label);
      int labelx = (srcxoffset + targxoffset) / 2 - label->width() / 2;
      int labely = yoffset + eventcount * YDELTA - label->height();
      addChild(label, labelx, labely);
      messages_.push_back(TraceMessage(symbol, event, label, &tasks_[srcname],
				       &tasks_[targetname], labely + 2, 
				       fromleft));
    }

    eventcount++;
      
  }

  height_ = eventcount * YDELTA + YDELTA;
  // Draw vertical lines
  
  std::cout << "Items = " << eventcount << std::endl;

  resizeContents(XOFFSET + XDELTA * tasks_.size(), 1000);
		 /*		 YDELTA * eventcount + YOFFSET + YDELTA);*/

}

