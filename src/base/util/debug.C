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

$Log: debug.C,v $
Revision 1.6  2003/05/15 09:07:46  bilhanan
std c++ .h headers deprecated, ostrstream replaced with ostringstream.

Revision 1.5  2002/08/14 09:25:58  bilhanan
ONS_Logger is now DOORS_Logger.

Revision 1.4  2002/07/30 13:29:12  bilhanan

initialisation and assignment bugfix for std::string types.

Revision 1.3  2002/07/25 10:08:40  ik
Use std::string.

Revision 1.2  2002/05/11 21:01:24  ik
Useless #include removed as HP-UX aCC fix.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:35  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.5  2001/06/08 08:39:35  bilhanan
Full licence clause added into doors 0.1


*/


#include <iostream>
#include <string>
#include <doors/debug.h>
#include <stdlib.h>

DOORS_Logger *DOORS_Logger::_instance = 0;

//
//Function: instance
//
//Member-Of: DOORS_Logger
//
//Return: DOORS_Logger *
//
//Description:
//	
//  Function for creating instance of DOORS_Logger.
//

DOORS_Logger *DOORS_Logger::instance() 
{
    if(!_instance) {
	_instance = new DOORS_Logger();
    }
    return _instance;

}



DOORS_Logger::DOORS_Logger()
    : _line(0),_type_ch(0),_stream(0)
{
}

//
//Function: at_position
//
//Member-Of: DOORS_Logger
//
//Parameters:
//     std::string, unsigned long
//
//Return: DOORS_Logger &
//
//Description:
//	
// Set position variables.
//

DOORS_Logger &DOORS_Logger::at_position(std::string file,unsigned long line)
{
    _file = file;
    _line = line;
    return *this;
}

//
//Function: type
//
//Member-Of: DOORS_Logger
//
//Parameters:
//     std::string , char
//
//Return: DOORS_Logger &
//
//Description:
//	
// Set type variables.
//

DOORS_Logger &DOORS_Logger::type(std::string str,char ch)
{
    _type_str = str;
    _type_ch = ch;
    return *this;
}

//
//Function: expr_str
//
//Member-Of: DOORS_Logger
//
//Parameters:
//     std::string
//
//Return: DOORS_Logger &
//
//Description:
//	
// Set string variables.
//

DOORS_Logger &DOORS_Logger::expr_str(std::string str)
{
    _expr_str = str;
    return *this;
}

//
//Function: stream
//
//Member-Of: DOORS_Logger
//
//Return: oststream &
//
//Description:
//	
// strstream cannot cleared, so creat new one each tracing time
//  

std::ostringstream &DOORS_Logger::stream()
{
    if(!_stream) {
	_stream = new std::ostringstream();
    }
    return *_stream;
}

//
//Function: terminate
//
//Member-Of: DOORS_Logger
//
//Description:
//	
// terminate the execution of program.
//  

void DOORS_Logger::terminate()
{
#if _LOG_LEVEL < _LOG_LEVEL_ASSERT
    exit(3);
#else
    abort();
#endif
}

//
//Function: flush
//
//Member-Of: DOORS_Logger
//
//Return: DOORS_Logger&
//
//Description:
//	
// Flush log text and reset variables.
//  

DOORS_Logger &DOORS_Logger::flush()
{
    log(_file,_line,_stream->str());
    //    _stream->rdbuf()->freeze(0);  -- no freeze() call in buffers, with ostringstream
    delete _stream;
    _stream = 0;


    _file.erase( _file.begin(), _file.end());
    _line = 0;
    _type_str.erase( _type_str.begin(), _type_str.end());
    _type_ch = 0;
    _expr_str.erase( _expr_str.begin(), _expr_str.end());
    return *this;
}

//
//Function: log
//
//Member-Of: DOORS_Logger
//
//Parameters:
//     std::string, unsigned long, std::string
//
//Description:
//	
// Write text to specified log file with comments. 
//  

void DOORS_Logger::log(std::string file,unsigned long line,std::string comment)
{
  std::cerr << "%" << _type_str << "[" << _expr_str << "];" << file 
	    << ";" << line << ";" << comment << std::endl;
}


