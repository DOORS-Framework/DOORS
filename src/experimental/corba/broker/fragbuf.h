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

$Log: fragbuf.h,v $
Revision 1.2  2002/07/23 14:32:47  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/05/13 14:38:35  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:30  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.8  2001/06/08 08:40:16  bilhanan
Full licence clause added into doors 0.1


*/

//===========================================================================
//File:     fragbuf.h
//Date:     11.01.2000
//Subject:  IIOP ver 1.2
//Desc:   
//
//Author:   Janne Makihonka, TTKK/DMI
//===========================================================================

#ifndef FRAGBUF_1_2_H
#define FRAGBUF_1_2_H

#include <doors/bastypes.h>
#include <doors/giop1_2.h>

class FragmentBuffer_1_2 {

private:
  
  class FragBufCell{
  public:
    FragBufCell* prev;
    Uint32 msg_id;
    bool is_msg_id;
    Frame *msg;
    FragBufCell* next;
  
    FragBufCell()
      :prev(NULL), msg_id(0), is_msg_id(false),msg(NULL), next(NULL){
    };

    ~FragBufCell(){
      delete msg;
    }
  };

  FragBufCell* NewCell();
 void DeleteCell(FragmentBuffer_1_2::FragBufCell* cell); 
  FragBufCell* head;
  FragBufCell* tail;
  FragBufCell* curr;
  FragBufCell* needs_request_id;

  Uint32 readfr(Frame &b, Uint32 index = 0, Byte swap = 0){
    Uint32 retu = 0;
    retu = (Uint32) b.read (index) << 24
    | (Uint32) b.read (index+1) << 16
    | (Uint32) b.read (index+2) << 8
    | (Uint32) b.read (index+3);
    if(swap) SWAP4(retu);
    return retu;
  };

public:
  FragmentBuffer_1_2();
  ~FragmentBuffer_1_2();

  int NewBuffer(Frame* &frm);
  Frame* AddFragment(Frame* &frm, Uint16 &params);
  FragBufCell* Find(Uint32 msg_id);
  bool Cancel(Uint32 msg_id);
  void DeleteAll();
  
};

#endif

