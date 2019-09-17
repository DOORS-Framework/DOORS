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

$Log: debug.h,v $
Revision 1.5  2003/11/20 10:59:36  bilhanan
macro conditional checks altered, clutter removed.

Revision 1.4  2003/05/15 09:07:46  bilhanan
std c++ .h headers deprecated, ostrstream replaced with ostringstream.

Revision 1.3  2002/08/14 09:25:58  bilhanan
ONS_Logger is now DOORS_Logger.

Revision 1.2  2002/07/25 10:08:41  ik
Use std::string.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:26  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:35  bilhanan
Full licence clause added into doors 0.1


*/

#if ! _debug_h
#define _debug_h 1

#include <string>
#include <doors/doorsconf.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <sstream>

#define _ER_FAULT 1
#define _ER_ERROR 2
#define _ER_DEBUG 3

//+ DOORS_Logger
//. DOORS_Logger is reposible for collecting development tracing messages
//. sended from macros and propagating them to various output devices.
class DOORS_Logger {
public:
    //+ instance
    //:example:
    //. DOORS_Logger *logger = DOORS_Logger::instance();
    //'syntax:
    static DOORS_Logger *instance();
    //-

    //+ at_position
    //:example:
    //. logger->at_position(__FILE__,__LINE__);
    //'syntax:
    DOORS_Logger &at_position(std::string file,unsigned long line);
    //-

    //+ type
    //'syntax:
    DOORS_Logger &type(std::string str,char ch);
    //-

    //+ expr_str
    //'syntax:
    DOORS_Logger &expr_str(std::string str);
    //-

    //+ stream
    //:example:
    //. #define DEBUG(str) \
    //. logger->stream() << str << std::ends;
    //.
    //. and in macro call:
    //.  DEBUG("coordinates: (" << x << ',' << y << ")");
    //'syntax:
    std::ostringstream &stream();
    //-

    //+ flush
    //:example:
    //. #define DEBUG() \
    //. ... printing ...
    //. logger->flush();
    //'syntax:
    DOORS_Logger &flush();
    //-

    //+ terminate
    //:example:
    //. #define ASSERT(expr) \
    //. ... if expr fails ...
    //. logger->terminate();
    //'syntax:
    void terminate();
    //-

    //+ log
    void log(std::string file,unsigned long line, std::string comment);
    //-

private:
    DOORS_Logger();
    std::string _file;            // trace filename
    unsigned long _line;          // trace line
    std::string _type_str;
    char _type_ch;
    std::string _expr_str;
    std::ostringstream *_stream;          // stream to write user trace messages
    static DOORS_Logger *_instance; // this singleton
};
//- DOORS_Logger

// macros ////////////////////////////////////////////////////////////

//+ _DOORS_DEBUG
//. Show out debugging and program tracing information.
//:syntax:
//. _DOORS_DEBUG(expr,comment)
//:parameters:
//. expr - C++ expression, see assert(3)
//.  
//. comment - insertion operations for strstream, see strstream(3C++)
//.
//:example:
//. BaseComponenent::BaseComponent(std::string name) {
//.    _DOORS_DEBUG(0,"BaseComponent(" << name << ")");
//. }
//. ...
//. BaseComponenent::~BaseComponent() {
//.    _DOORS_DEBUG(0,"~BaseComponent(" << name << ")");
//. }
//-
#define _DOORS_DEBUG(expr,comment)                                   \
do {                                                              \
    if(!(expr)) {                                                 \
	DOORS_Logger *logger = DOORS_Logger::instance();              \
	logger->at_position(__FILE__,__LINE__);                   \
        logger->type("debug",'D');				  \
        logger->expr_str(#expr);                                  \
	logger->stream() << comment << std::ends;                      \
	logger->flush();                                          \
    }                                                             \
} while(0)


#if ! _CF_DEBUG
#undef _DOORS_DEBUG
#define _DOORS_DEBUG(expr,comment) /* NULL _DOORS_DEBUG */
#endif


//+ _DOORS_ERROR
//. Declare user error for library usage.
//:syntax:
//. _DOORS_ERROR(expr,comment)
//:parameters:
//. expr - C++ expression, see assert(3)
//.  
//. comment - insertion operations for strstream, see strstream(3C++)
//.
//:example:
//. void attach(Object *object) {
//.   _DOORS_ERROR(object,"no object given");
//.   ...
//. }
//-
#define _DOORS_ERROR(expr,comment)                                   \
do {                                                              \
    if(!(expr)) {                                                 \
	DOORS_Logger *logger = DOORS_Logger::instance();              \
	logger->at_position(__FILE__,__LINE__);                   \
        logger->type("error",'E');				  \
        logger->expr_str(#expr);                                  \
	logger->stream() << comment << std::ends;                      \
	logger->flush();                                          \
        logger->terminate();                                      \
    }                                                             \
} while(0)
#if ! _CF_ERROR
#undef _DOORS_ERROR
#define _DOORS_ERROR(expr,comment) /* NULL _DOORS_ERROR */
#endif

//+ _DOORS_FAULT
//. Declare software failure and terminate system in wanted.
//:syntax:
//. _DOORS_FAULT(expr,comment)
//:parameters:
//. expr - C++ expression, see assert(3)
//.  
//. comment - insertion operations for strstream, see strstream(3C++)
//.
//:example:
//. void Observer::update() {
//.   _DOORS_FAULT(_subject,"no subject to observe");
//.   .. }
//.
//. if((fd = open(file,O_RDONLY)) < 0) 
//.     _DOORS_FAULT(0,"no error management for open, file:" << file);
//-
#define _DOORS_FAULT(expr,comment)                                   \
do {                                                              \
    if(!(expr)) {                                                 \
	DOORS_Logger *logger = DOORS_Logger::instance();              \
	logger->at_position(__FILE__,__LINE__);                   \
        logger->type("fault",'F');				  \
        logger->expr_str(#expr);                                  \
	logger->stream() << comment << std::ends;                      \
	logger->flush();                                          \
        logger->terminate();                                      \
    }                                                             \
} while(0)
#if ! _CF_FAULT
#undef _DOORS_FAULT
#define _DOORS_FAULT(expr,comment) /* NULL _DOORS_FAULT */
#endif

// old debug stuff, do NOT use ///////////////////////////////////////

#endif /* _debug_h */
