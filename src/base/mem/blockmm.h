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

/*

$Log: blockmm.h,v $
Revision 1.3  2002/07/23 14:25:18  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/05/03 07:51:09  ik
Memory manager new's throw std::bad_alloc.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:24  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:38:58  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef BLOCKMM_H
#define BLOCKMM_H

#include <new>
#include <stdlib.h>

#include <doors/util.h>
#include <doors/memmgr.h>


//
//Class: BlockMemManager
//
//Inherited: MemoryManager
//
//Description:
//  BlockMemManager is a memory manager that uses different sized 
//  preallocated memory blocks. It handles a queue of BlockManagers.

class BlockMemManager: public MemoryManager 
{
  public:
    ~BlockMemManager (void);

    void *operator new (size_t size) throw (std::bad_alloc);
    void operator delete (void *ptr) throw ();

    void *allocate (size_t size);
    void deallocate (void *ptr);

    MemoryManager *initBlock (uint16 bSize, uint16 amount);    
    virtual bool isInitialized (void) const;

    LinkList managerList;

    friend MemoryManager *MemoryManager::instance (void);
 
  protected:
    BlockMemManager (void);
    MemoryManager *getManager (size_t size);
    MemoryManager *getManager (void *ptr);

    uint16 blockManagerAmount;
};


#endif


