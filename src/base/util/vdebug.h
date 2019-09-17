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

$Log: vdebug.h,v $
Revision 1.4  2002/07/25 15:38:44  ik
Use std::string.

Revision 1.3  2002/07/25 10:08:41  ik
Use std::string.

Revision 1.2  2002/06/03 09:19:56  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:07  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:40  bilhanan
Full licence clause added into doors 0.1


*/



#ifndef VDEBUG_H
#define VDEBUG_H

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <doors/debug.h>

enum VdebugCtrl {
    endd = 1
};

//
//Class: Vdebug
//
//Description:
//      An output stream class. It can be used to print debugging
//      information. You can switch off output by unsetting the
//      OCONFIG_DEBUG macro, which makes all the operators of Vdebug
//      to empty functions. Standard output is used for printing.
//      Vdebug is independent from the user interface.
//
//Commentary:
//      The operators are hopefully optimized when compiling. 
//
class Vdebug {
    public:
        inline Vdebug &operator<< (const int);
        inline Vdebug &operator<< (const char *);
        inline Vdebug &operator<< (std::string);
        inline Vdebug &operator<< (const double);
        inline Vdebug &operator<< (VdebugCtrl);
        inline void fail (std::string, std::string, const int);
};


// If OCONFIG_DEBUG is not set operators of Vdebug are 
// defined as empty functions.

#ifdef OCONFIG_DEBUG

//
//Operator: <<
//
//Member-Of: Vdebug
//
//Parameters:
//      const int i
//
//Return:
//      Vdebug &
//
//Description:
//      Prints the given integer to the stream. Returns itself,
//      the debug stream.
//
inline Vdebug &Vdebug::operator<< (const int i)
{
    DEBUG1("%d",i);
    return *this;
}


//
//Operator: <<
//
//Member-Of: Vdebug
//
//Parameters:
//      const char *s
//
//Return:
//      Vdebug &
//
//Description:
//      Prints the given string to the stream. Returns itself,
//      the debug stream.
//
inline Vdebug &Vdebug::operator<< (const char *s)
{
    DEBUG1("%s\n",s);
    return *this;
}


//
//Operator: <<
//
//Member-Of: Vdebug
//
//Parameters:
//      std::string
//
//Return:
//      Vdebug &
//
//Description:
//      Prints the given string to the stream. Returns itself,
//      the debug stream.
//
inline Vdebug &Vdebug::operator<< (std::string s)
{
    DEBUG1("%s\n",s.c_str());
    return *this;
}


//
//Operator: <<
//
//Member-Of: Vdebug
//
//Parameters:
//
//Return:
//      Vdebug &
//
//Description:
//      Prints the given double to the stream. Returns itself,
//      the debug stream.
//
inline Vdebug &Vdebug::operator<< (const double d)
{
    DEBUG1("%g",d);
    return *this;
}

//
//Operator: <<
//
//Member-Of: Vdebug
//
//Parameters:
//      VdebugCtrl c
//
//Return:
//      Vdebug &
//
//Description:
//      Prints the given control token to the stream. A control token
//      is for example line feed. Returns itself, the debug stream.
//
inline Vdebug &Vdebug::operator<<(VdebugCtrl c)
{
    switch(c) {
    case endd:
	DEBUG0("\n");
	break;
    default:
	DEBUG0("Vdebug: undefined control\n");
	break;
    }
    return *this;
}


//
//Function: fail
//
//Member-Of: Vdebug
//
//Parameters:
//      std::string expr, std::string file, const int line
//
//Description:
//      Prints the predefined assertion failed text with the given
//      parameters. Exits the program by calling the abort function.
//
inline void Vdebug::fail(std::string expr, std::string file, const int line)
{
    DEBUG3("%s:%d: assertion failed for %s\n", 
	   file.c_str(), line, expr.c_str());
    abort();
}
<<<<<<< vdebug.h

//Macro: ASSERT
//Parameters:
//  expr
//Description:
//  ASSERT macro asserts the expr and if the assertion fails
//  then the execution of program is aborted in meaning
//  for debugging.
//  
//  If the macro OCONFIG_DEBUG is not set the ASSERT macro 
//  generates no code.
//
#if 0
#undef ASSERT
#define ASSERT(expr) \
  ((expr) ? (void)0 : vdbg.fail (#expr, __FILE__, __LINE__))
#endif

=======
>>>>>>> 2.6
#else


// OCONFIG_DEBUG was not set.
inline Vdebug &Vdebug::operator<< (const int) { return *this; }
inline Vdebug &Vdebug::operator<< (const char *) { return *this; }
inline Vdebug &Vdebug::operator<< (std::string) { return *this; }
inline Vdebug &Vdebug::operator<< (const double) { return *this; }
inline Vdebug &Vdebug::operator<< (VdebugCtrl) { return *this; }

inline void Vdebug::fail (std::string, std::string, const int) 
{ 
}

#endif // OCONFIG_DEBUG

extern Vdebug vdbg;

#endif /* VDEBUG_H */

