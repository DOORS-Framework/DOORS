/****************************************************************************
** Form implementation generated from reading ui file 'tracewin.ui'
**
** Created: Tue Nov 5 13:00:15 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "tracewin.h"

#include <qvariant.h>
#include <qheader.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qaccel.h>

#include "tracewin.ui.h"
#include "tracewin.moc.h"

/* 
 *  Constructs a TraceWin which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
TraceWin::TraceWin( QListView* parent, const char* name, 
		    const Demon::Symbol* system, bool modal, WFlags fl )
  : QDialog( (QWidget*)parent, name, modal, fl ), parent_(parent)
{
    if ( !name )
	setName( "TraceWin" );
    resize( 600, 480 ); 
    setCaption( trUtf8( "Trace Window" ) );
    TraceWinLayout = new QVBoxLayout( this, 11, 6, "TraceWinLayout"); 

    ScrollViewLayout = new QVBoxLayout( 0, 0, 6, "ScrollViewLayout"); 

    traceView = new TraceView( this, parent, "TraceView", system );
    ScrollViewLayout->addWidget( traceView );

    ButtonLayout = new QHBoxLayout( 0, 0, 6, "ButtonLayout"); 

    // Refresh button
    RefreshButton = new QPushButton( this, "RefreshButton" );
    RefreshButton->setText( trUtf8( "&Refresh" ) );
    ButtonLayout->addWidget( RefreshButton );

    // Spacer between buttons
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, 
					   QSizePolicy::Minimum );
    ButtonLayout->addItem( spacer );

    // Close button
    CloseButton = new QPushButton( this, "CloseButton" );
    CloseButton->setText( trUtf8( "&Close" ) );
    ButtonLayout->addWidget( CloseButton );

    ScrollViewLayout->addLayout( ButtonLayout );
    TraceWinLayout->addLayout( ScrollViewLayout );

    // Connect some keys to some slots
    QAccel *a = new QAccel(this);
    a->connectItem(a->insertItem(Key_Space),
    		   RefreshButton, SLOT(animateClick()));     
    a->connectItem(a->insertItem(Key_C), CloseButton, SLOT(animateClick()));
    a->connectItem(a->insertItem(Key_R), RefreshButton, SLOT(animateClick()));

    // signals and slots connections
    connect( CloseButton, SIGNAL( clicked() ), 
	     this, SLOT(closeButton()) );
    connect( RefreshButton, SIGNAL( clicked() ), 
	     this, SLOT(refreshButton() ) );

}

/*  
 *  Destroys the object and frees any allocated resources
 */
TraceWin::~TraceWin()
{
    // no need to delete child widgets, Qt does it all for us
}



