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

$Log: statbmm.C,v $
Revision 1.4  2002/08/14 09:31:39  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/23 14:25:19  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/05/03 07:51:10  ik
Memory manager new's throw std::bad_alloc.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:14  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:02  bilhanan
Full licence clause added into doors 0.1


*/


#include <new>
#include <doors/statbmm.h>
#include <doors/statbm.h>
#include <doors/sbheader.h>

extern bool setOrder (const Link *mgr1, const Link *mgr2);

//
//Constructor: StatBlockMemManager
//
//Parameters: 
//
//Description: 
//  Just to be.

StatBlockMemManager :: StatBlockMemManager (void)
{
}

//
//Destructor: StatBlockMemManager
//
//Description: 
//  Deletes the manager.

StatBlockMemManager :: ~StatBlockMemManager (void)
{
}

//
//Function: operator new
//
//Member-Of: StatBlockMemManager
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

void *StatBlockMemManager :: operator new(size_t size) throw (std::bad_alloc) 
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
//Member-Of: StatBlockMemManager
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

void StatBlockMemManager :: operator delete(void *ptr) throw ()
{
  free (ptr);
}

//
//Function: allocate
//
//Member-Of: StatBlockMemManager
//
//Returns:
//  void *
//
//Parameters:
//  size_t size
// 
//Description:
//  This function is used when memory is allocated.
//  It works basically same way as one in BlockMemManager
//  class. Only difference is that statistics are
//  collected.

void *StatBlockMemManager :: allocate (size_t size) 
{
  _MT_SYNCMONITOR;

  StatBlockManager *mgr = 0;

  // allocate requested memory block
  if (mgr = (StatBlockManager *) getManager (size)) 
  {
     add (size, mgr->getSize ());
     return mgr->allocate (size); 
  } 
  // create new StatBlockManager
  else
  {
     void *m;
     mgr = new StatBlockManager;
     blockManagerAmount++;
     m = mgr->allocate (size);
     managerList.add (mgr, setOrder);
     if (m)
     {
      add (size, mgr->getSize ());
      return m;  
     }
  }

  // Unable to allocate memory, so we must use malloc and pass by manager.
  // this is of course error and leads to memory leaking.
  memoryError = true;
     
  return malloc (size);
}

//
//Function: deallocate
//
//Member-Of: StatBlockMemManager
//
//Returns:
//  void 
//
//Parameters:
//  void *ptr
// 
//Description:
//  This function is used when memory is deallocated.
//  It works basically same way as one in BlockMemManager
//  class. Only difference is that statistics are
//  collected.
//
//  If for some reason parameter ptr does not belong
//  to any  manager then nothing is done. This is of 
//  course error and leads to memory leaking.

void StatBlockMemManager :: deallocate (void *ptr) 
{
  _MT_SYNCMONITOR;

  StatBlockManager *mgr = 0;
  StatBlockHeader *temp = 0;

  if (mgr = (StatBlockManager *) BlockMemManager :: getManager (ptr)) 
  {
    temp = (StatBlockHeader *) mgr->BlockManager :: getHeader (ptr);
    if (temp) 
    {
      del (temp->size, mgr->getSize());
      mgr->deallocate (ptr);
    }
  }   
}


//
//Function: isInitialized
//
//Member-Of: StatBlockMemManager
//
//Returns:
//  bool
//
//Parameters:
//  void 
// 
//Description:
//  This function returns non zero if memory manager is initialized.

bool StatBlockMemManager :: isInitialized (void) const 
{
  LinkListIterator iterator (&managerList);
  return (iterator.head()) ? true: false;
}


//
//Function: initBlock
//
//Member-Of: StatBlockMemManager
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
//  when initializing BlockMemManager. Only differense is that
//  header table consists of headers with statistical variables.


MemoryManager * StatBlockMemManager :: initBlock (uint16 bSize, uint16 bAmount) 
{
  // is block size divisible by four
  if (bSize % BLOCK_ALIGNMENT != 0)
    bSize += (BLOCK_ALIGNMENT - bSize % BLOCK_ALIGNMENT);

  // are the values in range
  if (bSize == 0 || bAmount == 0 || bSize * bAmount > 65535)
    return 0;

  StatBlockManager *mgr = new StatBlockManager;
  mgr->initBlock(bSize, bAmount);
  
  if (!mgr->isInitialized())  // was initialization succesfull
  {
    delete mgr;
    return 0;
  }

  blockManagerAmount++;
  managerList.add (mgr, setOrder);
  return mgr;
}


//
//Function: getBlockManagerAmount
//
//Member-Of: StatBlockMemManager
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

uint16 StatBlockMemManager :: getBlockManagerAmount (void) 
{
  return blockManagerAmount;
}
