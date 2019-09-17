#include <qapplication.h>
#include "managermain.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );

    ManagerMain *w = new ManagerMain;
    w->show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
