// 
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

$Log: otask.C,v $
Revision 1.4  2004/05/25 09:13:52  bilhanan
setName() introduced.

Revision 1.3  2002/08/14 09:31:41  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/07/24 13:41:24  ik
Use std::string.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:24  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:20  bilhanan
Full licence clause added into doors 0.1


*/

#include <string>
#include <doors/library.h>
#include <doors/otask.h>
#include <doors/osched.h>
#include <doors/ttask.h>

Otask :: Otask (std::string n, Scheduler *s) 
  : name(n), scheduler (s), priority (DOORS_PRIORITY_NORMAL), requestCounter (0)
{
  _MT_SYNCMONITOR;
  _MT_ASYNCMONITOR;
  _MT_RESET_ALIVE;

  if (scheduler) {
    scheduler->inform (this);
  }
}

Otask :: ~Otask (void)
{
  _MT_SYNCMONITOR;
  _MT_ASYNCMONITOR;

  if (scheduler) {
    scheduler->forget (this);
  }
  _MT_RESET_ALIVE;
  _MT_RELEASE_SYNCMONITOR;
  _MT_RELEASE_ASYNCMONITOR;

  _MT_SIGNAL_RUNLOCK;
  _MT_WAIT_EXITBARRIER(this);
}

IoHandler *Otask :: getIoHandler (void) const
{
  _MT_SYNCMONITOR;
  _MT_ASYNCMONITOR;

  return scheduler ? scheduler->getIoHandler () : 0;
}

TimerTask *Otask :: getTimerTask (void) const
{
  _MT_SYNCMONITOR;
  _MT_ASYNCMONITOR;

  return TimerTask :: instance();
}

std::string Otask :: getName (void) const
{
  _MT_SYNCMONITOR;

  return name;
}


void Otask :: setName (std::string newName)
{

  name = newName;
  return;
}



