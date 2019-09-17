//  Copyright 2000 
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


// A program that instanciates NamingContext service, registers it to
// SLP. At ^C (SIGINT), the program deregisters the namingservice from SLP
// and prints the time server was up.


#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>


#include <doors/ldapcon.h>
#include <doors/slpwrap.h>
#include "cosnaming.h"
#include <doors/namingcontext_impl.h>

time_t start_time = 0;
const std::string name = "service:namingservice://doors.atm.tut.fi/";

void own_catch_int(int)
{
    std::cout << "\nBailing out...just a sec, cleaning up..";
    std::cout.flush();

    DeregisterIORfromSLP(name);


    int total = static_cast<int>(difftime(time(NULL),start_time));
    std::cout << "..there." << std::endl;
    std::cout << "The server was up for ";

    // Days serving
    (total/86400) >= 1 ? std::cout << static_cast<int>(total/86400)
				   << " days, " : std::cout <<  "";

    total %= 86400;

    // Hours
    (total/3600) >= 1 ? std::cout << static_cast<int>(total/3600)
                                   << " hours, " : std::cout <<  "";

    total %= 3600;

    // Minutes
    (total/60) >= 1 ? std::cout << static_cast<int>(total/60)
				  << " minutes, " : std::cout <<  "";

    total %= 60;

    // and Seconds
    std::cout << (static_cast<int>(total))%60 << " secs." << std::endl;

    exit(1);
}

int main( int argc, char *argv[] )
{
    CORBA::ORB_ptr orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
    CORBA::Object_ptr poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_ptr poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_ptr mgr = poa->the_POAManager();

    start_time = time(NULL);

    // Let's start the server

    std::cout << "Creating NamingService.." << std::endl;

    NamingContext_impl *servant = new NamingContext_impl(orb);

    std::cout << "Activating NamingService Object.." << std::endl;

    PortableServer::ObjectId_var oid = poa->activate_object( servant );

    CORBA::Object_ptr temp_obj = poa->servant_to_reference(servant);

    std::cout << "Registering NamingService with SLP.." << std::endl;

    CORBA::String_var str_ior = orb->object_to_string(temp_obj);


    RegisterIORtoSLP(name, 65535, (char*)str_ior);


    // We inserted the server to ldap-dir, so we'd better be prepaired
    // to remove it if(when) SIG_INT comes.
    signal(SIGINT, own_catch_int);


    // Vaivutaan levolliseen uneen..
    std::cout << "NamingService ready to run.." << std::endl;
    mgr->activate ();
    orb->run();


    poa->destroy (TRUE, TRUE);
    delete servant;
    return 0;
}
