// 
//  Copyright 1999
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
//
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

$Log: obl_process.h,v $
Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:47  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:38  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef obl_process_h
#define obl_process_h

#include <doors/doorsconf.h>
#include <doors/synonym.h>
#include <doors/obl_dcl.h>
#include <doors/obl_string.h>
#include <doors/otime.h>

//+ obl_FileStatus
struct obl_FileStatus {
    bool       exist;        // true if exits
    obl_String path;         // absolute path
    unsigned int size;       // size of the file in bytes
    unsigned int uid;        // user id
    unsigned int gid;        // group id
    unsigned int fsid;       // filesystem id
    OTime        readTime;   // time when last read
    OTime        writeTime;  // time when last write
    OTime        statusTime; // time when last file status change
};
//-

//+ obl_Process
class obl_Process {
public:
    static obl_Process &instance();
    ~obl_Process();
    void fileStatus(obl_FileStatus &status,const obl_String &path);
    void cwd(obl_FileStatus &status);
private:
    obl_Process();
    static obl_Process *_instance;
};
//-

#endif
