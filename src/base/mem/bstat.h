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

$Log: bstat.h,v $
Revision 1.2  2002/07/23 14:25:18  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:25  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:38:58  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef BSTAT_H
#define BSTAT_H

#include <stdlib.h>

#include <doors/blockm.h>
#include <doors/stat.h>
#include <doors/util.h>

//
//Class: BlockMemoryStatistic
//
//Inherited: MemoryStatistic
//
//Description: 
//  This class declares commonly used statistical
//  variables for previously allocated fixed block 
//  size memory managers and functions to calculate them.
//  It also declares statistic interface.

class BlockMemoryStatistic: public MemoryStatistic 
{
  public:
    BlockMemoryStatistic (void);
    virtual ~BlockMemoryStatistic (void);

    void add (size_t s1, size_t s2); 
    void del (size_t s1, size_t s2);  

    uint32 getCurrentTotalAllocSize (void);
    uint32 getLargestTotalAllocSize (void);
    uint32 getWantedCurrentTotalAllocSize (void);
    uint32 getWantedLargestTotalAllocSize (void);
    uint32 getWantedTotalAllocSize (void);
    uint16 getWantedMeanAllocSize (void);
    uint16 getWantedLargestAllocSize (void);
    bool isMemError (void);

  protected:
    uint32 currentTotalAllocSize;
    uint32 largestTotalAllocSize;

    uint32 wantedCurrentTotalAllocSize;
    uint32 wantedLargestTotalAllocSize;
    uint32 wantedTotalAllocSize;
    uint16 wantedMeanAllocSize;
    uint16 wantedLargestAllocSize;
   
    bool memoryError;
};

#endif  
