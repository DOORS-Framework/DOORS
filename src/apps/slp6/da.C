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

$Log: da.C,v $
Revision 1.7  2004/04/05 07:10:42  hartmanj
SLP classes renamed and DMMTYPE environment variable removed

Revision 1.6  2004/03/05 07:37:05  hartmanj
A quick workaround to incompatibilities in v6 multicast implementations

Revision 1.5  2004/03/01 20:02:37  bilhanan
cleaned up, using DAConfigurator, UDP6Task joins correct DA mcast group on startup.

Revision 1.4  2004/02/10 18:15:37  bilhanan
now using udptask.up.connect()

Revision 1.3  2003/11/26 09:13:13  bilhanan
checks introduced for DMMTYPE and HOST6.

Revision 1.2  2003/10/01 11:27:08  bilhanan
HOST -> HOST6

Revision 1.1  2003/07/10 11:34:40  bilhanan
SLP6 apps added.

Revision 1.2  2003/06/04 11:09:57  kalliosa
Licence clause added

  
*/

#include <doors/ptb.h>
#include <doors/daent.h>
#include <doors/daentsm.h>
#include <doors/daconfigurator.h>
#include <doors/udp6task.h>

 
#include <doors/debug.h>

void usage(char *app)
{
   
  std::cout << "Usage: "<< app <<" [daconfigfile]" << std::endl;
  exit(1);
}



int main (int argc, char **argv)
{
  
  if (!getenv("HOST6")) {
    std::cout << "Please specify environment variable HOST6." << std::endl;
    return 0;
  }
  
   if( argc < 2 )
	 usage( argv[0] );
   
   std::string config_file (argv[1]);

   DAConfigurator *myconfig = DAConfigurator::Init (config_file);
			
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
 			
   
   IoHandler *io = IoHandler :: Instance ();                                   
   Scheduler *sched = Scheduler :: Instance();                                 
   TimerTask *tt  = TimerTask :: instance (sched, io);   
      
   DAEntSM sm;

   
   InetAddr6 DA_Address (myconfig->slp_SlpPort, da_group);

   InetAddr6 ownAddr (myconfig->slp_SlpPort,  getenv ("HOST6"));
#ifdef sparc
   Udp6Task udp ( "udp", sched, io, &DA_Address );
#else 
   Udp6Task udp ( "udp", sched, io, &ownAddr );
#endif
   DAEnt daentity("DAEntity", sched , &sm, ownAddr );

   udp.up.connect (&daentity.down_);
   
   while( true ) // ua.isTerminated () == false) 
   {                                            
      sched->run ();                                                          
   }
   
   // that's it                                                                
   return 0;           
}
