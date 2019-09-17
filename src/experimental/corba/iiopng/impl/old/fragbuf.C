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

$Log: fragbuf.C,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:20  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.7  2001/06/08 08:40:15  bilhanan
Full licence clause added into doors 0.1


*/

//===========================================================================
//File:     fragbuf.C
//Date:     11.01.2000
//Subject:  IIOP ver 1.2
//Desc:   
//
//Author:   Janne Makihonka, TTKK/DMI
//===========================================================================

#include <doors/fragbuf.h>


FragmentBuffer_1_2 :: FragmentBuffer_1_2()
  :head(NULL), tail(NULL), needs_request_id(0)
{
}

FragmentBuffer_1_2 :: ~FragmentBuffer_1_2()
{
  while(head){
    curr = head;
    head = head->next;
    delete curr;
  }
}


FragmentBuffer_1_2::FragBufCell* FragmentBuffer_1_2 :: NewCell()
{
  curr = new FragBufCell;
  if(!head){
    head = tail = curr;
  } else {
    tail->next = curr;
    curr->prev = tail;
    tail = curr;
  }
  return curr;
}

void
FragmentBuffer_1_2 :: DeleteCell(FragmentBuffer_1_2::FragBufCell* cell)
{
  if(cell == head){
    head = head->next;
    head->prev = NULL;
  } 
  else if(cell == tail){
    tail = tail->prev;
    tail->next = NULL;
  }
  else {
    curr = cell->next;
    curr->prev = cell->prev;
    (curr->prev)->next = curr;
  }
  delete cell;
}

int FragmentBuffer_1_2 :: NewBuffer(Frame* &frm)
{ 
  Uint32 frlen = frm->length();

  if(frlen < 12) return -1;

  Byte flags = frm->read(6);
  Byte msgtype = frm->read(7);
  Byte swap = flags & 0x1;

  switch(msgtype)
    {
    case Giop_1_2::request :
    case Giop_1_2::reply :
    case Giop_1_2::locateRequest :
    case Giop_1_2::locateReply :
      // If no request id included
      if(frlen < 16){
	needs_request_id = NewCell();
	needs_request_id->msg = frm;
	frm = NULL;
	return 1; 
      } else {
	// Request id included
	curr = NewCell();
	curr->msg_id = readfr(*frm, 12, swap);
	curr->is_msg_id = OdTrue;
	curr->msg = frm;
	frm = NULL;
      }
      break;
    default:
      //Wrong Message type
      return -2;
      break;
    }
  return 0;
}
  

Frame* FragmentBuffer_1_2 :: AddFragment(Frame* &frm, Uint16 &params)
{
  Uint32 frlen = frm->length();
  Byte flags = frm->read(6);
  Byte msgtype = frm->read(7);
  Byte swap = flags & 0x1;  
  Byte more = flags & 0x2;

  Frame *retu = NULL;
  params = 1; // more fragments needed

  Uint32 msgid = readfr(*frm, 12, swap);
  if(needs_request_id){
    Uint32 msglen = needs_request_id->msg->length();
    if((msglen + (frlen - 16)) < 16) {
      frm->delFirst(16);
      needs_request_id->msg->combine(*frm);
      params = 0x2; // request_id_needed
      curr = NULL;
    } else {
      frm->delFirst(16);
      needs_request_id->msg->combine(*frm);
      curr = needs_request_id;
      needs_request_id = NULL;
    }
  } else if( (curr = Find(msgid)) != NULL){
    frm->delFirst(16);
    curr->msg->combine(*frm);
  }
  if(!more){
    params = 0;
    retu = curr->msg;
    DeleteCell(curr);
  }

  return retu;
}


FragmentBuffer_1_2::FragBufCell* FragmentBuffer_1_2 :: Find(Uint32 msg_id)
{
  curr = head;
  while((curr->msg_id != msg_id) && curr->next){
    curr = curr->next;
  }
  if(curr->msg_id == msg_id){
    return curr;
  }
  return NULL;
}


OdBoolean FragmentBuffer_1_2 :: Cancel(Uint32 msg_id)
{
  OdBoolean retu = OdTrue;
  curr = Find(msg_id);
  if(curr){
    (curr->next)->prev = curr->prev;
    (curr->prev)->next = curr->next;
    delete curr;
    retu = OdFalse;
  }
  return retu;
}

void FragmentBuffer_1_2 :: DeleteAll()
{
  curr = head;
  while(curr->next){
    curr = curr->next;
    delete curr->prev;
  }
  return;
}

