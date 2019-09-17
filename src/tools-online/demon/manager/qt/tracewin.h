/****************************************************************************
** Form interface generated from reading ui file 'tracewin.ui'
**
** Created: Tue Nov 5 12:59:45 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef TRACEWIN_H
#define TRACEWIN_H

#include "demon.h"
#include <qlistview.h>
#include <qvariant.h>
#include <qdialog.h>
#include "traceview.h"

class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QPushButton;

class TraceWin : public QDialog
{ 
  Q_OBJECT

public:
  TraceWin( QListView* parent, const char* name, const Demon::Symbol* system, 
	    bool modal = FALSE, WFlags fl = 0 );
  ~TraceWin();
  
  TraceView* traceView;
  QPushButton* CloseButton;
  QPushButton* RefreshButton;

public slots:
  virtual void refreshButton();
  virtual void closeButton();

protected:
  QVBoxLayout* TraceWinLayout;
  QVBoxLayout* ScrollViewLayout;
  QHBoxLayout* ButtonLayout;

private:
  QListView* parent_;
};

#endif // TRACEWIN_H
