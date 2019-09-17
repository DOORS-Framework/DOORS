/****************************************************************************
** Form implementation generated from reading ui file 'managermain.ui'
**
** Created: su syys   29 17:50:53 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include <iostream>

#include "demon.h"
#include "managermain.h"
#include "mgrslpclnt.h"
#include "slpevent.h"
#include "corbaevent.h"

#include <qvariant.h>
#include <qheader.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qmime.h>
#include <qdragobject.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <qkeysequence.h>
#include <qaccel.h>

#include "managermain.ui.h"
#include "managermain.moc.h"

/* 
 *  Constructs a ManagerMain which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 */
ManagerMain::ManagerMain( QWidget* parent,  const char* name, WFlags fl )
  : QMainWindow( parent, name, fl )
{
    (void)statusBar();
    if ( !name )
	setName( "ManagerMain" );
    resize( 600, 480 ); 
    QFont f( font() );
    f.setFamily( "Helvetica [Adobe]" );
    setFont( f ); 
    setCaption( trUtf8( "DEMon Manager" ) );
    setCentralWidget( new QWidget( this, "qt_central_widget" ) );
    ManagerMainLayout = new QVBoxLayout( centralWidget(), 11, 6, 
					 "ManagerMainLayout"); 

    VerticalLayout = new QVBoxLayout( 0, 0, 6, "VerticalLayout"); 

    agentList = new AgentList( centralWidget(), "AgentList" );
    VerticalLayout->addWidget( agentList );

    HorizontalLayout = new QHBoxLayout( 0, 0, 6, "HorizontalLayout"); 

    SelectButton = new QPushButton( centralWidget(), "SelectButton" );
    SelectButton->setText( trUtf8( "&Select" ) );
    HorizontalLayout->addWidget( SelectButton );

    // Create SLP client under update-button and make agentList to receive
    // its events
    UpdateButton = new MgrSlpClnt(agentList, centralWidget(), "UpdateButton");
    UpdateButton->setText( trUtf8( "&Update" ) );
    HorizontalLayout->addWidget( UpdateButton );

    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, 
					   QSizePolicy::Minimum );
    HorizontalLayout->addItem( spacer );
    VerticalLayout->addLayout( HorizontalLayout );
    ManagerMainLayout->addLayout( VerticalLayout );

    // actions
    fileExitAction = new QAction( this, "fileExitAction" );
    fileExitAction->setText( trUtf8( "Exit" ) );
    fileExitAction->setMenuText( trUtf8( "E&xit" ) );
    fileExitAction->setAccel(QKeySequence(trUtf8("Ctrl+X", "File|Exit")));
    helpAboutAction = new QAction( this, "helpAboutAction" );
    helpAboutAction->setText( trUtf8( "About" ) );
    helpAboutAction->setMenuText( trUtf8( "&About..." ) );
    helpAboutAction->setAccel(QKeySequence(trUtf8("Ctrl+A", "Help|About")));

    // menubar
    menubar = new QMenuBar( this, "menubar" );

    fileMenu = new QPopupMenu( this ); 
    fileExitAction->addTo( fileMenu );
    menubar->insertItem( trUtf8( "&File" ), fileMenu );

    helpMenu = new QPopupMenu( this ); 
    helpAboutAction->addTo( helpMenu );
    menubar->insertItem( trUtf8( "&Help" ), helpMenu );

    // Connect some keys to some slots
    QAccel *a = new QAccel(this);
    a->connectItem(a->insertItem(Key_Space),
    		   SelectButton, SLOT(animateClick()));     
    a->connectItem(a->insertItem(Key_U), UpdateButton, SLOT(animateClick()));
    a->connectItem(a->insertItem(Key_S), SelectButton, SLOT(animateClick()));

    // signals and slots connections
    connect(fileExitAction, SIGNAL(activated()), this, SLOT(close()));
    connect(helpAboutAction, SIGNAL(activated()), this, SLOT( helpAbout()));
    connect(SelectButton, SIGNAL(pressed()), SelectButton, SLOT(toggle()));
    connect(SelectButton, SIGNAL(clicked()), this, SLOT(selectSlot()));

}

/*  
 *  Destroys the object and frees any allocated resources
 */
ManagerMain::~ManagerMain()
{

    // no need to delete child widgets, Qt does it all for us
}

 
