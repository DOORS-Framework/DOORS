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

$Log: otime.h,v $
Revision 1.2  2003/05/15 09:09:20  bilhanan
std c++ .h headers deprecated, std:: namespace added.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:49  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.5  2001/06/08 08:39:39  bilhanan
Full licence clause added into doors 0.1


*/



#ifndef OTIME_H
#define OTIME_H


#include <sys/time.h>
#include <iostream>
#include <doors/obl_dcl.h>


/**
  This is a basic class that is supposed to hold a time value. 
  OTime class is designed to be used in DOORS' i/o handler 
  that needs a time type that is capable to handle time in
  micro seconds. Time is normalized all the time. This is done 
  by the member functions. 
  
  This time class is designed to be used in the i/o handler. 
  This is partly implemented because the system dependant
  time structures are wanted to hide. 

  @see Timer
  */
class OTime : public timeval {
public:
    /** Default constructor */
    OTime (void);

    /**
      The constructor takes an initial time and normalizes it
      into its internal variables. 

      @param seconds The parameter seconds is an initial time
      @param micros is an initial time, default zero.
      */
    OTime (long seconds, long micros = 0);

    /** Copy constructor from OTime */
    OTime (const OTime &other);
    
    /**
      The assigment operator copies the given time in an OTime type
      variable to itself.
      @param rhs The paramter rhs is another time that is copied.
      @return Returns a reference to itself.
      */
    OTime &operator = (const OTime &rhs);

    /**
      The assigment operator copies the given time in a double type
      variable to itself. Note that precision can produce some
      problem when handling very large or small numbers.
      @return Returns a reference to itself.
      */
    OTime &operator = (const double other);

    /** Return difference between this and other time */
    OTime operator - (const OTime &other) const;

    /** Return addition of this and other time */
    OTime operator + (const OTime &other) const;

    /** Return addition of this and float time presentation */ 
    OTime operator + (const double other) const;

    /**
      Compares this time and the time given as parameter.
      Returns non-zero value if this time is less or equal than
      the other time. Otherwise, returns zero.
      */
    int operator <= (const OTime &other) const;

    /**
      Compares this time and the time given as parameter.
      Returns non-zero value if this time is greater or equal than
      the other time. Otherwise, returns zero.
      */
    int operator >= (const OTime &other) const;
    
    /**
      Compares this time and the time given as parameter.
      Returns non-zero value if this time is greater than
      the other time. Otherwise, returns zero.
      */
    int operator > (const OTime &other) const;

    /**
      Compares this time and the time given as parameter.
      Returns non-zero value if this time is greater or equal than
      the other time. Otherwise, returns zero.
      */
    int operator > (const double other) const;

    /**
      Compares this time and the time given as parameter.
      Returns non-zero value if the times are not equal. Otherwise,
      returns zero.
      */
    int operator == (const OTime &other) const;

    /**
      Compares this time and the time given as parameter.
      Returns non-zero value if the times are not equal. Otherwise,
      returns zero.
      */
    int operator == (const double other) const;
    
    /** Return second psrt from time */
    long getSeconds (void) const;

    /** Return microsecond part from time. */
    long getMicros (void) const;
};

inline OTime :: OTime (void) 
{
    tv_sec = tv_usec = 0;
}

inline long OTime :: getSeconds (void) const
{
    return tv_sec + tv_usec/1000000;
}

inline long OTime :: getMicros (void) const
{
    return tv_usec%1000000;
}

inline bool operator<(const OTime &lhs,const OTime &rhs)
{    
    return 
	(lhs.tv_sec < rhs.tv_sec) ||
	(lhs.tv_sec == rhs.tv_sec) && 
	(lhs.tv_usec < rhs.tv_usec);
}

/** ostream extraction operator
  Extract OTime into ostream as in format "[YY/MM/DD HH:MM:SS uS]".
  */
extern std::ostream &operator<<(std::ostream &os,const OTime &arg);

#endif // OTIME_H

