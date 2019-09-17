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

$Log: iiopdefs.h,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:23  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.7  2001/06/08 08:40:09  bilhanan
Full licence clause added into doors 0.1


*/
//
//  =========================================================================
//  File:     iiopdefs.h
//  Date:     24.03.2000
//  Subject:  IIOP ver 1.2
//  Desc:     
//
//  Author:   Janne Makihonka, TTKK/DMI
//  =========================================================================


#ifndef IIOPDEFS_H
#define IIOPDEFS_H

#include <iostream.h>

#include <doors/frame.h>

#define USE_SYMBOLS 0
#define USE_SYMBOLS_IIOP 1

#define DEFAULT_PORT 4000


#define ERROR(x) cout << "ERROR: " << x << endl; cout.flush();
#define PEERERROR(x) cout << "PEER ERROR: " << x << endl; cout.flush();
#define ERROREXIT(x) cout << "ERROR: " << x << endl; cout.flush(); exit(1);
#define DEBUGINFO(x) cout << "INFO: " << x << endl; cout.flush(); 
 


inline void FRAME_PRINT(Frame &frm) 
{ 
  Sint32 len = frm.length(); 
  DEBUGINFO("FRAME_PRINT " << len); 
  int j = 0; 
  for(j = 1; j <= len; ++j ){ 
    cout << (int)frm.read(j-1) << " "; 
    if(j%20 == 0) cout << "\n"; 
  } 
  cout.flush(); 
} 




#endif
