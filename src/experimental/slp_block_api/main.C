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

$Log: main.C,v $
Revision 1.1  2004/08/03 05:31:07  hartmanj
Initial revision


*/

#include <iostream>
#include "minislp.h"


SLPBoolean findsrvs_callback(SLPHandle hSLP,
                                     const char* pcSrvURL,
                                     unsigned short,
                                     SLPError err,
                                     void *)
{
    if( err == SLP_OK )
        std::cout << "Service found: " << pcSrvURL << std::endl;
    else
        std::cerr << "Error: " << err << std::endl;

    return SLP_TRUE;
}


int main( int argc, char *argv[] ) {


    if( argc >= 2 ) {

        SLPHandle handle;
        SLPError res = SLPOpen( "en", SLP_FALSE, &handle );

        if( res != SLP_OK ) {
            std::cout << "SLPOpen failed. Error " << res << std::endl;
            return 0;
        }

        for( unsigned int i = 1; i < argc; i++ ) {

            SLPFindSrvs( handle, argv[i], "default", "", findsrvs_callback, 0 );

        }

        std::cout << "The fun has ended" << std::endl;

        SLPClose( handle );

    } else {

        std::cerr << "Usage: '" << argv[0]
                  << " <srvtype1> [<srvtype2> ... ]'" << std::endl;

    }



    return 0;

}


