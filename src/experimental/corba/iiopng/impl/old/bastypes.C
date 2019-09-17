//Editor-Info: -*- C++ -*-
//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
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

$Log: bastypes.C,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:19  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.7  2001/06/08 08:40:10  bilhanan
Full licence clause added into doors 0.1


*/

//===========================================================================
//File:     bastypes.C
//Date:     27.09.1999
//Subject:  IIOP ver 1.2
//Desc:   
//
//Author:   Janne Makihonka, TTKK/DMI
//===========================================================================


#include <doors/bastypes.h>


CORBA :: iiopString :: iiopString(void) 
  : maxLen(0), len(0), data(0) 
{
}

CORBA :: iiopString :: iiopString(const Const_Char& c) 
  : maxLen(0) 
{
  const char* d = c.Ptr();
  len = strlen(d);
  if(len){
    len ++;}
  if(len > 0) {
    maxLen = (len > 16)? len : 16;
    data = AllocBuf(maxLen);
  } else {
    data = 0;
  }
  for (CORBA::ULong i = 0; i < len; i++) {
    data[i] = d[i];
  }
}


CORBA::iiopString&  CORBA :: iiopString :: operator = (const CORBA::iiopString &rhs)
{
  if ((this != &rhs) && (data != NULL)) 
    FreeBuf(data);
  
  len = rhs.len;
  maxLen = rhs.maxLen;
  
  if (maxLen > 0) {
    data = AllocBuf(maxLen);
  }
  else
    data = 0;
  
  for (CORBA::ULong i = 0; i < len; i++) {
    data[i] = rhs.data[i];
  }
  return *this;
}


bool CORBA :: iiopString :: operator==(const iiopString& rhs) const
{
  if (this == &rhs) 
    return true;
  
  if (len == rhs.len) {
    if (len)
      return !strcmp(data,rhs.data);
    else 
      return true;
  }
  return false;
}


void CORBA :: iiopString :: Length(CORBA::ULong newLen) 
//change the length of the sequence
{
  if (newLen < len) {
    for (CORBA::ULong i = newLen; i < len; i++) {
      data[i] = 0;
    }
  }
  else if (newLen > maxLen) {
    char* old = data;
    maxLen = newLen > maxLen + 16 ? newLen : maxLen + 16;
    data = AllocBuf(maxLen);
    
    for (CORBA::ULong i = 0; i < len; i++)
      data[i] = old[i];
    
    FreeBuf(old);
  }
  len = newLen;
}




