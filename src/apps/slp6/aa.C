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

$Log: aa.C,v $
Revision 1.1  2004/10/12 12:23:12  bilhanan
adding support for aa

Revision 1.2  2004/08/03 05:36:41  hartmanj
Bug fixes

Revision 1.1  2004/05/25 09:39:21  hartmanj
Initial versions


  
*/

#include <doors/ptb.h>
#include <doors/lemon.h>
#include <doors/udp6task.h>
#include <doors/aaesm.h>
#include <doors/aaent.h>
#include <doors/aaconfigurator.h>


void usage(char *app)
{
     std::cout << "Usage: "<< app <<" <aaconfigfile>" << std::endl;
}


int main (int argc, char **argv)
{
     if( argc < 2 ) {
         usage( argv[0] );
         return 0;
     }

     std::string config_file (argv[1]);

     AAConfigurator *myconfig = AAConfigurator::Init (config_file);

     // create input-output handler for tasks
     IoHandler *io = IoHandler :: Instance ();

     // create main scheduler
     Scheduler *sched = Scheduler :: Instance();
     // create task for managing timers
     TimerTask *tt  = TimerTask :: instance (sched, io);
     //UiTask ui ( sched, io );
     //DebugScheduler ds (0, 0, sched);
     //EnvTask env ( sched );

     AAEntitySM sm;
     
     InetAddr6 ownAddr;
     ownAddr.set (myconfig->slp_SlpPort , getenv( "HOST" ) );    
   
     Udp6Task udp ( "udp", sched, io, &ownAddr );
     STATIC_SYMBOL(udp, 0);

     AAEntity aa("access-agent", sched , &sm, &udp, ownAddr);
     //STATIC_SYMBOL (aa, 0);
     
     udp.up.connect( &aa.down );

     // Let the DOORS OPEN!
     while( 1 ) { //ui.isTerminated () == false ) {
          sched->run ();
     }

     return 0;
}
