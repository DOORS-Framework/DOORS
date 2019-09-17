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

$Log: statbm.C,v $
Revision 1.4  2002/08/14 09:31:39  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/23 14:25:19  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/05/03 07:51:10  ik
Memory manager new's throw std::bad_alloc.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:13  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:02  bilhanan
Full licence clause added into doors 0.1


*/


#include <new>
#include <doors/statbm.h>
#include <doors/sbheader.h>

//
//Constructor: StatBlockManager
//
//Parameters: 
//
//Description: 
//  Initialize attributes.

StatBlockManager :: StatBlockManager (void) : fragmentation(0)
{
}

//
//Destructor: StatBlockManager
//
//Description: 
//  Deletes the manager.

StatBlockManager :: ~StatBlockManager (void) 
{
}

//
//Function: operator new
//
//Member-Of: StatBlockManager
//
//Returns:
//  void *
//
//Parameters:
//  size_t size
// 
//Description:
//  This function is needed because manager itself
//  uses some dynamic memory before memory management 
//  is set up.

void *StatBlockManager :: operator new (size_t size) throw (std::bad_alloc)
{
  void* tmp = malloc(size);
  if (!tmp) {
    throw std::bad_alloc();
  }
  return tmp;
}

//
//Function: operator delete
//
//Member-Of: StatBlockManager
//
//Returns:
//  void 
//
//Parameters:
//  void *ptr
// 
//Description:
//  This function is needed because manager itself
//  uses some dynamic memory before memory management 
//  is set up.

void StatBlockManager :: operator delete (void *ptr) throw ()
{
  free (ptr);
}

//
//Function: allocate
//
//Member-Of: StatBlockManager
//
//Returns:
//  void *
//
//Parameters:
//  size_t size
// 
//Description:
//  This function is used when memory is allocated.
//  It works basically same way as one in BlockManager
//  class. Only difference is that statistics are
//  collected.

void *StatBlockManager :: allocate (size_t s) 
{
  _MT_SYNCMONITOR;

  StatBlockHeader *temp = 0;

  // if everything is ok
  if (size >= s)
  {
     temp = (StatBlockHeader *) BlockManager :: getHeader();
  }
  // manager is not initialized, so we use default values to do it.
  else if (!isInitialized () && s < DEFAULT_BLOCK_SIZE)
  {
     initBlock (DEFAULT_BLOCK_SIZE, DEFAULT_BLOCK_AMOUNT);
     temp = (StatBlockHeader *) BlockManager :: getHeader();
  }
  // size of allocation is greater than default size, so we initialize
  // the manager with it.
  else if (!isInitialized ())
  {
     uint16 bAmount = 65535 / s;
     initBlock (s, bAmount);
     temp = (StatBlockHeader *) BlockManager :: getHeader();
  }

  if (temp)
  {
     addStat (s, size);
     temp->size = s;
     temp->seqNumber = getTotalAllocAmount();
     return temp->block;
  }

  // allocation fails and malloc function is used.
  // this is of course error and leads to memory leaking.
  memoryError = true;
  return malloc (s);
}

//
//Function: deallocate
//
//Member-Of: StatBlockManager
//
//Returns:
//  void 
//
//Parameters:
//  void *ptr
// 
//Description:
//  This function is used when memory is deallocated.
//  It works basically same as one in BlockManager
//  class. Only difference is that statistics are
//  collected.
//
//  If for some reason parameter ptr does not belong
//  to manager then nothing is done. This is of course error 
//  and leads to memory leaking.

void StatBlockManager :: deallocate (void *ptr) 
{
  _MT_SYNCMONITOR;

  StatBlockHeader *temp;
  if (isMyMemory (ptr))
  {
    if (temp = (StatBlockHeader *) setHeader (ptr))
    {
      delStat (temp->size, size);
      temp->size = 0;
      temp->seqNumber = 0;
    }
  }
}

//
//Function: initBlock
//
//Member-Of: StatBlockManager
//
//Returns:
//  void
//
//Parameters:
//  uint16 bSize
//  uint16 bAmount
// 
//Description:
//  This function is used when memory manager is initialized
//  It sets up memory pool defined by user given parameters.
//  
//  Initialization in this case works almost the same way as
//  when initializing BlockManager. Only differense is that
//  header table consists of headers with statistical variables.

void StatBlockManager :: initBlock(uint16 bSize, uint16 bAmount ) 
{
  BlockHeader *temp = 0;

  // is manager already initialized ... it can be done only once
  if (size != 0)
    return;  

  // is block size divisible by proper value
  if (bSize % BLOCK_ALIGNMENT != 0)
    bSize += (BLOCK_ALIGNMENT - bSize % BLOCK_ALIGNMENT);

  // are the values in range
  if (bSize == 0 || bAmount == 0 || bSize * bAmount > 65535)
    return;

  lowBoundary = malloc(bSize * bAmount);
  highBoundary = (void*) ((unsigned long) lowBoundary + (bAmount * bSize));
  size = bSize;
  amount = bAmount;
  firstFree = temp;

  headerTbl = (BlockHeader**) malloc(amount * sizeof(StatBlockHeader*));
  for (int i = 0; i < amount; i++) {
    temp = firstFree;
    headerTbl[i] = (BlockHeader*) new StatBlockHeader;  
    firstFree = headerTbl[i];
    headerTbl[i]->next = temp;
    headerTbl[i]->block = (void*) ((unsigned long) lowBoundary + (i * bSize));
  }
}

//
//Function: isInitialized
//
//Member-Of: StatBlockManager
//
//Returns:
//  bool
//
//Parameters:
//  void 
// 
//Description:
//  This function returns non zero if memory manager is initialized.

bool StatBlockManager :: isInitialized (void) const 
{
  return (size != 0) ? true: false;
}


//
//Function: addStat
//
//Member-Of: StatBlockManager
//
//Returns:
//  void 
//
//Parameters:
//  size_t wanted
//  size_t allocated
// 
//Description:
//  This function is used when memory is allocated.
//  It is needed because there is one extra statistical varible 
//  in this class that must be also calculated.

void StatBlockManager :: addStat (size_t wanted, size_t allocated)
{
  BlockMemoryStatistic :: add (wanted, allocated);

  if (currentTotalAllocSize)
    fragmentation = (100 - ((100 * (long) wantedCurrentTotalAllocSize) / currentTotalAllocSize)) * 
    ((100 * (long) getCurrentAllocAmount() / amount)) / 100; 
  else
    fragmentation = 0;
}

//
//Function: delStat
//
//Member-Of: StatBlockManager
//
//Returns:
//  void 
//
//Parameters:
//  size_t wanted
//  size_t allocated
// 
//Description:
//  This function is used when memory is deallocated.
//  It is needed because there is one extra statistical varible 
//  in this class that must be also calculated.

void StatBlockManager :: delStat (size_t wanted, size_t allocated)
{
  BlockMemoryStatistic :: del (wanted, allocated);

  if (currentTotalAllocSize)
    fragmentation = (100 - ((100 * (long) wantedCurrentTotalAllocSize) / currentTotalAllocSize)) * 
    ((100 * (long) getCurrentAllocAmount() / amount)) / 100; 
  else
    fragmentation = 0;
}

//
//Function: getHeader
//
//Member-Of: StatBlockManager
//
//Returns:
//  StatBlockHeader *
//
//Parameters:
//  uint16 i
// 
//Description:
//  This function provides statistic interface of the class.
//  It returns proper variable value.

StatBlockHeader * StatBlockManager :: getHeader(uint16 i)
{
  return (StatBlockHeader*) headerTbl[i];
}  


//
//Function: getFragmentation
//
//Member-Of: StatBlockManager
//
//Returns:
//  uint16
//
//Parameters:
//  void
// 
//Description:
//  This function provides statistic interface of the class.
//  It returns proper variable value.

uint16 StatBlockManager :: getFragmentation(void)
{
  return fragmentation;
}  





