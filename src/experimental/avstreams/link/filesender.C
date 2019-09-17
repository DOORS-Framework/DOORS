//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
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

$Log: filesender.C,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/08/31 13:59:00  ik
AVStreams changes and modifications to configure.in accordingly.

Revision 1.1  2001/08/14 15:29:19  ik
TCP- and UDP-links are partly rewritten and structure is changed.

Revision 1.2  2001/06/08 16:46:47  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:54  bilhanan
Full licence clause added into doors 0.1


*/

#include <doors/doorsconf.h>
#include <assert.h>
#include <string>
#include <doors/filesender.h>

#define ASSERT(param) assert((param))

FileSender::FileSender()
  : paused_(false), stopped_(true)
{ 
}

FileSender::FileSender(std::string filename)
  : paused_(false), stopped_(true)
{
  inputstream_.open(filename.c_str());
}

FileSender::FileSender(int fd)
  : paused_(false), stopped_(true)
{
  // attach() is not in Solaris:
  //  inputstream_.attach(fd); // Attach to file descriptor
}

FileSender::~FileSender()
{
  if (inputstream_.is_open()) {
    inputstream_.close();
  }
}

bool 
FileSender::sndr_reopen(int fd) 
{
  // attach() is not in Solaris:
  //  if (inputstream_.is_open()) {
  //    inputstream_.close();
  //  }
  //  inputstream_.attach(fd);
  //  return inputstream_.is_open();
  return false;
}

bool 
FileSender::sndr_reopen(std::string url) 
{
  if (inputstream_.is_open()) {
    inputstream_.close();
  }
  if (url.substr(0, 5) == "file:") {
    inputstream_.open(url.substr(5, std::string::npos).c_str());
  } else if (url.substr(0, 3) == "fd:") {
    return sndr_reopen(atoi(url.substr(3, std::string::npos).c_str()));
  }
  return inputstream_.is_open();
}

bool 
FileSender::sndr_is_naptime()
{
  if ((!inputstream_.is_open()) || (paused_) || (stopped_)) {
    return true;
  }
  return false;
}
