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

$Log: ua.C,v $
Revision 1.13  2004/04/05 07:10:42  hartmanj
SLP classes renamed and DMMTYPE environment variable removed

Revision 1.12  2004/04/01 06:24:36  hartmanj
Commented out delete tt; which caused segfaulting when exiting program. Renamed UAMain ua from 'main' to 'UAEnt'

Revision 1.11  2004/03/30 06:07:23  hartmanj
Fixes to rid compile errors

Revision 1.10  2004/03/27 12:59:11  bilhanan
removed uaapptask #include directives.

Revision 1.9  2004/03/27 12:48:11  bilhanan
ua built with Lemon.

Revision 1.8  2004/03/08 11:34:54  bilhanan
using UAConfigurator

Revision 1.7  2004/02/10 14:48:34  bilhanan
progressive bugfixing

Revision 1.6  2003/10/09 12:18:45  bilhanan
HOST -> HOST6

Revision 1.5  2003/09/04 13:10:17  hartmanj
Correcting myself. See comment for rv.1.4

Revision 1.4  2003/09/04 08:05:00  hartmanj
Removed #include <doors/slpmcasthelper.h>

Revision 1.3  2003/07/10 13:07:14  bilhanan
std:: namespace added to cerr and endl

Revision 1.2  2003/07/10 11:58:12  bilhanan
this is the correct code for ua functionality, ignore previous version.

Revision 1.2  2003/06/04 11:09:59  kalliosa
Licence clause added

  
*/


#include <iostream>
#include <doors/uaconfigurator.h>
#include <doors/ptb.h>
#include <doors/uaent.h>
#include <doors/uaentsm.h>
#include <doors/uaconn.h>
#include <doors/uaconnsm.h>
#include <doors/udp6task.h>
#include <doors/lemon.h>


#include <doors/debug.h>

void usage (char *app)
{
  std::cout << "Usage: "<< app <<" [uaconfigfile]" << std::endl;
  exit(1);

}


int main (int argc, char *argv[]) {

  if( argc < 2 ) {
    usage( argv[0] );
    return 0;
  }

   if (!getenv ("HOST6"))
   {      
    std::cout << "Please specify environment variable HOST6." << std::endl;
    return 0;
   }
   
   std::string config_file (argv[1]);

   UAConfigurator *myconfig = UAConfigurator::Init (config_file);
			
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
   Scheduler *s = Scheduler :: Instance();
   TimerTask *tt  = TimerTask :: instance (s, io);   
   
   UiTask ui (s, io);
   EnvTask env (s);

   DebugScheduler ds (0, 0, s);


   UAEntSM sm;
   UAEnt uaentity ("UAEnt", s, &sm);
   STATIC_SYMBOL (uaentity, 0);

   InetAddr6 DA_Address (myconfig->slp_SlpPort, da_group);
   InetAddr6 ownAddr (myconfig->slp_SlpPort, getenv("HOST6"));

#ifdef sparc
   Udp6Task udp ( "udp", s, io, &DA_Address );
#else 
   Udp6Task udp ( "udp", s, io, &ownAddr );
#endif

   STATIC_SYMBOL (udp, 0);

   UASAP :: User *usr = new UASAP :: User(&env, 0);
   env.add (usr, "usr");

   uaentity.up.connect(usr);
   udp.up.connect(&uaentity.down);

   while( ui.isTerminated () == false) {                                            
      s->run ();                                                          
   }
   
   DELETE_SYMBOL (&env);
   DELETE_SYMBOL (&uaentity);
   DELETE_SYMBOL (&udp);
   
   delete io;
   delete s;
	 //delete tt;
   env.remove ();
   
   return 0;

}
