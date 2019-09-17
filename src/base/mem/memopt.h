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

$Log: memopt.h,v $
Revision 1.3  2002/07/25 10:08:39  ik
Use std::string.

Revision 1.2  2002/05/03 07:51:09  ik
Memory manager new's throw std::bad_alloc.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:41  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.5  2001/06/08 08:38:59  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef MEMORYOPTIMIZER_H
#define MEMORYOPTIMIZER_H

#include <string>
#include <new>
#include <doors/dmm.h>
#include <doors/obl_time.h>
#include <fcntl.h>
#include <unistd.h>

#define PERMS 777

//
//Class: MemoryOptimizer
//
//Description:
//  This class is used for printing out to the file
//  the contents of the statistical memory managers.
//
//  It does not use any services provided by DOORS
//  and it is not asynchronous. Reason for this is that
//  all dynamic memory access is eliminated and it can
//  therefore print out the memory usage of actual 
//  user application.

class MemoryOptimizer
{
  public:
    enum ReportType {all, some} reportType; 
    MemoryOptimizer (std::string fileName, ReportType type);
    ~MemoryOptimizer (void);

    void *operator new (size_t size) throw (std::bad_alloc);
    void operator delete (void *ptr) throw ();

    void results (void);

  private:
    void writeHeader (void);
    void writeSpeed (void);

    void writeStat (MemoryStatistic *m);
    void writeBStat (BlockMemoryStatistic *m);
    void writeHeaderTbl (MemoryManager *m);

    void writePlain (MemoryManager *m);
    void writeBlock (MemoryManager *m);
    void writeBlockMem (MemoryManager *m);

    uint8 strLength (char *);

    int fd;
    const char *mainManager;
    char buffer[80];
    OTime startTime, stopTime;
};

#endif
