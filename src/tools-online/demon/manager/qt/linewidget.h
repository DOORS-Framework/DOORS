#ifndef LINEWIDGET_H
#define LINEWIDGET_H

#include <qwidget.h>
#include <qpainter.h>

class LineWidget : public QWidget
{
public:
  LineWidget(QWidget* parent, const char* name, int length, 
	     bool vertical = true, WFlags f = 0)
    : QWidget(parent, name, f), length_(), vertical_(vertical) { 
    if (vertical) {
      setFixedSize(3, length);
    } else {
      setFixedSize(length, 3);
    }
  }

protected:
  virtual void paintEvent (QPaintEvent* pe) {
    QPainter paint(this);
    paint.setPen(QPen(black, 3));
    if (vertical_) {
      paint.moveTo(1, 0);
      paint.lineTo(1, length_);
    } else {
      paint.moveTo(0, 1);
      paint.lineTo(length_, 1);
    }
  }

private:
  int length_;
  bool vertical_;
};
   
#endif
