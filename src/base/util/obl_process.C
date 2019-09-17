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

$Log: obl_process.C,v $
Revision 1.2  2002/08/14 09:31:42  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:37  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:38  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/obl_process.h>
#include <doors/debug.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

obl_Process *obl_Process::_instance = 0;

obl_Process &obl_Process::instance()
{
    if(!_instance) {
	_instance = new obl_Process();
    }
    return *_instance;
}

obl_Process::obl_Process()
{
    _DOORS_DEBUG(0,"obl_Prosess: go");
}

obl_Process::~obl_Process()
{
    _DOORS_DEBUG(0,"obl_Prosess: done");
}

void obl_Process::fileStatus(obl_FileStatus &status,
			     const obl_String &path)
{
    struct stat sbuf;
    status.exist = true;
    obl_String::iterator si = path.begin();
    if(*si != '/') { // relative path
	obl_FileStatus cwdir;
	cwd(cwdir);
	if(!cwdir.exist) {
	    _DOORS_FAULT(0,"cannot get current working directory");
	}
	status.path = cwdir.path;
	status.path += '/';
	status.path += path;
    } else { // absolute path
	status.path = path;
    }
    if(stat(status.path.c_str(),&sbuf) < 0) {
	switch(errno) {
	case ENOENT:
	    status.exist = false;
	    break;
	default:
	    _DOORS_FAULT(0,"unmanaged stat error");
	}
    }
    status.size = sbuf.st_size;
    status.uid = sbuf.st_uid;
    status.gid = sbuf.st_gid;
    status.fsid = sbuf.st_dev;
    status.readTime = sbuf.st_atime;
    status.writeTime = sbuf.st_mtime;
    status.statusTime = sbuf.st_ctime;
}

void obl_Process::cwd(obl_FileStatus &status)
{
    char path[1024+1];
    status.exist = true;
    if(!getcwd(path,1024)) {
	status.exist = false;
	return;
    }
    fileStatus(status,obl_String(path));
}

