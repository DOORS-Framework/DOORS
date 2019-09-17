// 
//  Copyright 1999
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
//
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

$Log: symhc.h,v $
Revision 1.8  2003/05/08 14:20:30  bilhanan
symbol support for std::string added.

Revision 1.7  2002/08/14 09:31:38  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.6  2002/07/25 22:41:09  ik
Fixes for std::string changes.

Revision 1.5  2002/07/23 14:25:17  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.4  2002/06/06 14:34:42  ik
Typo in #defines.

Revision 1.3  2002/06/06 14:22:43  ik
InetAddr6 symbol.

Revision 1.2  2002/06/06 14:05:18  ik
Handler for InetAddr6.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.4  2002/01/05 18:54:19  ik
Changes to give HandlerCreatorPool a bit more Singleton Design Pattern look.

Revision 1.3  2001/12/12 14:01:47  ik
References to DebugScheduler removed.

Revision 1.2  2001/06/08 16:46:04  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:07  bilhanan
Full licence clause added into doors 0.1


*/



#ifndef SYMHC_H
#define SYMHC_H

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <doors/obl_dcl.h>
#include <doors/tqueue.h>
#include <string>

#if HAVE_IPV6
#include <doors/inetaddr6.h>
#endif

class EnvTask;
class DebugScheduler;

class HandlerCreator {
  public:

    virtual void createHandler (Otask *, Symbol *s) = 0;
    virtual void createHandler (EventTask *, Symbol *s) = 0;
    virtual void createHandler (PTask *, Symbol *s) = 0;
    virtual void createHandler (LinkList *, Symbol *s) = 0;
    virtual void createHandler (DebugScheduler *, Symbol *s) = 0;
    virtual void createHandler (EnvTask *, Symbol *s) = 0; // modularity break
    virtual void createHandler (Port *, Symbol *s) = 0;
    virtual void createHandler (Timer *, Symbol *s) = 0;
    virtual void createHandler (Iface *, Symbol *s) = 0;
    virtual void createHandler (Peer *, Symbol *s) = 0;
    virtual void createHandler (Message *, Symbol *s) = 0;
    virtual void createHandler (UnixAddr *, Symbol *s) = 0;
    virtual void createHandler (InetAddr *, Symbol *s) = 0;
#if HAVE_IPV6
    virtual void createHandler (InetAddr6 *, Symbol *s) = 0;
#endif
    virtual void createHandler (bool *, Symbol *s) = 0;
    virtual void createHandler (Sint8 *, Symbol *s) = 0;
    virtual void createHandler (Sint16 *, Symbol *s) = 0;
    virtual void createHandler (Sint32 *, Symbol *s) = 0;
    virtual void createHandler (Uint8 *, Symbol *s) = 0;
    virtual void createHandler (Uint16 *, Symbol *s) = 0;
    virtual void createHandler (Uint32 *, Symbol *s) = 0;
    virtual void createHandler (std::string *, Symbol *s) = 0;
    virtual void createHandler (Load *, Symbol *s) = 0;
    virtual void createHandler (char **, Symbol *s) = 0;
    virtual void createHandler (Frame *, Symbol *s) = 0;

#if _CF_MEM    
    virtual void createHandler (MemoryManager *, Symbol *s) = 0;
    virtual void createHandler (StatPlainMemManager *, Symbol *s) = 0;
    virtual void createHandler (StatBlockManager *, Symbol *s) = 0;
    virtual void createHandler (StatBlockMemManager *, Symbol *s) = 0;
#endif

#if _CF_ASN1    
    virtual void createHandler (AsnType *, Symbol *s) = 0; // modularity break
#endif
};

typedef TQueue<HandlerCreator> HandlerCreatorPtrSeq; // ph: Ptr ?

class HandlerCreatorPool {
  public:

    static HandlerCreatorPool *instance (void);

    HandlerCreatorPtrSeq *creators (void);

  protected:
    // Although nothing needs to be done here, the constructor is
    // created to protected area to prevent creation of multiple
    // instances.
    HandlerCreatorPool() { }

  private:
    static HandlerCreatorPool *_instance;

    // TQueue<HandlerCreator> _creators;
    HandlerCreatorPtrSeq _creators;
};


#endif  // SYMHC_H

