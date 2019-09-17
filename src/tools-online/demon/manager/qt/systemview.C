/****************************************************************************
** Form implementation generated from reading ui file 'systemview.ui'
**
** Created: Wed Nov 20 12:13:29 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "systemview.h"

#include <qvariant.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "systemview.ui.h"
#include "systemview.moc.h"

/* 
 *  Constructs a SystemView which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
SystemView::SystemView( const Demon::Symbol* system, QWidget* parent, 
			QObject* corbaobj, std::string agenturl)
  : QDialog( parent, "SystemView", false,  Qt::WStyle_Tool), system_(system), 
    symbol_(0), corbaobj_(corbaobj), agent_url_(agenturl)
{
    setName( "SystemView" );
    resize( 600, 480 ); 
    QString caption(trUtf8("System View - "));
    caption += agenturl.c_str();
    setCaption(caption);
    SystemViewLayout = new QVBoxLayout( this, 11, 6, "SystemViewLayout"); 

    Layout3 = new QHBoxLayout( 0, 0, 6, "Layout3"); 

    SystemList = new QListView( this, "SystemList" );
    SystemList->addColumn( trUtf8( "System" ) );

    Layout3->addWidget( SystemList );
    SystemViewLayout->addLayout( Layout3 );

    Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1"); 

#if 0
    FilterLabel = new QLabel( this, "FilterLabel" );
    FilterLabel->setText( trUtf8( "Filter" ) );
    Layout1->addWidget( FilterLabel );

    FilterEdit = new QLineEdit( this, "FilterEdit" );
    Layout1->addWidget( FilterEdit );
    SystemViewLayout->addLayout( Layout1 );
#endif

    Layout4 = new QHBoxLayout( 0, 0, 6, "Layout4"); 

    CloseButton = new QPushButton( this, "CloseButton" );
    CloseButton->setDefault(false);
    CloseButton->setText( trUtf8( "&Close" ) );
    Layout4->addWidget( CloseButton );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, 
					     QSizePolicy::Minimum );
    Layout4->addItem( spacer_2 );
    FilterButton = new QPushButton( this, "FilterButton" );
    FilterButton->setDefault(false);
    FilterButton->setText( trUtf8( "&Set Filter" ) );
    Layout4->addWidget( FilterButton );
    SystemViewLayout->addLayout( Layout4 );

    // signals and slots connections
#if 0
    connect( FilterEdit, SIGNAL( textChanged(const QString&) ), 
	     this, SLOT( changedFilterEdit(const QString&) ) );
    connect( FilterEdit, SIGNAL( returnPressed() ), 
    	     this, SLOT( returnFilterEdit() ) );
#endif
    connect( FilterButton, SIGNAL( clicked() ),
	     this, SLOT( clickedFilterButton() ) );
    connect( CloseButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( SystemList, SIGNAL( clicked(QListViewItem*) ), 
	     this, SLOT( selectedSystemItem(QListViewItem*) ) );

    // Show system in the system list view
    symbol_ = new AgentSymbolCL(system_, SystemList, 
				false/*don't show values*/);
}

/*  
 *  Destroys the object and frees any allocated resources
 */
SystemView::~SystemView()
{
    // no need to delete child widgets, Qt does it all for us
}

void 
SystemView::closeEvent(QCloseEvent* event)
{
  // Hide; don't close
  hide();
  // Just ignore the close event coming from the windowing system.
  event->ignore();
}

void
SystemView::keyPressEvent(QKeyEvent* event)
{
  switch(event->key()) {
  case Key_Return :
    // Ignore newline key because we don't want our dialog to
    // close now.
    event->ignore();
    break;
  default :
    // Handle all the other keys in the parent method.
    QDialog::keyPressEvent(event);
    break;
  }
}

