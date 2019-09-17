//  Copyright 2004
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.

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
  
  $ Log: $

*/

#include "test_logger.h"
#include <doors/debug.h>

TestLogger * TestLogger :: _instance = 0;

TestLogger :: TestLogger() {

    std::string file_name;

    if( getenv("VALOGFILE") ) {
        file_name = getenv("VALOGFILE");
    } else {
        file_name = "va_test_log.out";
    }

    log_file.open( file_name.c_str() );

    logTime();
    log_file << "TestLogger initialised" << std::endl;

}

void TestLogger :: logTime() {

    if( gettimeofday( &t, 0 ) == -1 ) {
        _DOORS_ERROR(0, "Error getting time of day in TestLogger::logTime()");
    }

    log_file << "At " << t.tv_sec << ", " << t.tv_usec << ": ";

}

TestLogger * TestLogger :: Instance() {

        if( !_instance ) {

            _instance = new TestLogger();

        }


  return _instance;
}

void TestLogger :: LogEvent( std::string s ) {

    logTime();
    log_file << s << std::endl;
  
}

TestLogger :: ~TestLogger() {
    log_file.close();
}
