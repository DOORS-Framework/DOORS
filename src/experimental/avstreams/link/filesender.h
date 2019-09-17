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

$Log: filesender.h,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.3  2001/08/21 16:51:23  ik
Changes to AVStreams.

Revision 1.2  2001/08/17 13:41:40  ik
Changes to #include's. There was an old version of filemas.h, changed.

Revision 1.1  2001/08/15 12:57:04  ik
Added some include files for AVStreams/link.

Revision 1.2  2001/06/08 16:45:30  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:54  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef FILESENDER_H
#define FILESENDER_H

#include <string>
#include <fstream>
#include <doors/streamer.h>

class FileSender : public Streamer
{
public:
  FileSender();
  FileSender(std::string filename);
  FileSender(int fd);
  ~FileSender();

  inline bool sndr_is_streaming() 
    { return (bool)(inputstream_.is_open() ? true : false); }

  bool sndr_reopen(std::string url);

  void sndr_start() { paused_ = false; stopped_ = false; }
  void sndr_pause() { paused_ = true; stopped_ = false; }
  void sndr_stop() { paused_ = false; stopped_ = true; }
  void sndr_rewind() { inputstream_.seekg(0); }
  void sndr_repos_abs(std::streampos pos) 
    { inputstream_.seekg(pos, std::ios::beg); }
  void sndr_repos_rel(std::streampos pos)
    { inputstream_.seekg(pos, std::ios::cur); }
  bool sndr_is_naptime();

protected:
  std::ifstream inputstream_;

private:
  bool sndr_reopen(int fd);
  bool paused_;
  bool stopped_;
};

#endif
