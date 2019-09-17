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

$Log: blockm.C,v $
Revision 1.4  2002/08/14 09:31:38  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/23 14:25:18  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/05/03 07:51:09  ik
Memory manager new's throw std::bad_alloc.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:11  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:38:57  bilhanan
Full licence clause added into doors 0.1


*/

#include <new>
#include <stdio.h>
#include <doors/blockm.h>

//
//Constructor: BlockManager
//
//Parameters: 
//
//Description: 
//  Initialize attributes..

BlockManager :: BlockManager (void): lowBoundary(0), highBoundary(0), 
size(0), amount(0), headerTbl(0), firstFree(0) 
{
}

//
//Destructor: BlockManager
//
//Description: 
//  Deletes the manager and also frees allocated memory pool and
//  header table with its elements.

BlockManager :: ~BlockManager(void) 
{
  if (lowBoundary) 
    free (lowBoundary);

  for (int i = 0; i < amount; i++) 
  {
    if (headerTbl[i])   
      delete headerTbl[i];
  }

  if (headerTbl)
    free (headerTbl);
}


//
//Function: operator new
//
//Member-Of: BlockManager
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

void *BlockManager :: operator new(size_t size) throw (std::bad_alloc) 
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
//Member-Of: BlockManager
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

void BlockManager :: operator delete(void *ptr) throw ()
{
  free (ptr);
}

//
//Function: allocate
//
//Member-Of: BlockManager
//
//Returns:
//  void *
//
//Parameters:
//  size_t size
// 
//Description:
//  This function is used when memory is allocated.
//  
//  The memory is returned if there is a suitable free memory 
//  block availeble. 
//
//  It is possible that manager is not initialized, so in that
//  case the must be done first. After that the memory block 
//  is returned.
//
//  If for some reason there in no free blocks availeble, then
//  malloc function call is used.

void *BlockManager :: allocate (size_t s) 
{
  _MT_SYNCMONITOR;

  BlockHeader *temp = 0;

  // if everything is ok
  if (size >= s)
  {
     temp = getHeader ();
  }
  // manager is not initialized, so we use default values to do it.
  else if (!isInitialized () && s < DEFAULT_BLOCK_SIZE)
  {
     initBlock (DEFAULT_BLOCK_SIZE, DEFAULT_BLOCK_AMOUNT);
     temp = getHeader ();
  }
  // size of allocation is greater than default size, so we initialize
  // the manager with it.
  else if (!isInitialized ())
  {
     uint16 bAmount = 65535 / s;
     initBlock (s, bAmount);
     temp = getHeader ();
  }
     
  if (temp)
     return temp->block;

  // no memory availeble, so we must use malloc and pass by manager.
  // this is of course error and leads to memory leaking.
  return malloc (s);
}

//
//Function: deallocate
//
//Member-Of: BlockManager
//
//Returns:
//  void 
//
//Parameters:
//  void *ptr
// 
//Description:
//  This function is used when memory is deallocated.
//  It checks whether the pointer belongs to manager 
//  and then the block is marked for free.
//
//  If for some reason parameter ptr does not belong
//  to manager then nothing is done. This is of course error 
//  and may lead to memory leaking.

void BlockManager :: deallocate (void *ptr) 
{
  _MT_SYNCMONITOR;

  if (isMyMemory (ptr))
    setHeader(ptr);
}

//
//Function: getHeader
//
//Member-Of: BlockManager
//
//Returns:
//  BlockManager *
//
//Parameters:
//  void 
// 
//Description:
//  This function is used when memory is allocated.
//  Pointer firstFree points to first free memory block.
//  It is taken away from queue of free blocks and then its
//  memory block is returned. If no memory is awaileble then zero
//  is returned.

BlockHeader *BlockManager :: getHeader(void) 
{
  BlockHeader *temp = firstFree;
  
  if (firstFree) 
  {
    firstFree = firstFree->next;
    temp->next = 0;
    return temp;    
  }
  return 0;
}

//
//Function: getHeader
//
//Member-Of: BlockManager
//
//Returns:
//  BlockManager *
//
//Parameters:
//  void *ptr
// 
//Description:
//  This function is used for retreving the header of the 
//  memory block that is given as a parameter.

BlockHeader *BlockManager :: getHeader(void *ptr) 
{
  uint16 index = ( (unsigned long) ptr - (unsigned long) lowBoundary) / size;    
  return headerTbl[index];
}


//
//Function: setHeader
//
//Member-Of: BlockManager
//
//Returns:
//  BlockManager *
//
//Parameters:
//  void *
// 
//Description:
//  This function is used when memory is deallocated.
//  Correct table index is calculated from block address 
//  so that the right block is found. This block is then 
//  added to queue of free blocks. Correct header is finally 
//  returned. 

BlockHeader *BlockManager :: setHeader(void *ptr) 
{
  uint16 index = ( (unsigned long) ptr - (unsigned long) lowBoundary) / size;  
  
  headerTbl[index]->next = firstFree;
  firstFree = headerTbl[index];
  return firstFree;  
}
 
//
//Function: initBlock
//
//Member-Of: BlockManager
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
//  First function parameters are checked. Notice that 
//  block size must divisible by system specific value (in unix it is 8) 
//  or system may crasch.
//  
//  Table of block headers is created and queue of free headers is
//  initialized.
//
//  Parameter bSize is wanted block size and parameter bAmount
//  is amount of blocks.

void BlockManager :: initBlock(uint16 bSize, uint16 bAmount ) 
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

  headerTbl = (BlockHeader**) malloc(amount * sizeof(BlockHeader*));
  for (int i = 0; i < amount; i++) 
  {
    temp = firstFree;
    headerTbl[i] = new BlockHeader;  
    firstFree = headerTbl[i];
    headerTbl[i]->next = temp;
    headerTbl[i]->block = (void*) ((unsigned long) lowBoundary + (i * bSize));
  }
}

//
//Function: isMyMemory
//
//Member-Of: BlockManager
//
//Returns:
//  bool
//
//Parameters:
//  void *ptr
// 
//Description:
//  This function checks whether the pointer to memory block
//  is allocated from this manager. If memory belongs to memory
//  pool of this manager then one is returned.

bool BlockManager :: isMyMemory (void *ptr) const
{
  return (ptr >= lowBoundary && ptr < highBoundary) ? true: false;
}

//
//Function: isInitialized
//
//Member-Of: BlockManager
//
//Returns:
//  bool
//
//Parameters:
//  void 
// 
//Description:
//  This function returns non zero if memory manager is initialized.

bool BlockManager :: isInitialized (void) const 
{
  return (size != 0) ? true: false;
}


//
//Function: isFreeBlock
//
//Member-Of: BlockManager
//
//Returns:
//  bool
//
//Parameters:
//  void 
// 
//Description:
//  This function returns one is there are free memory
//  blocks on its queue.

bool BlockManager :: isFreeBlock (void) const 
{
  return (firstFree) ? true: false;
}

//
//Function: getSize
//
//Member-Of: BlockManager
//
//Returns:
//  uint16
//
//Parameters:
//  void 
// 
//Description:
//  This function returns size of block.

uint16 BlockManager :: getSize (void) const 
{
  return size;
}

//
//Function: getAmount
//
//Member-Of: BlockManager
//
//Returns:
//  uint16
//
//Parameters:
//  void 
// 
//Description:
//  This function returns amount of blocks.

uint16 BlockManager :: getAmount (void) const 
{
  return amount;
}
