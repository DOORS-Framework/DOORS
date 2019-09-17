#ifndef MGRSLPCLNT_H
#define MGRSLPCLNT_H

#include <qwidget.h>
#include <qpushbutton.h>
#include <qthread.h>

class MgrSlpClnt : public QPushButton, public QThread 
{
  Q_OBJECT

 public:
  MgrSlpClnt(QObject *sigrec, QWidget *parent = 0, const char *name = 0);
  ~MgrSlpClnt();

 public slots:
  void slotClicked();
 
 protected:
  void run();

 private:
  QObject *sigrec_;
};

#endif
