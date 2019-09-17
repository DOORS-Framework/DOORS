/****************************************************************************
** Form implementation generated from reading ui file 'agentview.ui'
**
** Created: su syys   29 17:50:53 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include <CORBA.h>
#include <string>
#include "demon.h"
#include "agentwin.h"
#include "corbaevent.h"
#include "agentevent.h"
#include <qvariant.h>
#include <qheader.h>
#include <qpushbutton.h>
#include <qscrollbar.h>
#include <qsplitter.h>
#include <qmime.h>
#include <qdragobject.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qapplication.h>
#include <qfont.h>
#include <qaccel.h>

#include "agentwin.ui.h"
#include "agentwin.moc.h"

static QPixmap uic_load_pixmap_AgentWin( const QString &name )
{
    const QMimeSource *m = QMimeSourceFactory::defaultFactory()->data( name );
    if ( !m )
	return QPixmap();
    QPixmap pix;
    QImageDrag::decode( m, pix );
    return pix;
}
/* 
 *  Constructs a AgentWin which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
AgentWin::AgentWin(AgentListItem* listitem, QWidget* parent, std::string ior,
		   std::string agenturl)
  : QDialog( parent, "AgentWin", FALSE, 0 ), agent_ior_(ior),
    corbasrvr_(0), corbaclnt_(0), listitem_(listitem), system_(0),
    systemview_(0), agent_url_(agenturl)
{
    resize( 600, 480 ); 
    QString caption(trUtf8("Agent Window - "));
    caption += agenturl.c_str();
    setCaption(caption);

    AgentWinLayout = new QVBoxLayout( this, 11, 6, "AgentWinLayout"); 

    Layout8 = new QVBoxLayout( 0, 0, 6, "Layout8"); 

    Splitter = new QSplitter( this, "Splitter" );
    Splitter->setOrientation( QSplitter::Vertical );

    QWidget* privateLayoutWidget = new QWidget( Splitter, "Layout5" );
    Layout5 = new QHBoxLayout( privateLayoutWidget, 0, 6, "Layout5"); 

    AgentEventList = new QListView( privateLayoutWidget, "AgentEventList" );
    AgentEventList->addColumn( trUtf8( "ID" ) );
    AgentEventList->addColumn( trUtf8( "Time" ) );
    AgentEventList->addColumn( trUtf8( "Event name" ) );
    AgentEventList->addColumn( trUtf8( "Event type" ) );
    AgentEventList->setSorting(1); // Sort according to time
    Layout5->addWidget( AgentEventList );

    QWidget* privateLayoutWidget_2 = new QWidget( Splitter, "Layout6" );
    Layout6 = new QHBoxLayout( privateLayoutWidget_2, 0, 6, "Layout6"); 

    AgentSymbolList = new QListView( privateLayoutWidget_2, "AgentSymbolList" );
    AgentSymbolList->addColumn( trUtf8( "Symbol" ) );
    Layout6->addWidget( AgentSymbolList );

    Layout8->addWidget( Splitter );

    Layout7 = new QHBoxLayout( 0, 0, 6, "Layout7"); 

    Select = new QPushButton( this, "Select" );
    Select->setText( trUtf8( "&Select" ) );
    Layout7->addWidget( Select );
    Trace = new QPushButton( this, "Trace" );
    Trace->setText( trUtf8( "&Trace" ) );
    Layout7->addWidget( Trace );
    SystemButton = new QPushButton( this, "SystemButton" );
    SystemButton->setText( trUtf8( "Sh&ow System" ) );
    Layout7->addWidget( SystemButton );

    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, 
					   QSizePolicy::Minimum );
    Layout7->addItem( spacer );
    Close = new QPushButton( this, "Close" );
    Close->setText( trUtf8( "&Close" ) );
    Layout7->addWidget( Close );
    Layout8->addLayout( Layout7 );
    AgentWinLayout->addLayout( Layout8 );

    // signals and slots connections
    connect( AgentEventList, SIGNAL(selectionChanged(QListViewItem*)),
	     SLOT(selchangedAgentList(QListViewItem*)));
    connect( Select, SIGNAL( clicked() ), this, SLOT( selectButton() ) );
    connect( Trace, SIGNAL( clicked() ), this, SLOT( traceButton() ) );
    connect( Close, SIGNAL( clicked() ), this, SLOT( closeButton() ) );
    //    connect( Close, SIGNAL( clicked() ), this, SLOT( close() ) );
    //    connect( Select, SIGNAL( pressed() ), Select, SLOT( toggle() ) );
    connect( AgentEventList, SIGNAL( clicked(QListViewItem*,const QPoint&,int) ),
	     this, SLOT( clickedAgentList(QListViewItem*,const QPoint&,int) ));
    //    connect( AgentEventList, SIGNAL( doubleClicked(QListViewItem*) ), 
    //	     this, SLOT( doubleclickedAgentList(QListViewItem* item) ) );
    connect( AgentSymbolList, SIGNAL( clicked(QListViewItem*,const QPoint&,int) ),
	     this, SLOT( clickedSymbolist(QListViewItem*,const QPoint&,int) ));
    //    connect( AgentSymbolList, SIGNAL( doubleClicked(QListViewItem*) ), 
    //	     this, SLOT( doubleclickedSymbolist(QListViewItem* item) ) );
    connect(SystemButton, SIGNAL(clicked()), this, SLOT(systemSlot()));

    // Connect some keys to some slots
    QAccel *a = new QAccel(this);
    a->connectItem(a->insertItem(Key_Space),
    		   Select, SLOT(animateClick()));     
    //    a->connectItem(a->insertItem(Key_Space),
    //		   Select, SLOT(toggle()));
    a->connectItem(a->insertItem(Key_S), Select, SLOT(animateClick()));
    a->connectItem(a->insertItem(Key_T), Trace, SLOT(animateClick()));
    a->connectItem(a->insertItem(Key_C), Close, SLOT(animateClick()));
    a->connectItem(a->insertItem(Key_O), SystemButton, SLOT(animateClick()));

    // Give us CORBA
    corbasrvr_ = new MgrCorbaSrvr(this, listitem);
    if (corbasrvr_) {
      CORBA::Object_ptr agent_obj = corbasrvr_->stringToObject(agent_ior_);
      if (agent_obj) {
	corbaclnt_ = new MgrCorbaClnt(agent_obj);
      }
    }

    if (corbaclnt_) {
      CorbaEvent* event = new CorbaEvent(CorbaEvent::SetManager);
      event->setManager(1, corbasrvr_->getIOR());
      QApplication::postEvent(corbaclnt_, event);
      std::cout << "SetManager posted." << std::endl;
    }

}

/*  
 *  Destroys the object and frees any allocated resources
 */
AgentWin::~AgentWin()
{
  // Send disconnect() to agent as our last effort
  if (corbaclnt_) {
    (void)corbaclnt_->disconnect();
  }

  delete corbasrvr_;
  delete corbaclnt_;

    // no need to delete child widgets, Qt does it all for us
}

void
AgentWin::addSymbol(CORBA::ULong event_id,
		    CORBA::Long handle, Demon::TimeStamp timestamp, 
		    const Demon::Symbol* symbol)
{
  AgentEvent* event = new AgentEvent(event_id, handle, timestamp,
				     symbol, AgentEventList);

  std::cout << "addSymbol" << std::endl;
}

void 
AgentWin::closeEvent(QCloseEvent* event)
{
  // Hide; don't close
  hide();
  // Just ignore the close event coming from the windowing system.
  event->ignore();
}

void 
AgentWin::customEvent(QCustomEvent* event)
{
  if (event->type() == QEVENT_CORBA) {
    CorbaEvent* corbaevent = (CorbaEvent*)event;
    if (corbaevent->eventtype() == CorbaEvent::SendSymbol) {
      addSymbol(corbaevent->getEventId(),
		corbaevent->getHandle(),
		corbaevent->getTimeStamp(),
		corbaevent->getSymbol());
    } else if (corbaevent->eventtype() == CorbaEvent::SetSystem) {
      system_ = corbaevent->getSystem();
    }
  }
}
