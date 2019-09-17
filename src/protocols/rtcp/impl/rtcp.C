//Editor-Info: -*- C++ -*-

//  Copyright 2000 
//  	Telecoms Lab, Tampere University of Technology.  All rights reserved.

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

$Log: rtcp.C,v $
Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.2  2001/06/08 16:47:05  bilhanan
Full Licence clause into DOORS 0.2


*/
//
//===========================================================================
//File:     rtcp.C
//Date:     11.5.1999
//Subject:  RTCP-specific common things
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#include <string>
#include <doors/rtcp.h>

Report::Report()
  : ssrc(0), fraction_lost(0), packets_lost(0), highest_sequence(0),
    jitter(0), lsr(0), dlsr(0)
{ }

Report::Report(Report& report)
{
  ssrc = report.ssrc;
  fraction_lost = report.fraction_lost;
  packets_lost = report.packets_lost;
  highest_sequence = report.highest_sequence;
  jitter = report.jitter;
  lsr = report.lsr;
  dlsr = report.dlsr;
}

Report&
Report::copy()
{
  Report* report = new Report(*this);
  return *report;
}

SrcDesc::SrcDesc()
  : src(0), next(NULL)
{ }

SrcDesc::~SrcDesc()
{
  if (next != NULL)
    delete next;
}

SrcDesc::SrcDesc(SrcDesc& srcdesc)
  : next(NULL)
{
  src = srcdesc.src;
  cname = srcdesc.cname;
  name = srcdesc.name;
  email = srcdesc.email;
  phone = srcdesc.phone;
  loc = srcdesc.loc;
  tool = srcdesc.tool;
  note = srcdesc.note;
  priv_prefix = srcdesc.priv_prefix;
  priv_value = srcdesc.priv_value; 
}

SrcDesc&
SrcDesc::copy()
{
  SrcDesc* srcdesc = new SrcDesc(*this);
  return *srcdesc;
}

