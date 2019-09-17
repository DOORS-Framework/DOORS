//  Copyright 2003
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

$Log: sa.C,v $
Revision 1.7  2004/04/05 07:10:42  hartmanj
SLP classes renamed and DMMTYPE environment variable removed

Revision 1.6  2004/03/08 10:40:11  hartmanj
SAConfigurator called to the rescue

Revision 1.5  2004/02/16 12:59:53  bilhanan
hack to process multicast joins after udp connect

Revision 1.4  2004/02/10 18:15:37  bilhanan
now using udptask.up.connect()

Revision 1.3  2003/11/26 13:33:32  bilhanan
HOST6 and DMMTYPE checks added.

Revision 1.2  2003/10/09 12:18:45  bilhanan
HOST -> HOST6

Revision 1.1  2003/07/10 11:34:40  bilhanan
SLP6 apps added.

Revision 1.2  2003/06/04 11:09:59  kalliosa
Licence clause added

  
*/

#include <doors/ptb.h>
#include <doors/saent.h>
#include <doors/saentsm.h>
#include <doors/configreader.h>
#include <doors/saconfigurator.h>
#include <doors/sasap.h>
#include <doors/serviceurl.h>
#include <doors/saregsm.h>
#include <doors/udp6task.h>
#include <doors/lemon.h>


void usage(char *app)
{
  std::cout << "Usage: "<< app <<" <saconfigfile>" << std::endl;
}


int main (int argc, char **argv)
{
  if( argc < 2 ) {
    usage( argv[0] );
    return 0;
  }

  if (!getenv("HOST6")) {
    std::cout << "Please specify environment variable HOST6." << std::endl;
    return 0;
  }
  
   int bindport_ = 427;
   int slpPort_ = 427;

   std::string config_file (argv[1]);

   SAConfigurator *myconfig = SAConfigurator::Init (config_file);
   
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

   // create input-output handler for tasks                                    
   IoHandler *io = IoHandler :: Instance ();                                   

   // create main scheduler                                                    
   Scheduler *sched = Scheduler :: Instance();                                 
   // create task for managing timers                                          
   TimerTask *tt  = TimerTask :: instance (sched, io);   
   UiTask ui ( sched, io );
   DebugScheduler ds (0, 0, sched);
   EnvTask env ( sched );
   
   
   SAEntSM saentsm;

   InetAddr6 ownAddr;
   ownAddr.set (myconfig->slp_SlpPort , getenv( "HOST6" ) );    
   
#ifdef sparc
   InetAddr6 DA_Address (myconfig->slp_SlpPort, da_group);
   Udp6Task udp ( "udp", sched, io, &DA_Address );
#else 
   Udp6Task udp ( "udp", sched, io, &ownAddr );
#endif
   
   
   SAEnt saentity("SAEnt", sched , &saentsm, ownAddr);

   udp.up.connect (&saentity.down_);

   STATIC_SYMBOL( saentity, 0 );
   STATIC_SYMBOL( udp, 0 );

   SASAP :: User *usr = new SASAP :: User( &env, 0 );
   env.add ( usr, "usr" );

   saentity.up_.connect( usr );
   
   saentity.join();  // initial mcast joins... hack

   while( ui.isTerminated () == false) 
   {                                            
      sched->run ();                                                          
   }

   DELETE_SYMBOL( &saentity );
   DELETE_SYMBOL( &udp );

   env.remove();

   // that's it                                                                
  return 0;           
}
