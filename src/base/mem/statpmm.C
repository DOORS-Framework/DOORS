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

$Log: statpmm.C,v $
Revision 1.3  2002/08/14 09:31:39  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/05/03 07:51:10  ik
Memory manager new's throw std::bad_alloc.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:14  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:03  bilhanan
Full licence clause added into doors 0.1


*/

#include <new>
#include <doors/statpmm.h>

//
//Constructor: StatPlainMemManager
//
//Parameters: 
//
//Description: 
//  Just to be.

StatPlainMemManager :: StatPlainMemManager (void) 
{
}

//
//Destructor: StatPlainMemManager
//
//Description: 
//  Deletes the manager.

StatPlainMemManager :: ~StatPlainMemManager (void) 
{
}

//
//Function: operator new
//
//Member-Of: StatPlainMemManager
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


void *StatPlainMemManager :: operator new(size_t size) throw (std::bad_alloc) 
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
//Member-Of: StatPlainMemManager
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

void StatPlainMemManager :: operator delete(void *ptr) throw ()
{
  free (ptr);
}

//
//Function: allocate
//
//Member-Of: StatPlainMemManager
//
//Returns:
//  void *
//
//Parameters:
//  size_t size
// 
//Description:
//  This function is used when memory is allocated.
//  There is no real memory management but some 
//  basic statistics are collected.

void *StatPlainMemManager :: allocate (size_t size) 
{
  _MT_SYNCMONITOR;

  add (size);
  return malloc (size);
}

//
//Function: deallocate
//
//Member-Of: StatPlainMemManager
//
//Returns:
//  void 
//
//Parameters:
//  void *ptr
// 
//Description:
//  This function is used when memory is deallocated.
//  There is no real memory management but some 
//  basic statistics are collected.

void StatPlainMemManager :: deallocate (void *ptr) 
{
  _MT_SYNCMONITOR;

  del ();
  free (ptr);
}
