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

$Log: logger.h,v $
Revision 1.5  2002/08/14 09:31:41  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.4  2002/07/25 10:08:40  ik
Use std::string.

Revision 1.3  2002/07/23 14:25:21  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:53  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:40  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:16  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <doors/util.h>
#include <doors/etask.h>
#include <doors/ttydev.h>
#include <doors/vstream.h>
#include <doors/ocoremac.h>


/**   Logger is a base class for all classes that format output
      from Vostream. */
      
class Logger : public EventTask {
 public:
  /** Constructor of logger 
    @param n Name of logger
    @s scheduler under which the Logger is run.
    */
  Logger (std::string n = "Logger", Scheduler * = 0);

  /** destructor for logger */   
  virtual ~Logger (void) {};

  /** filter formats the output from vostream to device and then writes
    the message to device.    
    */
  virtual void filter (VstreamMessage *) = 0;

  /** The logging is turned on for Vostream::Type type entries.
    @param arg is the type off Vostream. 
    */
  Logger &on (Vostream::Type);

  /** The logging is turned off for Vostream::Type type entries.
    @param arg is the type off Vostream. 
    */
  Logger &off (Vostream::Type);

  /** @return The Port by which the Logger is connected to Vostream.
   */
  Port *getPort (void);

 protected:
  
  Frame buf;
  Port port;
  bool types[Vostream::NROF]; // if off, line off
};

inline Port *Logger::getPort (void)
{
  _MT_SYNCMONITOR;
  return &port;
}


//
//Function: on
//
//Member-Of: Logger
//
//Parameters: 
//      Vostream::Type arg
//
inline Logger &Logger::on (Vostream::Type arg)
{
  _MT_SYNCMONITOR;
  types[arg] = true;
  return *this;
}


inline Logger &Logger::off (Vostream::Type arg)
{
  _MT_SYNCMONITOR;
  types[arg] = false;
  return *this;
}


/** TtyLogger handles the writing the log output to the
    terminal. The output is formatted to human readable form.
    */

class TtyLogger : public Logger {
 public:
  /** Constructor of TtyLogger.
    @param s is the scheduler that is supposed to run this task. 
    @param ioh is the i/o handler of the system. 
    */
  TtyLogger (Scheduler *s, IoHandler *io, std::string file = "/dev/tty");
  /** destructor for TtyLogger */ 
  ~TtyLogger (void);
  /** run function is called when there is messages in loggers queue.
  */
  void run (void);
  /** VstreamMessage is formatted to readable form and printed to 
    terminal. This is called from run function.
    @param msg Is the Vstream message to be written on terminal
    */
  void filter (VstreamMessage *);
  DevIf :: User devIf;
  _MT_KILLALL(&device)
 protected:
  TtyDevice device;
};


/** FileLogger is used for logging events to a file. It is used by
  the user interface, only. 
  */
class FileLogger : public Logger {
 public:
  /** Initializes base classes. 
    @param s is the scheduler that is supposed to run this task. 
    @param ioh is the i/o handler of the system. 
    @param file is the nameof the file that is being read. 
    */
  FileLogger (Scheduler *s, IoHandler *io, std::string file);

  /** destructor of FileLogger. */
  ~FileLogger (void);

  /** The main function of FileLogger, which takes a message
    out off the message queue and calls the filter function to 
    format output and to handle actual device writing. 
    */
  void run (void);

  /** Formats data in the given message and requests the device
    to write it to a file associated with this logger. 
    @param msg VstreamMessage to be formatted and written to device
    */
  void filter (VstreamMessage *);

  DevIf :: User devIf;
  _MT_KILLALL(&device)
 protected:
  FileDevice device;
};

#endif // LOGGER_H 






