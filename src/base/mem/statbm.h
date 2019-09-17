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

$Log: statbm.h,v $
Revision 1.3  2002/07/23 14:25:19  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/05/03 07:51:10  ik
Memory manager new's throw std::bad_alloc.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.3  2001/07/13 12:17:17  bilhanan
friend allocate func's argument should be size_t instead of unsigned int

Revision 1.2  2001/06/08 16:46:02  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:02  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef STATBM_H
#define STATBM_H

#include <new>
#include <stdlib.h>

#include <doors/blockm.h>
#include <doors/bstat.h>
#include <doors/util.h>
#include <doors/sbheader.h>
#include <doors/statbmm.h>

//
//Class: StatBlockManager
//
//Inherited: 
//  MemoryManager
//  BlockMemoryStatistic
//
//Description:
//  StatBlockManager is a memory manager that uses one sized 
//  preallocated memory blocks. It collects some statistical 
//  information about the usage of memory.

class StatBlockManager: public BlockManager, public BlockMemoryStatistic 
{
  public:
    virtual ~StatBlockManager (void);

    void *operator new (size_t size) throw (std::bad_alloc);
    void operator delete (void *ptr) throw ();

    void *allocate (size_t size);
    void deallocate (void *ptr);

    void initBlock(uint16 bSize, uint16 bAmount);
    virtual bool isInitialized (void) const;

    virtual void addStat (size_t s1, size_t s2); 
    virtual void delStat (size_t s1, size_t s2);  

    uint16 getFragmentation (void);
    StatBlockHeader *getHeader (uint16 i);

    friend MemoryManager *MemoryManager::instance (void);
    friend void *StatBlockMemManager::allocate(size_t size);
    friend MemoryManager *StatBlockMemManager::initBlock(short unsigned int, short unsigned int);

  protected:
    StatBlockManager(void);

  private:
    uint16 fragmentation;
};

#endif

