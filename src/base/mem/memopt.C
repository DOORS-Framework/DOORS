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

$Log: memopt.C,v $
Revision 1.5  2009/04/20 13:44:18  bilhanan
bugfix to allow c-style string manipulation

Revision 1.4  2002/08/14 09:31:39  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/25 10:08:38  ik
Use std::string.

Revision 1.2  2002/05/03 07:51:09  ik
Memory manager new's throw std::bad_alloc.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:12  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:38:59  bilhanan
Full licence clause added into doors 0.1


*/

#include <string>
#include <cstring>
#include <new>
#include <doors/memopt.h>
#include <doors/debug.h>
#include <sys/types.h>
#include <sys/time.h>



//
//Constructor: MemoryOptimizer
//
//Parameters:
//  std::string fileName
//  ReporType type    
//Description:
//  Constructor opens the file given as a first parameter fileName.
//  No file services from core is used in order to avoid dynamic 
//  memory access.
//  
//  Type of memory manager and time of day is also recorded.
//
//  Parameter type specifies wheather the contents of possible
//  header tables are also printed out.

MemoryOptimizer :: MemoryOptimizer (std::string fileName, ReportType type)
: reportType (type)
{
  if ((fd = ::creat (fileName.c_str(), 0777)) == -1)
  {
      _DOORS_FAULT(0,"unmanaged error condition for creat(" 
		<< fileName << ")");
  }

  mainManager = getenv ("DMMTYPE"); 
  gettimeofday (&startTime, 0);
}


//
//Constructor: MemoryOptimizer
//
//Description:
//  Destructor records time of day and prints out the results 
//  into the file.

MemoryOptimizer :: ~MemoryOptimizer (void)
{
  gettimeofday (&stopTime, 0);

  results();
  writeSpeed();

  ::close (fd);
}

//
//Function: operator new
//
//Member-Of: MemoryOptimizer
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

void *MemoryOptimizer :: operator new(size_t size) throw (std::bad_alloc)  
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
//Member-Of: MemoryOptimizer
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

void MemoryOptimizer :: operator delete(void *ptr) throw ()
{
  free (ptr);
}



//
//Function: results
//
//Member-Of: MemoryOptimizer
//
//Parameters:
//
//Return:
//
//Description:
//  Function checks the type of memory manager and acts upon that. 
//

void MemoryOptimizer :: results (void)
{
  MemoryManager *mem = MemoryManager :: instance ();
  writeHeader();

  if (mainManager)
  { 
    if (!strcmp ("statplain", mainManager))
    {
      writePlain (mem);
    }
    else if (!strcmp ("statblock", mainManager))
    {
      writeBlock (mem);
    }
    else if (!strcmp ("statblockmem", mainManager))
    {
      writeBlockMem (mem);

      LinkListIterator iterator (&( (StatBlockMemManager*) mem)->managerList);
      MemoryManager *mgr = 0;

      while(mgr = (MemoryManager*) iterator.next()) 
      {
        writeBlock (mgr);
      }
    }
    else
    {
    // error
    }
  }
}

//
//Function: header
//
//Member-Of: MemoryOptimizer
//
//Parameters:
//
//Return:
//
//Description:
//  Function prints out the header in the beginning of the file.
//  The name of the manager and the time of day is printed out.

void MemoryOptimizer :: writeHeader (void)
{
  char *buf = buffer;
  time_t clock;

  time (&clock);
  sprintf (buf, "// Memory optimizer - %s \n\n", ctime(&clock)); 
  ::write (fd, buf, strLength (buf));
}


//
//Function: writeSpeed
//
//Member-Of: MemoryOptimizer
//
//Parameters:
//
//Return:
//
//Description:
//  Function prints out the execution time into the file. 
//  

void MemoryOptimizer :: writeSpeed (void)
{
  char *buf = buffer;
  long sTime = (stopTime.getSeconds() - startTime.getSeconds());
  long uTime = (stopTime.getMicros() - startTime.getMicros());

  sprintf (buf, "Execution time was %u seconds, %u micro seconds\n", sTime, uTime); 
  ::write (fd, buf, strLength (buf));
}


//
//Function: writeStat
//
//Member-Of: MemoryOptimizer
//
//Parameters:
//  MemoryStatistic *mgr
//Return:
//
//Description:
//  Function prints out the statistical variables of the class 
//  MemoryStatistic into the file. 
//  

void  MemoryOptimizer :: writeStat (MemoryStatistic *mgr)
{
  char *buf = buffer;

  sprintf (buf, "TotalAllocAmount = %u\n", mgr->getTotalAllocAmount ()); 
  ::write (fd, buf, strLength (buf));

  sprintf (buf, "CurrentAllocAmount = %u\n", mgr->getCurrentAllocAmount ()); 
  ::write (fd, buf, strLength (buf)); 

  sprintf (buf, "TotalAllocSize = %u\n", mgr->getTotalAllocSize ()); 
  ::write (fd, buf, strLength (buf));

  sprintf (buf, "MeanAllocSize = %u\n", mgr->getMeanAllocSize ()); 
  ::write (fd, buf, strLength (buf));

  sprintf (buf, "LargestAllocSize = %u\n", mgr->getLargestAllocSize ()); 
  ::write (fd, buf, strLength (buf));

  sprintf (buf, "StatisticError = %d\n", mgr->isStatError ()); 
  ::write (fd, buf, strLength (buf));

}


//
//Function: writeBStat
//
//Member-Of: MemoryOptimizer
//
//Parameters:
//  MemoryStatistic *mgr
//Return:
//
//Description:
//  Function prints out the statistical variables of the class 
//  BlockMemoryStatistic into the file. 
//  

void  MemoryOptimizer :: writeBStat (BlockMemoryStatistic *mgr)
{
  char *buf = buffer;

  sprintf (buf, "CurrentTotalAllocSize = %u\n", mgr->getCurrentTotalAllocSize ()); 
  ::write (fd, buf, strLength (buf));

  sprintf (buf, "LargestTotalAllocSize = %u\n", mgr->getLargestTotalAllocSize ()); 
  ::write (fd, buf, strLength (buf));

  sprintf (buf, "WantedTotalAllocSize = %u\n", mgr->getWantedTotalAllocSize ()); 
  ::write (fd, buf, strLength (buf));

  sprintf (buf, "WantedMeanAllocSize = %u\n", mgr->getWantedMeanAllocSize ()); 
  ::write (fd, buf, strLength (buf));

  sprintf (buf, "WantedLargestAllocSize = %u\n", mgr->getWantedLargestAllocSize ()); 
  ::write (fd, buf, strLength (buf));

  sprintf (buf, "WantedCurrentTotalAllocSize = %u\n", mgr->getWantedCurrentTotalAllocSize ()); 
  ::write (fd, buf, strLength (buf));

  sprintf (buf, "WantedLargestTotalAllocSize = %u\n", mgr->getWantedLargestTotalAllocSize ()); 
  ::write (fd, buf, strLength (buf));

  sprintf (buf, "MemoryError = %d\n", mgr->isMemError ()); 
  ::write (fd, buf, strLength (buf));
}


//
//Function: writeHeaderTbl
//
//Member-Of: MemoryOptimizer
//
//Parameters:
//  MemoryManager *m
//Return:
//
//Description:
//  Function prints out the the contents of the header table 
//  of class StatBlockManager.
//
//  Notice that the amount of information can be very large
//  so there is a possibility to choose wheather the information
//  is printed out of not. 

void  MemoryOptimizer :: writeHeaderTbl (MemoryManager *m)
{
  char *buf = buffer;
  StatBlockManager *mgr = (StatBlockManager*) m;
  
  for (int i = 0;i < mgr->getAmount();)
  {
      for (int j = 0;i < mgr->getAmount() && j < 2; j++)
      {
           sprintf(buf, "[%d]=(seq =%5d, size =%5d), ", i, (mgr->getHeader(i))->seqNumber, 
           (mgr->getHeader(i))->size);
           buf += strlen (buf);
           i++;
      }
      sprintf(buf, "\n", 0);
      buf = buffer;
      ::write (fd, buf, strLength (buf));
  }
}


//
//Function: writePlain
//
//Member-Of: MemoryOptimizer
//
//Parameters:
//  MemoryManager *m
//Return:
//
//Description:
//  Function is called when StatPlainMemManager is used. 
//  It calls proper function to print out its statistical
//  variables.

void MemoryOptimizer :: writePlain (MemoryManager *m)
{
  StatPlainMemManager *mgr = (StatPlainMemManager*) m;
  char *buf = buffer;

  sprintf (buf, "/* StatPlainMemManager memory management */\n"); 
  ::write (fd, buf, strLength (buf));

  writeStat ((MemoryStatistic *) mgr);

  sprintf (buf, "\n\n"); 
  ::write (fd, buf, strLength (buf));
}


//
//Function: writeBlock
//
//Member-Of: MemoryOptimizer
//
//Parameters:
//  MemoryManager *m
//Return:
//
//Description:
//  Function is called when StatBlockManager is used. 
//  It calls proper functions to print out its statistical
//  variables. Also the variable fragmentation is printed
//  out.
//  
//  There is a possibility for user to choose wheather
//  to print out the contents of the header table.
//  The information in it can be useful when trackin 
//  memory leaks.

void MemoryOptimizer :: writeBlock (MemoryManager *m)
{
  StatBlockManager *mgr = (StatBlockManager*) m;
  char *buf = buffer;

  sprintf (buf, "/* StatBlockManager memory management */\n"); 
  ::write (fd, buf, strLength (buf));

  writeStat ((MemoryStatistic *) mgr);
  writeBStat ((BlockMemoryStatistic *) mgr);

  sprintf (buf, "Fragmentation = %d \n", mgr->getFragmentation ()); 
  ::write (fd, buf, strLength (buf));

  sprintf (buf, "\n\n"); 
  ::write (fd, buf, strLength (buf));

  if (reportType == all)
     writeHeaderTbl (m);

  sprintf (buf, "\n\n"); 
  ::write (fd, buf, strLength (buf));
}


//
//Function: writeBlockMem
//
//Member-Of: MemoryOptimizer
//
//Parameters:
//  MemoryManager *m
//Return:
//
//Description:
//  Function is called when StatBlockMemManager is used. 
//  It calls proper functions to print out its statistical
//  variables. Also the variable blockManagerAmount is printed
//  out.

void MemoryOptimizer :: writeBlockMem (MemoryManager *m)
{
  StatBlockMemManager *mgr = (StatBlockMemManager*) m;

  char *buf = buffer;

  sprintf (buf, "/* StatBlockMemManager memory management */\n"); 
  ::write (fd, buf, strLength (buf));

  writeStat ((MemoryStatistic *) mgr);
  writeBStat ((BlockMemoryStatistic *) mgr);

  sprintf (buf, "BlockManagerAmount = %d \n", mgr->getBlockManagerAmount ()); 
  ::write (fd, buf, strLength (buf));

  sprintf (buf, "\n\n"); 
  ::write (fd, buf, strLength (buf));
}


//
//Function: strLenght
//
//Member-Of: MemoryOptimizer
//
//Parameters:
//  char *s
//
//Return:
//  uint8
//
//Description:
//  Function calculates the lenght of string given as a   
//  parameter s and returns the value.
//

uint8 MemoryOptimizer :: strLength (char *s)
{
  uint8 num;

  for (num = 0; *s != '\0'; s++)
    num++;

  return num;
}






