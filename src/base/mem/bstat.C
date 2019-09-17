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

$Log: bstat.C,v $
Revision 1.2  2002/07/23 14:25:18  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:12  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:38:58  bilhanan
Full licence clause added into doors 0.1


*/

#include <doors/bstat.h>
#include <doors/stat.h>

#define STAT_LIMIT 0xFFFF0000


//
//Constructor: BlockMemoryStatistic
//
//Parameters: 
//  void
//
//Description: 
//  Initializes its variables to zero.

BlockMemoryStatistic :: BlockMemoryStatistic (void) : currentTotalAllocSize(0), largestTotalAllocSize (0)
, wantedCurrentTotalAllocSize(0), wantedLargestTotalAllocSize(0), wantedTotalAllocSize(0), 
wantedMeanAllocSize(0), wantedLargestAllocSize(0), memoryError(false)
{
}

//
//Destructor: BlockMemoryStatistic
//
//Description: 
//  Deletes class.

BlockMemoryStatistic :: ~BlockMemoryStatistic (void)
{
}

//
//Function: add
//
//Member-Of: BlockMemoryStatistic
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
//  First it checks if the range of one variable and
//  sets error flag if it is needed. Then the values
//  of different variables are calculated.
//
//  Parameter wanted is size of memory block user
//  wants to allocate and parameter allocated is
//  size of memory block that is really allocated.

void BlockMemoryStatistic :: add (size_t wanted, size_t allocated)
{
  MemoryStatistic :: add (allocated);
  
  if (getTotalAllocSize() >= STAT_LIMIT)
    setStatError ();
  

  currentTotalAllocSize += allocated;
  largestTotalAllocSize = (largestTotalAllocSize < currentTotalAllocSize) ?
                          currentTotalAllocSize : largestTotalAllocSize;
  
  wantedTotalAllocSize += wanted;
  wantedCurrentTotalAllocSize += wanted;
  if (getTotalAllocAmount())
    wantedMeanAllocSize = wantedTotalAllocSize / getTotalAllocAmount();
  wantedLargestAllocSize = (wantedLargestAllocSize < wanted) 
                           ? wanted : wantedLargestAllocSize;
  wantedLargestTotalAllocSize = (wantedLargestTotalAllocSize < wantedCurrentTotalAllocSize)
                                ? wantedCurrentTotalAllocSize : wantedLargestTotalAllocSize;
}

//
//Function: del
//
//Member-Of: BlockMemoryStatistic
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
//  It calculates few variables.
//  
//  Parameter wanted is size of memory block user
//  wants to allocate and parameter allocated is
//  size of memory block that is really allocated.

void BlockMemoryStatistic :: del (size_t wanted, size_t allocated)
{
  MemoryStatistic :: del ();
  wantedCurrentTotalAllocSize -= wanted;
  currentTotalAllocSize -= allocated;  
}

//
//Function: getCurrentTotalAllocSize
//
//Member-Of: BlockMemoryStatistic
//
//Returns:
//  uint32
//
//Parameters:
//  void
// 
//Description:
//  This function provides statistic interface of the class.
//  It returns proper variable value.

uint32 BlockMemoryStatistic :: getCurrentTotalAllocSize (void)
{
  return currentTotalAllocSize;
}

//
//Function: getLargestTotalAllocSize
//
//Member-Of: BlockMemoryStatistic
//
//Returns:
//  uint32
//
//Parameters:
//  void
// 
//Description:
//  This function provides statistic interface of the class.
//  It returns proper variable value.

uint32 BlockMemoryStatistic :: getLargestTotalAllocSize (void) 
{
  return  largestTotalAllocSize;
}

//
//Function: getWantedCurrentTotalAllocSize
//
//Member-Of: BlockMemoryStatistic
//
//Returns:
//  uint32
//
//Parameters:
//  void
// 
//Description:
//  This function provides statistic interface of the class.
//  It returns proper variable value.

uint32 BlockMemoryStatistic :: getWantedCurrentTotalAllocSize (void)
{
  return wantedCurrentTotalAllocSize;  
}

//
//Function: getWantedLargesTotalAllocSize
//
//Member-Of: BlockMemoryStatistic
//
//Returns:
//  uint32
//
//Parameters:
//  void
// 
//Description:
//  This function provides statistic interface of the class.
//  It returns proper variable value.
    
uint32 BlockMemoryStatistic :: getWantedLargestTotalAllocSize (void)
{
  return wantedLargestTotalAllocSize;
}

//
//Function: getWantedTotalAllocSize
//
//Member-Of: BlockMemoryStatistic
//
//Returns:
//  uint32
//
//Parameters:
//  void
// 
//Description:
//  This function provides statistic interface of the class.
//  It returns proper variable value.
    
uint32 BlockMemoryStatistic :: getWantedTotalAllocSize (void)
{
  return wantedTotalAllocSize;
}
    
//
//Function: getWantedMeanAllocSize
//
//Member-Of: BlockMemoryStatistic
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

uint16 BlockMemoryStatistic :: getWantedMeanAllocSize (void)
{
  return wantedMeanAllocSize;
}

//
//Function: getWantedLargestAllocSize
//
//Member-Of: BlockMemoryStatistic
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
 
uint16 BlockMemoryStatistic :: getWantedLargestAllocSize (void)
{
  return wantedLargestAllocSize;
}  

//
//Function: isMemError
//
//Member-Of: BlockMemoryStatistic
//
//Returns:
//  bool
//
//Parameters:
//  void
// 
//Description:
//  This function provides statistic interface of the class.
//  It returns non zero value if there has not been enough.
//  preallocated memory blocks. Otherwize zero is returned.

bool BlockMemoryStatistic :: isMemError (void)
{
  return memoryError;
}




