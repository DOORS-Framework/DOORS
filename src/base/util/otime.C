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

$Log: otime.C,v $
Revision 1.3  2003/05/15 09:09:20  bilhanan
std c++ .h headers deprecated, std:: namespace added.

Revision 1.2  2002/07/25 10:08:40  ik
Use std::string.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:37  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:39  bilhanan
Full licence clause added into doors 0.1


*/



#include <doors/otime.h>
#include <stdio.h>

OTime :: OTime (long seconds, long micros) 
{
    tv_sec = seconds + micros/1000000;
    tv_usec = micros%1000000;
}

OTime :: OTime (const OTime &other)
{
    *this = other;
}

OTime &OTime :: operator = (const OTime &rhs)
{
    tv_sec = rhs.tv_sec;
    tv_usec = rhs.tv_usec;
    return *this;
}

OTime &OTime :: operator = (const double other)
{
    tv_sec = (long)other;
    tv_usec = (long)((other - (long)other)*1000000);
    return *this;
}

OTime OTime :: operator - (const OTime &other) const
{
    OTime result (tv_sec - other.tv_sec,
                  tv_usec - other.tv_usec);
    // normalize
    while (result.tv_usec < 0) {
	result.tv_usec += 1000000;
	result.tv_sec--;
    }
    return result;
}

OTime OTime :: operator + (const OTime &other) const
{
    OTime result (tv_sec + other.tv_sec + (tv_usec + other.tv_usec)/1000000,
                  (tv_usec + other.tv_usec)%1000000);
    return result;
}

OTime OTime :: operator + (const double other) const
{
    long usecs = (long)((other - (long)other)*1000000);    
    OTime result (tv_sec + (long)other 
                  + (tv_usec + usecs)/1000000,
                  (tv_usec + usecs)%1000000);
    return result;
}

int OTime :: operator <= (const OTime &other) const
{
    if ((tv_sec > other.tv_sec) ||
        ((tv_sec == other.tv_sec) && (tv_usec > other.tv_usec))) {
        return 0;
    }
    return ~0;
}

int OTime :: operator >= (const OTime &other) const
{
    return (*this <= other) ? (*this == other) ? ~0 : 0 : ~0;
}

int OTime :: operator > (const OTime &other) const
{
    if ((tv_sec <= other.tv_sec) || 
        ((tv_sec == other.tv_sec) && tv_usec <= other.tv_usec)) {
        return 0;
    }
    return ~0;
}

int OTime :: operator > (const double other) const
{
    if ((tv_sec > (long)other) || ((tv_sec == (long)other) 
        && (tv_usec > (long)((other - (long)other)*1000000)))) {
        return ~0;
    }
    return 0;
}

int OTime :: operator == (const OTime &other) const
{
    return ((tv_sec == other.tv_sec) 
            && (tv_usec == other.tv_usec)) ? ~0 : 0; 
}

int OTime :: operator == (const double other) const
{
    return ((tv_sec == (long)other) 
            && (tv_usec == (long)((other - (long)other)*1000000))) 
            ? ~0 : 0;
}

std::ostream &operator<<(std::ostream &os,const OTime &arg)
{
    char sbuf[1024+1];
    long ltmp = arg.getSeconds();
    struct tm *tms = gmtime((const time_t *)&ltmp);
    strftime(sbuf,1024,"[%D %T",tms);
    os << sbuf;
    snprintf(sbuf,1024," %d]",arg.getMicros());
    os << sbuf;
    return os;
}


