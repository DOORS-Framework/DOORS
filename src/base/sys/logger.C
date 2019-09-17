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

$Log: logger.C,v $
Revision 1.6  2002/08/14 09:31:41  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.5  2002/08/14 08:43:54  hartmanj
In FileLogger :: filter(VstreamMessage *msg) msg->data to msg->data.c_str()

Revision 1.4  2002/07/25 10:08:40  ik
Use std::string.

Revision 1.3  2002/07/23 14:25:21  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:53  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:46:21  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:16  bilhanan
Full licence clause added into doors 0.1


*/

#include <string>
#include <doors/library.h>
#include <doors/logger.h>
#include <doors/ocoremac.h>
// #include "symtable.h"

//
//Constructor: Logger
//
//Parameters:
//      std::string n,
//      Scheduler *s
//
//Description:
//      A instance of Logger is constructed by given name n and the scheduler
//      s under the Logger is runned.
//
Logger::Logger (std::string n, Scheduler *s)
    : port (this), EventTask (n, s)
{
  _MT_SYNCMONITOR;

  for (int i = 0; i < Vostream::NROF; i++) {
    types [i] = true;
  }
}


//
//Constructor: TtyLogger
//
//Parameters:
//      Scheduler *s,
//      IoHandler *ioh,
//      std::string file

TtyLogger :: TtyLogger (Scheduler *s, IoHandler *ioh, std::string file) 
    : Logger (file, s), device (ioh, this, TtyDevice :: WRITE, 256), devIf(this) 
{
  _MT_SYNCMONITOR;

  devIf.connect (&device.iFace) ;
  device.setWrite (true);
  device.open();
  _MT_SCHEDULE(&device);
}

TtyLogger ::  ~TtyLogger (void) 
{
  _MT_KILLTHREAD(this);
}

//
//Function: filter
//
//Member-Of: TtyLogger
//
//Parameters:
//      VstreamMessage *msg
//
void TtyLogger::filter (VstreamMessage *msg)
{
  _MT_SYNCMONITOR;

  static std::string cbuf;

  if (!types[msg->type]) return;
  
  if (msg) {
    if (msg->type == Vostream::DIRE ) {
      delete msg;
      return;
    }
  }

  cbuf = "";
  for (int i=1; i<msg->level; i++) {
      cbuf += "    ";
  }
  if (!msg->data.empty()) {
      cbuf += msg->data;
  }
  cbuf += "\n";
  
  buf.putLast ((Byte *) cbuf.c_str(), cbuf.length());
    
  device.write (buf);
  
  buf.destroy ();
  
  delete msg;

}


//
//Function: run
//
//Member-Of: TtyLogger
//
void TtyLogger::run (void)
{
  _MT_SYNCMONITOR;
  _MT_RELEASE_SYNCMONITOR;
  _MT_STABLE_MONITOR;
  _MT_ACQUIRE_SYNCMONITOR;

  Message *msg = 0;

  if (event (msg) == UserEvent) 
    {
      if (msg->getTarget () == &devIf) {
	switch (msg->getType()) {
	  case (DevIf :: open) : {
	    delete msg;
	    decrementLoad();
	    return;
	  }
	  case (DevIf :: close) : {
	    IoHandler *io = getIoHandler();
	    if (io)
	      io->forget(&device);
	    delete msg;
	    decrementLoad();
	    return;
	  }
	  case (DevIf :: read) : {
	    delete msg;
	    decrementLoad();
	    return;
	  }
	}
	
      }
      else 
	filter ((VstreamMessage *) msg);
    }
  decrementLoad();
}



// - the max leght of names is 5
static const char *loggerTypes[] = {
    "EMPTY", // in any case empty log-message
    "END",   // ending log-message
    "VAR",   // general variable data e.g. frame
    "MSG",   // log-message from the message
    "TASK",  // log-message of task
    "INFO",  // informationnal log
    "USER",  // user data log
    "ERROR", // error log-messages
    "INPUT", // input from user
    "DIRE"   // direct output to stdout
};



//
//Constructor: FileLogger
//
//Parameters:
//      Scheduler *s,
//      IoHandler *ioh,
//      std::string file
//
//Description:
//      Initializes base classes.
//
//      The parameter s is the scheduler that is supposed to run this
//      task. Ioh is the i/o handler of the system. File is the name
//      of the file that is being read. 
//
FileLogger::FileLogger (Scheduler *s, IoHandler *ioh, std::string file) 
  : Logger (file, s), device (ioh, this, file, 256, Device :: WRITE), 
    devIf (this)
{
  _MT_SYNCMONITOR;

  devIf.connect(&device.iFace);
  device.open();
  _MT_SCHEDULE(&device);
}


FileLogger :: ~FileLogger (void) 
{
  _MT_SYNCMONITOR;
}

//
//Function: filter
//
//Member-Of: FileLogger
//
//Parameters:
//      VstreamMessage *msg
//
//Description:
//      Formats data in the given message and requests the device
//      to write it to a file associated with this logger. 
//
void FileLogger :: filter(VstreamMessage *msg)
{
  _MT_SYNCMONITOR;

  static char cbuf[GBUFSIZE+1];
  char *p = cbuf;

  snprintf(cbuf,GBUFSIZE+1,"%s:%d:%s\n",loggerTypes[msg->type],msg->level,msg->data.c_str());

  buf.putLast ((Byte *) cbuf, strlen (cbuf));

  device.write(buf);

  buf.destroy();
}


//
//Function: run
//
//Member-Of: FileLogger
//
//Description:
//      The main function of FileLogger, which takes a message
//      out off the message queue and calls the filter function to 
//      format output and to handle actual device writing. 
//
void FileLogger :: run (void)
{
  _MT_SYNCMONITOR;
  _MT_RELEASE_SYNCMONITOR;
  _MT_STABLE_MONITOR;
  _MT_ACQUIRE_SYNCMONITOR;

  Message *msg = 0;

  switch (event (msg)) {
   case SystemEvent:
    // got empty SystemEvent !
    break;
   case UserEvent:
    {
      if (msg->getTarget () == &devIf) {
	switch (msg->getType()) {
	  case (DevIf :: open) : {
	    break;
	  }
	  case (DevIf :: close) : {
	    IoHandler *io = getIoHandler();
	    if (io)
	      io->forget(&device);
	    break;
	  }
	  case (DevIf :: read) : {
	    break;
	  }
	}
      }
      else 
	filter((VstreamMessage *) msg);
      break;
    }
  default:
    ; // got empty event !
  }
  delete msg;  
  decrementLoad();
}









