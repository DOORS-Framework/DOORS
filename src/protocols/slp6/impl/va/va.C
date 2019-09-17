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

$Log: va.C,v $
Revision 1.2  2007/10/03 09:06:05  bilhanan
devt

Revision 1.1  2006/08/10 15:39:06  bilhanan
initial commit

Revision 1.6  2004/08/16 10:19:41  bilhanan
removed explicit binding to home address,  in favour of binding to
in6addr_any

Revision 1.5  2004/06/17 08:56:30  hartmanj
Development.

Revision 1.4  2004/05/20 07:00:09  hartmanj
Manual fix to last comment

Revision 1.3  2004/05/19 13:52:02  hartmanj
Update

Revision 1.2  2004/05/12 08:33:20  hartmanj
Development

Revision 1.1  2004/01/28 09:23:30  hartmanj
Initial revision. Compiles.

  
*/

#include <doors/ptb.h>
#include <doors/lemon.h>
//#include <doors/vaesm.h>
//#include <doors/vaent.h>
#include <doors/udp6task.h>
#include "vaesm.h"
#include "vaent.h"
#include "vaconfigurator.h"


void usage(char *app)
{
     std::cout << "Usage: "<< app <<" <vaconfigfile>" << std::endl;
}


int main (int argc, char **argv)
{
     if( argc < 2 ) {
         usage( argv[0] );
         return 0;
     }
    
     std::string config_file (argv[1]);

     VAConfigurator *myconfig = VAConfigurator::Init (config_file);

     std::string aa_group = "ff02::1:1259";

     // create input-output handler for tasks
     IoHandler *io = IoHandler :: Instance ();

     // create main scheduler
     Scheduler *sched = Scheduler :: Instance();
     // create task for managing timers
     TimerTask *tt  = TimerTask :: instance (sched, io);
     UiTask ui ( sched, io );
     DebugScheduler ds (0, 0, sched);
     EnvTask env ( sched );

     VAEntitySM sm;

     InetAddr6 ownAddr;
     ownAddr.set (myconfig->slp_SlpPort);    
     Udp6Task udp ( "udp6", &ds, io, &ownAddr );
     std::string va_name = "va";
     VAEntity va(va_name, &ds, &sm, ownAddr);

     VASAP :: User *usr = new VASAP :: User(&env,0);
     env.add( usr, "usr" );
     va.up.connect(usr);
     udp.up.connect( &va.down );
     STATIC_SYMBOL(udp, 0);
     STATIC_SYMBOL (va, 0);

     // Let the DOORS OPEN!
     while( ui.isTerminated () == false ) {
          sched->run ();
     }

     return 0;
}
