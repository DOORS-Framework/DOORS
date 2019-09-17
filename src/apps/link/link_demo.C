//Editor-Info: -*- C++ -*-
//
 //  Copyright 1999 
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
 

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

//  $Log: link_demo.C,v $
//  Revision 1.6  2002/08/14 09:33:02  bilhanan
//  ONS_ macros are now renamed. Refer to top-level ChangeLog for details.
//
//  Revision 1.5  2002/07/25 22:41:09  ik
//  Fixes for std::string changes.
//
//  Revision 1.4  2002/07/23 16:34:05  hartmanj
//
//  Development
//
//  Revision 1.3  2002/07/23 14:28:51  ik
//  Od{Boolean,True,False} -> {bool,true,false}
//
//  Revision 1.2  2002/07/22 22:33:37  bilhanan
//  Licence clause added.
//

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <doors/ptb.h>
#include <doors/lemon.h>
#include <doors/dmm.h>
#include <doors/linksapbase.h>
#include <doors/linkent.h>
#include <doors/linkconn.h>
#include <doors/linkesm.h>
#include <doors/linkent.h>
#include <doors/physent.h>
#include <doors/physsm.h>

#if _CF_MEM

bool mminit (void) {

    const char * mmtype; 
    if (!(mmtype = getenv ("DMMTYPE"))) return false;
    if (strcmp (mmtype, "block") == 0) {
	BlockManager *bm = (BlockManager *) MemoryManager :: instance ();
	bm->initBlock (272, 230);
    }

    if (strcmp (mmtype, "statblock") == 0) {
	StatBlockManager *sbm = (StatBlockManager *) MemoryManager :: instance ();
	sbm->initBlock (272, 230);
    }

    if (strcmp (mmtype, "blockmem") == 0) {
	BlockMemManager *bm = (BlockMemManager *) MemoryManager :: instance ();
	bm->initBlock (16, 4000);
	bm->initBlock (64, 1000);
	bm->initBlock (128, 500);
	bm->initBlock (544, 2);  // for State machines
    }

    if (strcmp (mmtype, "statblockmem") == 0) {
	StatBlockMemManager *smb = (StatBlockMemManager *) MemoryManager :: instance ();
	smb->initBlock (16, 4000);
	smb->initBlock (64, 1000);
	smb->initBlock (128, 500);
	smb->initBlock (544, 2);  // for State machines
    }
return true;
}
#endif

int encodeinit (void) {

    const char * mmtype; 
    if (!(mmtype = getenv ("NETWORK"))) return 0;
    if ((strcmp (mmtype, "down") == 0)|| (strcmp (mmtype, "up") == 0)|| (strcmp (mmtype, "partial") == 0)){
      return 1;
    }

    return 0;
}


main () {

#if _CF_TRACE  

#if _CF_MEM

  if (!mminit ())
    {
      printf("You need to set DMMTYPE before running this demo.\n");
      printf("$DMMTYPE=[plain|block|statblock|blockmem|statblockmem]\n");
      return 0;
    }
  MemoryOptimizer mopt("mem.log", MemoryOptimizer :: all);
#endif

  if (!encodeinit ())
    {
      printf("You need to set NETWORK (network link condition) before running this demo.\n");
      printf("$NETWORK=[down|up]\n");
      return 0;
    }

  // create I/O Handler 
  IoHandler *io = IoHandler :: Instance ();

  // create main scheduler
  Scheduler *s = Scheduler :: Instance ();

  TimerTask *tt = TimerTask :: instance (s, io);
  
#if _CF_MEM
  MemoryManager *memmgr = MemoryManager :: instance ();
#endif

  UiTask ui (s, io);

#if _CF_MEM
  ui.notify (memmgr);
#endif
  
  // scheduler for debugging
  DebugScheduler ds (0, 0, s);
  
  // environment task
  EnvTask env (s);
  
  LinkEntitySM lsm;
  LinkEntity link1 ("link.1", &ds, &lsm);
  LinkEntity link2 ("link.2", &ds, &lsm);
  
  STATIC_SYMBOL (link1, 0);
  STATIC_SYMBOL (link2, 0);

  PhysSM psm;
  PhysEntity phys1 ("phys.1", &ds, &psm);
  PhysEntity phys2 ("phys.2", &ds, &psm);
  
  STATIC_SYMBOL (phys1, 0);
  STATIC_SYMBOL (phys2, 0);
  
  LinkSAP :: User *usr = new LinkSAP :: User (&env, 0);
  env.add (usr, "usr1");
  link1.up.connect (usr);
  
  usr = new LinkSAP :: User (&env, 0);
  env.add (usr, "usr2");
  link2.up.connect (usr);

  phys1.up.connect (&link1.down);
  phys2.up.connect (&link2.down);
  
  phys2.left.connect (&phys1.right);
  
  while (ui.isTerminated () == false) {
    s->run ();
  }
  
  DELETE_SYMBOL (&link2);
  DELETE_SYMBOL (&link1);
  DELETE_SYMBOL (&phys1);
  DELETE_SYMBOL (&phys2);
  
  env.remove ();
  
#else
  printf("You need to compile with _CF_TRACE flag, if you want to run this demo.\n"); 
  
#endif
  return 0;
}




