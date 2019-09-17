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

$Log: memmgr.h,v $
Revision 1.2  2002/08/14 09:31:39  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:41  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:38:59  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef MEMMGR_H
#define MEMMGR_H

#include <doors/util.h>
#include <doors/ocoremac.h>

//
//Class: MemoryManager
//
//Description: 
//  This class declares a common interface for all different
//  memory managers. 
//
//  It uses a method called sigleton pattern to ensure
//  that only one instance of memory manager is used.  
//  At the same time it provides a global point to
//  access it.

class MemoryManager: public Link 
{
  public:
    virtual ~MemoryManager (void);

    virtual void *allocate (size_t) = 0; 
    virtual void deallocate (void *) = 0;

    _MT_GETSYNCLOCK
    _MT_STATICLOCK(_DOORS_syncLock)
    static MemoryManager *instance (void);
  protected:
    MemoryManager (void);

  private:
    static MemoryManager *_instance;  
};

#endif




