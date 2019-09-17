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

$Log: blockmm.C,v $
Revision 1.4  2002/08/14 09:31:38  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/23 14:25:18  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/05/03 07:51:09  ik
Memory manager new's throw std::bad_alloc.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:12  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:38:58  bilhanan
Full licence clause added into doors 0.1


*/

#include <new>
#include <stdio.h>
#include <doors/blockmm.h>
#include <doors/blockm.h>



//
//Function: setOrder
//
//Parameters: 
//  Link *mgr1
//  Link *mgr2
//
//Return: 
//  bool
//
//Description: 
//  This function is used with TQueues add function. 
//  With it the queue of BlockManagers is set in order.


bool setOrder (const Link *mgr1, const Link *mgr2)
{
    if (mgr1)
	if ( ((BlockManager*) mgr1)->getSize() < ((BlockManager*) mgr2)->getSize())
	    return false;
    return true;
}

//
//Constructor: BlockMemManager
//
//Parameters: 
//
//Description: 
//  Just to be.

BlockMemManager ::  BlockMemManager (void): blockManagerAmount (0)
{
}

//
//Destructor: BlockMemManager
//
//Description: 
//  Deletes the main manager and BlockManagers on its list.  

BlockMemManager ::  ~BlockMemManager (void) 
{
  LinkListIterator iterator (&managerList);

  while(iterator.head()) 
  {
    BlockManager *mgr = 0;
    BlockManager *last =0;

    while(mgr = (BlockManager*) iterator.next()) 
    {
      last = mgr;
    }
    managerList.remove(last);
    delete last;
  }
}

//
//Function: operator new
//
//Member-Of: BlockMemManager
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

void *BlockMemManager :: operator new(size_t size) throw (std::bad_alloc)
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
//Member-Of: BlockMemManager
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

void BlockMemManager :: operator delete(void *ptr) throw ()
{
  free (ptr);
}

//
//Function: allocate
//
//Member-Of: BlockMemManager
//
//Returns:
//  void *
//
//Parameters:
//  size_t size
// 
//Description:
//  This function is used when memory is allocated.
//  The correct BlockManager is selected by size.
//  If no proper manager is found then new one
//  is created. 
//

void *BlockMemManager :: allocate (size_t size) 
{
  _MT_SYNCMONITOR;

  BlockManager *mgr = 0;

  // allocate requested memory block
  if (mgr = (BlockManager*) getManager (size))
    return mgr->allocate(size);
  // check if new BlockManager can be created
  else 
  {
     void *m;
     mgr = new BlockManager;
     blockManagerAmount++;
     m = mgr->allocate (size);
     managerList.add (mgr, setOrder);
     if (m)
	 return m;
  }
  
  // Unable to allocate memory, so we must use malloc and pass by 
  // manager. This is of course error and leads to memory leaking.
  return malloc (size);
}

//
//Function: deallocate
//
//Member-Of: BlockMemManager
//
//Returns:
//  void 
//
//Parameters:
//  void *ptr
// 
//Description:
//  This function is used when memory is deallocated.
//  It checks to which manager the pointer belongs to 
//  and then the block is deallocated.
//
//  If for some reason parameter ptr does not belong
//  to any  manager then nothing is done. This is of 
//  course error and leads to memory leaking.

void BlockMemManager :: deallocate (void *ptr) 
{
  _MT_SYNCMONITOR;

  BlockManager *mgr = 0;

  if (mgr = (BlockManager *) getManager (ptr))
    mgr->deallocate (ptr);
}

//
//Function: getManager
//
//Member-Of: BlockMemManager
//
//Returns:
//  MemoryManager *
//
//Parameters:
//  size_t size
// 
//Description:
//  This function is used when memory is allocated.
//  It returns the first BlockManager which block size is
//  bigger than parameter size and there is also free block
//  availeble. If no suitable manager is found then zero
//  is returned.

MemoryManager * BlockMemManager :: getManager (size_t size) 
{
  LinkListIterator iterator (&managerList);
  BlockManager *mgr = 0;

  while(mgr = (BlockManager *) iterator.next()) 
  {
    if (size <= mgr->getSize() && mgr->isFreeBlock())
      return mgr;  
  }

  return 0;
}

//
//Function: getManager
//
//Member-Of: BlockMemManager
//
//Returns:
//  MemoryManager *
//
//Parameters:
//  void *ptr
// 
//Description:
//  This function is used when memory is deallocated.
//  It returns the BlockManager which owns the memory 
//  block passed as parameter ptr. If no correct manager is 
//  found then zero is returned. This would mean that memory
//  block is allocated with malloc function call because
//  no free memory block were availeble.

MemoryManager * BlockMemManager :: getManager (void *ptr) 
{
  LinkListIterator iterator (&managerList);
  BlockManager *mgr = 0;

  while(mgr = (BlockManager *) iterator.next()) 
  {
    if (mgr->isMyMemory (ptr)) 
      break;
  }
  return mgr;
}

//
//Function: initBlock
//
//Member-Of: BlockMemManager
//
//Returns:
//  MemoryManager *
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
//  New BlockManager is created and initialized. After that it is
//  added to list of BlockManagers.
//
//  Parameter bSize is wanted block size and parameter bAmount
//  is amount of blocks.
//
//  If initialization is succesful then new manager is returned.
//  Otherwise zero is returned.

MemoryManager * BlockMemManager :: initBlock (uint16 bSize, uint16 bAmount) 
{

  if (bSize % BLOCK_ALIGNMENT != 0)
    bSize += (BLOCK_ALIGNMENT - bSize % BLOCK_ALIGNMENT);

  // are the values in range
  if (bSize == 0 || bAmount == 0 || bSize * bAmount > 65535)
    return 0;

  BlockManager *mgr = new BlockManager;
  mgr->initBlock(bSize, bAmount);

  if (!mgr->isInitialized())  // initialization fails  
  {
    delete mgr;
    return 0;
  }

  blockManagerAmount++;
  managerList.add (mgr, setOrder);
  return mgr;
}


//
//Function: isInitialized
//
//Member-Of: BlockMemManager
//
//Returns:
//  bool
//
//Parameters:
//  void 
// 
//Description:
//  This function returns non zero if memory manager is initialized.

bool BlockMemManager :: isInitialized (void) const 
{
  LinkListIterator iterator (&managerList);
  return (iterator.head()) ? true: false;
}







