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

$Log: obl_string.h,v $
Revision 1.2  2003/05/15 09:09:20  bilhanan
std c++ .h headers deprecated, std:: namespace added.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:47  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:38  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef _obl_string_h
#define _obl_string_h 1

#include <iostream>
#include <doors/obl_dcl.h>

//+ obl_String
//. obl_String provides operations for basic strings management.
//:binary operators:
//. Comparation operators: ==,!=,<,>,<= and >= with overloading
//. (const obl_String &,const obl_String &),
//. (const char *,const obl_String &) and (const obl_String &,const char *)
//. are provides as binary operators. 
//. There are 6 * 3 = 18 binary operators for obl_String.
class obl_String {
public:
    //+ types
    //'syntax:
    typedef char *iterator;
    //-

    //+ create
    //. Create a new obl_String by default constructor, copy constructor
    //. or by c-string as a parameter.
    //'syntax:
    obl_String();
    obl_String(const obl_String &s);
    obl_String(const char *cs);
    //-

    virtual ~obl_String();

    //+ assignment
    //. Assing a obl_String or c-string to this obl_String.
    //. Same operator semantics as int.
    //'syntax:
    obl_String &operator=(const obl_String &s);
    obl_String &operator=(const char *cs);
    //-

    //+ append
    //. Operator += append a s or a cs to end of string of this.
    //'syntax:
    obl_String &operator+=(const obl_String &s);
    obl_String &operator+=(const char *cs);
    obl_String &operator+=(const char ch);
    //-

    //+ management
    //'syntax:
    obl_String &erase();
    //-

    //+ information
    //. size() returns number or characters in obl_String
    //. (not including the terminating null).
    //.
    //. c_str() return a pointer to c-string.
    //'syntax:
    int size() const {return _size;}
    const char *c_str() const {return _value;} // naming DWP:21.1.1.9
    //-

    //+ equality
    //. compare() has same functionality as strcmp(3).
    //:return:
    //. < 0 if *this < s,cs. 0 if *this == s,cs. > 0 if *this > s,cs.
    //'syntax:
    int compare(const obl_String &s) const; // naming DWP:21.1.1.9.8
    int compare(const char *cs) const;
    //-

    //+ iterators
    //'syntax:
    iterator begin() const {return _value;}
    iterator end() const {return _value + _size;}
    //-

    //+ old Name interface
    //. set() and get() functions are for backward compability for
    //. Name class. These functions should not to be used. Use
    //. operator=() and c_str() instead.
    //'syntax:
    const char *get() const {return _value;}
    void set(const char *s) {operator=(s);}
    //-
private:
    char *_value; // character array, including terminating null
    int _size;    // number of characters, without terminating null
}; 

//+ ostream extraction operator
//. obl_String extraction operator for ostream.
//:syntax:
//. ostream &operator<<(ostream &os,const obl_String &s)
//-

inline std::ostream &operator<<(std::ostream &os,const obl_String &s)
{
    return os << s.c_str();
}

// String binary operations //////////////////////////////////////////

//+ equality operator
//. int semantic binary operator== for obl_String.
//:conform: DWP: 21.1.1.10.2
//:syntax:
//. bool operator==(const obl_String &lhs,const obl_String &rhs)
//. bool operator==(const char *lhs,const obl_String &rhs)
//. bool operator==(const obl_String &lhs,const char *rhs)
//- 

inline bool operator==(const obl_String &lhs,const obl_String &rhs)
{
    return lhs.compare(rhs) == 0;
}

inline bool operator==(const char *lhs,const obl_String &rhs)
{
    return rhs.compare(lhs) == 0;
}

inline bool operator==(const obl_String &lhs,const char *rhs)
{
    return lhs.compare(rhs) == 0;
}

//+ inequality operator
//. int semantic binary operator!= for obl_String.
//:conform: DWP: 21.1.1.10.3
//:syntax:
//. bool operator!=(const obl_String &lhs,const obl_String &rhs)
//. bool operator!=(const char *lhs,const obl_String &rhs)
//. bool operator!=(const obl_String &lhs,const char *rhs)
//-

inline bool operator!=(const obl_String &lhs,const obl_String &rhs)
{
    return lhs.compare(rhs) != 0;
}

inline bool operator!=(const char *lhs,const obl_String &rhs)
{
    return rhs.compare(lhs) != 0;
}

inline bool operator!=(const obl_String &lhs,const char *rhs)
{
    return lhs.compare(rhs) != 0;
}

//+ less operator
//. int semactic binary operator< for obl_String.
//:conform: DWP: 21.1.1.10.4
//:syntax:
//. bool operator<(const obl_String &lhs,const obl_String &rhs)
//. bool operator<(const char *lhs,const obl_String &rhs)
//. bool operator<(const obl_String &lhs,const char *rhs)
//-

inline bool operator<(const obl_String &lhs,const obl_String &rhs)
{
    return lhs.compare(rhs) < 0;
}

inline bool operator<(const char *lhs,const obl_String &rhs)
{
    return rhs.compare(lhs) > 0;
}

inline bool operator<(const obl_String &lhs,const char *rhs)
{
    return lhs.compare(rhs) < 0;
}

//+ greater operator
//. int semactic binary operator> for obl_String
//:conform: DWP: 21.1.1.10.5
//:syntax:
//. bool operator>(const obl_String &lhs,const obl_String &rhs)
//. bool operator>(const char *lhs,const obl_String &rhs)
//. bool operator>(const obl_String &lhs,const char *rhs)
//-

inline bool operator>(const obl_String &lhs,const obl_String &rhs)
{
    return lhs.compare(rhs) > 0;
}

inline bool operator>(const char *lhs,const obl_String &rhs)
{
    return rhs.compare(lhs) < 0;
}

inline bool operator>(const obl_String &lhs,const char *rhs)
{
    return lhs.compare(rhs) > 0;
}

//+ less or equal operator
//. int semactic binary operator <= for obl_String.
//:conform: DWP: 21.1.1.10.6
//:syntax:
//. bool operator<=(const obl_String &lhs,const obl_String &rhs)
//. bool operator<=(const char *lhs,const obl_String &rhs)
//. bool operator<=(const obl_String &lhs,const char *rhs)
//.

inline bool operator<=(const obl_String &lhs,const obl_String &rhs)
{
    return lhs.compare(rhs) <= 0;
}

inline bool operator<=(const char *lhs,const obl_String &rhs)
{
    return rhs.compare(lhs) >= 0;
}

inline bool operator<=(const obl_String &lhs,const char *rhs)
{
    return lhs.compare(rhs) <= 0;
}

//+ greater or equal operator
//. int semantic binary opeartor>= for obl_String.
//:conform: DWP: 21.1.1.10.7
//:syntax:
//. bool operator>=(const obl_String &lhs,const obl_String &rhs)
//. bool operator>=(const char *lhs,const obl_String &rhs)
//. bool operator>=(const obl_String &lhs,const char *rhs)
//-

inline bool operator>=(const obl_String &lhs,const obl_String &rhs)
{
    return lhs.compare(rhs) >= 0;
}

inline bool operator>=(const char *lhs,const obl_String &rhs)
{
    return rhs.compare(lhs) <= 0;
}

inline bool operator>=(const obl_String &lhs,const char *rhs)
{
    return lhs.compare(rhs) <= 0;
}

//- obl_String
#endif // _obl_string_h

