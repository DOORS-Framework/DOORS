
//  Copyright 2004
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.

//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
//  THIS SOFTWARE IS PROVIDED BY TAMPERE UNIVERSITY OF TECHNOLOGY AND
//  CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
//  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//  FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY
//  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/*

$Log: mobmodule.C,v $
Revision 1.3  2004/07/16 11:42:40  hartmanj
pthread_signalling to wakeup DOORS IoHandler from callback

Revision 1.2  2004/05/21 06:06:54  hartmanj
Reference counting to keep track of interested Ports. Validating Ports when registering and removing them from Mux.

Revision 1.1  2004/05/19 13:52:01  hartmanj
Initial revision

  
*/

#include "mobmodule.h"
#include <doors/debug.h>
#include <doors/handler.h>
#include <doors/portmux.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define USE_MOBAPI 1


MobilityModule* MobilityModule :: _instance = 0;

int MobilityModule::mark_mobapi_callback(mobile_node_t mn, int event, long int par)
{
    if( event == MIP_MN_MOVED ) {

        MobilitySAP :: Movement *doors_msg = new MobilitySAP :: Movement;

        struct sockaddr_in6 saddr;
        saddr.sin6_port = 0;
        saddr.sin6_addr = mn.co_addr;

        doors_msg->coa = InetAddr6( &saddr, sizeof(saddr));

        saddr.sin6_addr = mn.home_addr;
        doors_msg->ha = InetAddr6( &saddr, sizeof(saddr));

        _DOORS_DEBUG(0, "[callback]: coa is " << doors_msg->coa.getHostAddr());

        doors_msg->setMultipleTargets();
        _instance->listeners.putMessage(doors_msg);

        int res = pthread_kill( _instance->doors_main_thread_id, SIGUSR1 );

    } else {

        _DOORS_DEBUG(0,"[callback]: Other event (" << event << ")");

    }

    return 0;
}

static bool isForListener( const EventTask *t, const Message *m)
{
    if( t && m && m->isForMany() )
        return true;

    return false;
}

MobilityModule::MobilityModule( )
    :  movement_count(0), listeners(0),
       listenerMux (new SinglePropagator (new PortList), new MultiPropagator (new PortList)),
       ref_count(0)
{
    _DOORS_DEBUG(0, "MobilityModule created");

#ifdef USE_MOBAPI

    listeners.connect( listenerMux.createUser() );

    mn.home_addr = in6addr_any;
    int ret = mip_notify_movement( &mn, 0, 0, getpid(), 0 );

    if( ret == -1 ) {
        _DOORS_ERROR(0, "Voi voi ..mobility api not working");
    }

    struct sockaddr_in6 saddr;
    saddr.sin6_port = 0;
    saddr.sin6_addr = mn.co_addr;
    current_coa_address = InetAddr6( &saddr, sizeof(saddr) );

    saddr.sin6_addr = mn.home_addr;
    home_address = InetAddr6( &saddr, sizeof(saddr) );

    mn.home_addr = in6addr_any;
    ret = mip_notify_movement( &mn, 1, 0, getpid(), &mark_mobapi_callback );

    if( ret == -1 ) {
        _DOORS_ERROR(0, "Voi voi ..mobility api not working");
    }

    _DOORS_DEBUG(0, "Home address is " << home_address.getHostAddr());
    _DOORS_DEBUG(0, "Care-of address is " << current_coa_address.getHostAddr());
#else
    _DOORS_DEBUG(0, "MobAPI not is use");
#endif

    doors_main_thread_id = pthread_self();
    IoHandler :: Instance() -> enableWakeups();

    STATIC_SYMBOL(movement_count, this);
    STATIC_SYMBOL(current_coa_address, this);
    STATIC_SYMBOL(home_address, this);
}


MobilityModule::~MobilityModule()
{
    std::cout << "MobilityModule::~MobilityModule" << std::endl;
    mip_notify_movement( &mn, 1, 0, getpid(), NULL );
    DELETE_SYMBOL(this);
}

bool MobilityModule :: RemoveListener( Port *listener_port )
{
    _DOORS_DEBUG(0, "MobilityModule :: RemoveListener");

    if( _instance == 0 ) {
        _DOORS_DEBUG(0, "MobilityModule :: RemoveListener: No _instance!");
        return false;
    }

    if( !_instance->validatePort( listener_port ) )
        return false;

    if(listener_port->disconnect() == false ) {
        return false;
    }

    if( _instance->ref_count > 0 ) {
        _instance->ref_count--;
        _DOORS_DEBUG(0, "ref_count == " << _instance->ref_count);

        if( _instance->ref_count == 0 ) {
            delete _instance;
            _instance = 0;
        }

        return true;
    }

    return false;
}

bool MobilityModule :: RegisterListener( Port *listener_port )
{
    _DOORS_DEBUG(0, "MobilityModule :: RegisterListener");
    bool rc = false;

    if( _instance == 0 ) {
        _instance = new MobilityModule;
    }

    if( _instance->validatePort( listener_port ) ) {
        _DOORS_DEBUG(0, "Port already connected!");
        return false;
    }

    if( listener_port->getOther() )
        listener_port->disconnect();

    _instance->ref_count++;
    _DOORS_DEBUG(0, "ref_count == " << _instance->ref_count);

    Port *p = _instance->listenerMux.createProvider(0);

    if( p ) {
        listener_port->connect(p);
        rc = true;
    }

    return rc;
}

bool MobilityModule :: validatePort( Port *p )
{
    if( !p || !_instance )
        return false;

    Propagator *propagator = listenerMux.getProviderPropagator();

    if( !propagator )
        return false;

    PortSet *set = propagator->getPortSet();

    if( !set )
        return false;

    Port *p_other = p->getOther();
    
    for( Port *mux_p = set->getFirst();
         mux_p != 0; mux_p = set->getNext() ) {

        if( mux_p == p_other ) {
            return true;
        }

    }


    return false;
}
