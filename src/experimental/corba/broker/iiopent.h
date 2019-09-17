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

$Log: iiopent.h,v $
Revision 1.2  2002/07/23 14:32:47  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/05/13 14:38:35  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:37  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.8  2001/06/08 08:40:09  bilhanan
Full licence clause added into doors 0.1


*/
//
//  =========================================================================
//  File:     iiopent.h
//  Date:     24.03.2000
//  Subject:  IIOP ver 1.2
//  Desc:     
//
//  Author:   Janne Makihonka, TTKK/DMI
//  =========================================================================



#ifndef IIOPENT_H
#define IIOPENT_H


#define THIS "[" << getName() << "] "


#include <doors/sockmsg.h>
#include <doors/sockeos.h>

#include <doors/iiopsap.h>
#include <doors/iiopsm1_0.h>
#include <doors/iiopsm1_1.h>
#include <doors/iiopsm1_2.h>

class IiopConnection;


// At this point the number of socket connections is limited to 256. Notice
// also that no range checking is done.


class IiopEntity : public EventTask {
 public:

  IiopEntity (const char *n, Address *listenaddr);
  virtual ~IiopEntity (void);
  
  bool execute (EventType type, Message *msg);

  IiopConnection *getConnection (Uint32 i);
  Uint8 ByteOrder(void); 

  IiopSAP :: Provider up;
  DevIf :: User entity;

  IiopSm_1_0 sm_1_0;
  IiopSm_1_1 sm_1_1;
  IiopSm_1_2 sm_1_2;

 private:
  
  SocketEntityOS edevice;

  IiopConnection *connections [256];
};


#endif  // IIOPENT_H

