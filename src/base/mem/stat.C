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

$Log: stat.C,v $
Revision 1.2  2002/07/23 14:25:18  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:13  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:01  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/stat.h>

#define STAT_LIMIT 0xFFFF0000

//
//Constructor: MemoryStatistic
//
//Parameters: 
//  void
//
//Description: 
//  Initializes its variables to zero.

MemoryStatistic :: MemoryStatistic (void) : totalAllocAmount(0), currentAllocAmount(0),
totalAllocSize(0), meanAllocSize(0), largestAllocSize(0), statisticError(false) 
{
}  

//
//Destructor: MemoryStatistic
//
//Description: 
//  Deletes class.

MemoryStatistic :: ~MemoryStatistic (void) 
{
}  

//
//Function: add
//
//Member-Of: MemoryStatistic
//
//Returns:
//  void 
//
//Parameters:
//  size_t size
// 
//Description:
//  This function is used when memory is allocated.
//  First it checks if the range of one variable and
//  sets error flag if it is needed. Then the values
//  of different variables are calculated.

void MemoryStatistic :: add (size_t size) 
{
  if (totalAllocSize >= STAT_LIMIT)
    setStatError ();

  totalAllocAmount++;
  currentAllocAmount++;

  totalAllocSize += size; 
  if (totalAllocAmount)
    meanAllocSize = totalAllocSize / totalAllocAmount; 
  largestAllocSize = (largestAllocSize < size ) ? size : largestAllocSize;
}

//
//Function: del
//
//Member-Of: MemoryStatistic
//
//Returns:
//  void 
//
//Parameters:
//  void
// 
//Description:
//  This function is used when memory is deallocated.
//  There is only one variable that can be updated.
//  Reason for this is that used memory model does not 
//  provide any more information.

void MemoryStatistic :: del (void) 
{
  currentAllocAmount--;
}

//
//Function: getTotalAllocAmount
//
//Member-Of: MemoryStatistic
//
//Returns:
//  uint32
//
//Parameters:
//  void
// 
//Description:
//  This function provides classes statistic interface.
//  It returns proper variable value.

uint32 MemoryStatistic :: getTotalAllocAmount (void)
{
  return totalAllocAmount;
}

//
//Function: getCurrentAllocAmount
//
//Member-Of: MemoryStatistic
//
//Returns:
//  uint32
//
//Parameters:
//  void
// 
//Description:
//  This function provides classes statistic interface.
//  It returns proper variable value.

uint32 MemoryStatistic :: getCurrentAllocAmount (void)
{
  return currentAllocAmount;
}

//
//Function: getTotalAllocSize
//
//Member-Of: MemoryStatistic
//
//Returns:
//  uint32
//
//Parameters:
//  void
// 
//Description:
//  This function provides classes statistic interface.
//  It returns proper variable value.

uint32 MemoryStatistic :: getTotalAllocSize (void)
{
  return totalAllocSize;
}

//
//Function: getMeanAllocSize
//
//Member-Of: MemoryStatistic
//
//Returns:
//  uint16
//
//Parameters:
//  void
// 
//Description:
//  This function provides classes statistic interface.
//  It returns proper variable value.

uint16 MemoryStatistic :: getMeanAllocSize (void)
{
  return meanAllocSize;
}

//
//Function: getLargestAllocSize
//
//Member-Of: MemoryStatistic
//
//Returns:
//  uint16
//
//Parameters:
//  void
// 
//Description:
//  This function provides classes statistic interface.
//  It returns proper variable value.

uint16 MemoryStatistic :: getLargestAllocSize (void)
{
  return largestAllocSize;
}

//
//Function: getMeanAllocSize
//
//Member-Of: MemoryStatistic
//
//Returns:
//  bool
//
//Parameters:
//  void
// 
//Description:
//  This function provides classes statistic interface.
//  It returns non zero if ranges of statistical variables are
//  exceeded and the numbers are no longer reliable. Otherwise
//  zero is returned.

bool MemoryStatistic :: isStatError (void)
{
  return statisticError;
}

//
//Function: getMeanAllocSize
//
//Member-Of: MemoryStatistic
//
//Returns:
//  bool
//
//Parameters:
//  void
// 
//Description:
//  This function provides classes statistic interface.
//  With it error flag can be set to non zero value.

void MemoryStatistic :: setStatError (void)
{
  statisticError = true;
}




