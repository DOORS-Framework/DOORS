#ifndef MESSAGELABEL_H
#define MESSAGELABEL_H

#include <qlabel.h>
#include <qobject.h>
#include <qstring.h>
#include <qwidget.h>

class MessageLabel : public QLabel
{
  Q_OBJECT

public:
  MessageLabel(const QString& text, QWidget* parent);
  void mouseReleaseEvent(QMouseEvent* e);

signals:
  void clicked();
};

#endif
