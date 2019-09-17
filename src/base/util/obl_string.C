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

$Log: obl_string.C,v $
Revision 1.3  2002/08/14 09:31:42  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/06/03 09:19:55  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:37  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:38  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/obl_string.h>
#include <doors/debug.h>
#include <string.h>
#include <stdlib.h>

obl_String::obl_String()
    : _value(0),_size(0)
{}

obl_String::obl_String(const obl_String &s)
   : _value(0),_size(0)
{
    if(s._value) {
	_size = s._size;
	_value = new char[_size+1];
	_DOORS_ERROR(_value,"obl_String allocation error");
	memcpy(_value,s._value,_size+1);
    }
}

obl_String::obl_String(const char *cs)
    : _value(0),_size(0)
{
    if(cs) {
	_size = strlen(cs);
	_value = new char[_size+1];
	_DOORS_ERROR(_value,"obl_String allocation error");
	memcpy(_value,cs,_size+1);
    }
}

obl_String :: ~obl_String ()
{
    delete [] _value;
}

obl_String &obl_String::operator=(const obl_String &s)
{
    if(s._value) {
	if(this != &s) {
	    delete [] _value;
	    _size = s._size;
	    _value = new char[_size+1];
	    _DOORS_ERROR(_value,"allocation error");
	    memcpy(_value,s._value,_size+1);
	}
    } else {
	_value = 0;
	_size = 0;
    }
    return *this;
}

obl_String &obl_String::operator=(const char *cs)
{
    delete [] _value;
    if(cs) {
	_size = strlen(cs);
	_value = new char[_size+1];
	_DOORS_ERROR(_value,"allocation error");
	memcpy(_value,cs,_size+1);
    } else {
	_value = 0;
	_size = 0;
    }
    return *this;
}

obl_String &obl_String::operator+=(const obl_String &s)
{
    return (*this) += s.c_str();
}

obl_String &obl_String::operator+=(const char *cs)
{
    if(cs) {
	int asize = strlen(cs);
	int osize = _size;
	char *ovalue = _value;
	_size = osize + asize;
	_value = new char[_size + 1];
	_DOORS_ERROR(_value,"allocation error");
	memcpy(_value,ovalue,osize);
	memcpy(_value + osize,cs,asize + 1);
	delete [] ovalue;
    }
    return *this;
}

obl_String &obl_String::operator+=(const char ch)
{
    if(ch) {
	int osize = _size;
	char *ovalue = _value;
	_size = osize + 1;
	_value = new char[_size + 1];
	_DOORS_ERROR(_value,"allocation error");
	memcpy(_value,ovalue,osize);
	_value[_size - 1] = ch;
	_value[_size] = 0;
	delete [] ovalue;
    }
    return *this;
}

obl_String &obl_String::erase()
{
    delete [] _value;
    _value = 0;
    _size = 0;
    return *this;
}

int obl_String::compare(const obl_String &s) const
{
    if(!this->c_str() && !s.c_str()) return 0;
    if(!this->c_str() || !s.c_str()) return 1;
    return strcmp(this->c_str(),s.c_str());
}

int obl_String::compare(const char *cs) const
{
    if(!this->c_str() && !cs) return 0;
    if(!this->c_str() || !cs) return 1;
    return strcmp(this->c_str(),cs);
}

