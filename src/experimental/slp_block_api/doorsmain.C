

#include <pthread.h>
#include <iostream>
#include "uaplugin.h"
#include <doors/uasap.h>
#include <doors/uaent.h>
#include <doors/uaentsm.h>
#include <doors/udp6task.h>
#include "uapluginsm.h"


UAPlugin *ua_plugin = 0;
UAPluginSM plugin_sm;
extern pthread_t doors_tid;


void *doors_thread_run( void *arg ) {

    Scheduler *sched = (Scheduler*)arg;

    while( 1 ) {
        sched->run ();
    }

    std::cout << "Oooops!!" << std::endl;
}

// TODO: exiting thread
void doors_main( ) {

    // create input-output handler for tasks
    IoHandler *io = IoHandler :: Instance ();

    Scheduler *sched = Scheduler :: Instance();
    // create task for managing timers
    TimerTask *tt  = TimerTask :: instance (sched, io);

    UAConfigurator *myconfig = UAConfigurator::Init (getenv("UACONFIG"));

    std::string da_group;

    switch (myconfig->slp_IPv6Scope) {
    case 2:
      da_group = "ff02::123";
      break;
    case 5:
      da_group = "ff05::123";
      break;
    default:
      da_group = "ff01::123";
      break;
    }
    InetAddr6 DA_Address (myconfig->slp_SlpPort, da_group);
    InetAddr6 ownAddr (myconfig->slp_SlpPort, getenv("HOST6"));

#ifdef sparc
    Udp6Task *udp = new Udp6Task( "udp", sched, io, &DA_Address );
#else 
    Udp6Task *udp = new Udp6Task( "udp", sched, io, &ownAddr );
#endif
    
    UAEntSM *ua_sm = new UAEntSM;
    UAEnt *uaent = new UAEnt( "ua-entity", sched, ua_sm );

    ua_plugin = UAPlugin :: Instance();

    ua_plugin->slp_sap.connect( &uaent->up );

    udp->up.connect(&uaent->down);

    int res = pthread_create( &doors_tid, 0, doors_thread_run, (void*)sched );

    if( res ) {
        std::cout << "Error! pthread_create( &doors_tid,0, doors_main,0 ) returned " << res << std::endl;
    }

    signal( SIGUSR1, Scheduler :: signal_handler_empty );

    sigset_t sigset;

    sigaddset( &sigset, SIGUSR1 );

    res = sigprocmask( SIG_SETMASK, &sigset, 0);

}
