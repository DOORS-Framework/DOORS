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

$Log: mobmodule.h,v $
Revision 1.3  2004/07/16 11:42:40  hartmanj
pthread_signalling to wakeup DOORS IoHandler from callback

Revision 1.2  2004/05/21 06:06:54  hartmanj
Reference counting to keep track of interested Ports. Validating Ports when registering and removing them from Mux.

Revision 1.1  2004/05/19 13:52:02  hartmanj
Initial revision


  
*/

#ifndef _MOBMODULE_H_
#define _MOBMODULE_H_

extern "C"{
#include <mobapi.h>
}

#include "mobilitysap.h"
#include <doors/debug.h>
#include <doors/lemon.h>
#include <doors/inetaddr6.h>


class MobilityModule {

public:

    static bool RegisterListener( Port *listener_port );
    static bool RemoveListener( Port *listener_port );

    virtual ~MobilityModule (void);

private:

    bool validatePort( Port *p );

    static MobilityModule *_instance;
    static int mark_mobapi_callback(mobile_node_t mn, int event, long int par);

    MobilityModule( );

    Port listeners;
    PortMux listenerMux;

    InetAddr6 current_coa_address;
    InetAddr6 home_address;

    Uint32 movement_count;

    mobile_node_t mn;
    Uint32 ref_count;

    pthread_t doors_main_thread_id;
};

#endif

