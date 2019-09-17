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

$Log: memmgr.C,v $
Revision 1.3  2002/08/14 09:31:39  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/05/03 07:51:09  ik
Memory manager new's throw std::bad_alloc.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:12  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:38:59  bilhanan
Full licence clause added into doors 0.1


*/

#include <new>
#include <doors/memmgr.h>
#include <doors/plainmm.h>
#include <doors/statpmm.h>
#include <doors/blockm.h>
#include <doors/statbm.h>
#include <doors/blockmm.h>
#include <doors/statbmm.h>
#include <doors/atexit.h>

#include <string.h>

static MemoryManager *memoryManager;


//
//Constructor: MemoryManager
//
//Parameters: 
//
//Description: 
//  Just to be.

MemoryManager :: MemoryManager (void) 
{
}

//
//Destructor: MemoryManager
//
//Description: 
//  Deletes memory manager.

MemoryManager :: ~MemoryManager (void) 
{
}

//
//Function: operator new
//
//Parameters: size_t size
//
//Return: void *
//
//Description: 
//  Overloads globally operator new and instead of using
//  operating systems memory management it uses one provided
//  by the user. 
//
//  Parameter size is size of memory block to be allocated by calling virtual 
//  allocate function.
//
//  Function returns zero if no memory availeble. Otherwise it returns
//  pointer to memory block.  
//
//  If for some reason there is no memory manager then one is
//  created with instance function. By doing this be can assure that
//  at all time only one instance of memory manager is created and used. 

void * operator new (size_t size) throw (std::bad_alloc)
{
    void* memp = 0;

    if (size > 0) {
        if (memoryManager == 0) {
	    _MT_STATIC_SMONITOR(MemoryManager :: _DOORS_syncLock);
	    if (memoryManager == 0) {
		memoryManager = MemoryManager :: instance ();
	    } 
	}

	memp = memoryManager->allocate (size);
	if (!memp) {
	  throw std::bad_alloc();
	}
    }

    return memp;
}     

//
//Function: operator delete
//
//Parameters: void *ptr
//
//Return: void 
//
//Description: 
//  Overloads globally operator delete and instead of using
//  operating systems memory management it uses one provided
//  by the user. 
//
//  Parameter ptr is a pointer to memory block that is to be 
//  deallocated. 
//
//  If for some reason there is no memoryManager then nothing
//  is done. This is an error and may lead to memory leaking.

void operator delete (void *ptr) throw ()
{
  if (memoryManager)
    memoryManager->deallocate(ptr);
}

//
//Function: destroyInstance
//
//Return: void 
//
//Description: 
//  This function is used with ATEXIT macro and it
//  deletes the memory manager at the end of the program.

void destroyInstance (void)
{
    if (memoryManager)
    {
	delete memoryManager;
	memoryManager = 0;
    }
}


//
//Function: instance
//
//Parameters: 
//
//Return: MemoryManager * 
//    
//Description: 
//  This function is used for creating a instance of memory
//  manager. The wanted type of manager must be set as an
//  environment variable with setenv command. The proper
//  type of manager is created and the instance of it is
//  returned.

MemoryManager *MemoryManager :: instance (void)
{
  if (_instance == 0)
  {
      const char *type = getenv ("DMMTYPE");
      if (type)
      {
        if (strcmp (type, "statplain") == 0)
            _instance = (MemoryManager *) new StatPlainMemManager; 
        else if (strcmp (type, "block") == 0)
            _instance = (MemoryManager *) new BlockManager; 
        else if (strcmp (type, "statblock") == 0)
            _instance = (MemoryManager *) new StatBlockManager; 
        else if (strcmp (type, "blockmem") == 0)
            _instance = (MemoryManager *) new BlockMemManager; 
        else if (strcmp (type, "statblockmem") == 0)
            _instance = (MemoryManager *) new StatBlockMemManager; 
        else
            _instance = (MemoryManager *) new PlainMemManager;
      }
      else
        _instance = (MemoryManager *) new PlainMemManager;

      ATEXIT (destroyInstance);
  }

  return _instance;
}

MemoryManager *MemoryManager :: _instance = 0;
_MT_LOCK(MemoryManager :: _DOORS_syncLock);


