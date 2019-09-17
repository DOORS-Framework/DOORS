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

$Log: errmgmt.h,v $
Revision 1.2  2002/07/25 10:08:41  ik
Use std::string.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:28  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:36  bilhanan
Full licence clause added into doors 0.1


*/


#if ! _errmgmt_h
#define _errmgmt_h 1

#include <string>
#include <doors/obl_dcl.h>

//Class: ErrorManager
//
//Description:
//  Interface abstract class to the error manager implementations.
//
class ErrorManager {
public:
    // interfaces
    virtual void error(std::string) = 0;
    virtual void fatal(std::string) = 0;
    virtual void info(std::string) = 0;
    virtual void showInfo() = 0;
    virtual void hideInfo() = 0;
    virtual void warn(std::string) = 0;
    virtual void showWarn() = 0;
    virtual void hideWarn() = 0;
};

//Name: error
//Type: function
//Context: ErrorManager
//View: public
//Parameters:
//  std::string msg
//Description:
//  Error message to ErrorManager, there might be a limit of number
//  in errormessages util application makes exit.

//Name: fatal
//Type: function
//Context: ErrorManager
//View: public
//Parameters:
//  std::string msg
//Description:
//  Fatal message to ErrorManager, application has entered into
//  situation from where it cannot recover. The execution of
//  application ends.

//Name: warn
//Type: function
//Context: ErrorManager
//Group: warn
//View: public
//Parameters:
//  std::string msg
//Description:
//  Warning message to ErrorManager, application enter situation,
//  that is may or may not affect the execution of application.
//
//  The futher delegation of warning messages to the displayer can be
//  hided by hideWarn() and show again by showWarn().

//Name: showWarn
//Type: function
//Context: ErrorManager
//Group: warn
//View: public
//Description:
//  showWarn requires the ErrorManager to delegate the warning messages
//  to the displayer.

//Name: hideWarn
//Type: function
//Context: ErrorManager
//View: public
//Group: warn
//Description:
//  hideWarn requires the ErrorManager to stop delegate the warning
//  messages to the displayer

//Name: info
//Type: function
//Context: ErrorManager
//View: public
//Group: info
//Parameters:
//  std::string msg
//Description:
//  Informational message to the ErrorManager, application is in
//  situation, that is wanted to raport to the user.
//
//  Futher delegation of informational messages can be stopped by
//  hideInfo and show again by showInfo.

//Name: hideInfo
//Type: function
//Context: ErrorManager
//View: public
//Group: info
//Description:
//  hideInfo requires ErrorManager to stop delegating messages into
//  displayer.

//Name: showInfo
//Type: function
//Context: ErrorManager
//View: public
//Group: info
//Description:
//  showInfo requires the ErrorManager to continue delegation messages
//  into displayer.

//////////////////////////////////////////////////////////////////////

//Name: DefaultErrorManager
//Type: class
//Context: ErrorManager
//View: private
//Description:
//  Default implementation of ErrorManager.
//
//Class: DefaultErrorManager
//
//Inherited: ErrorManager
//
//Description:
//   A default error manager implementation.
//
class DefaultErrorManager : public ErrorManager {
public:
    // implements
    void info(std::string);
    void warn(std::string);
    void error(std::string);
    void fatal(std::string);
    void hideInfo();
    void showInfo();
    void hideWarn();
    void showWarn();
private:
    uint16 _errors;
    static uint16 maxErrors;
    struct _flags {
	uint16 info : 1;
	uint16 warn : 1;
    } flags;
};

//////////////////////////////////////////////////////////////////////

extern ErrorManager *em;

#ifndef ERROR
#define ERROR(msg) {em->error(msg);}
#endif

#ifndef FATAL
#define FATAL(msg) {em->fatal(msg);}
#endif

#ifndef INFO
#define INFO(msg) {em->info(msg);}
#endif

#ifndef WARN
#define WARN(msg) {em->warn(msg);}
#endif

#endif // _errmgmt_h

