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

$Log: rtcp.h,v $
Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.2  2001/06/08 16:45:54  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:40:22  bilhanan
Full licence clause added into doors 0.1


*/
//===========================================================================
//File:     rtcp.h
//Date:     11.5.1999
//Subject:  RTCP-specific common things
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#ifndef RTCP_H
#define RTCP_H

#include <string>

/** An object to hold reports from RTP senders
  */
class Report { 
public:
  /** Constructor
    */
  Report();

  /** Constructor
      @param report An old report to copy to this new object
  */
  Report(Report& report);
  
  /** Create a copy of this object
      @return A copy of this object
  */
  Report& copy();

  // Items are public for fun
  unsigned long ssrc;              // SSRC id number
  unsigned char fraction_lost;     // Fraction lost
  unsigned long packets_lost;      // Packets lost
  unsigned long highest_sequence;  // Highest sequence received
  unsigned long jitter;            // Jitter
  unsigned long lsr;               // LSR (= last SR timestamp)
  unsigned long dlsr;              // DLSR (= delay since last SR timestamp)
};

/** An object to hold source description items of RTP senders and
    receivers
*/
class SrcDesc {
public:
  /** Constructor
    */
  SrcDesc();

  /** Constructor
      @param srcdesc An old source description to copy to this new object
  */
  SrcDesc(SrcDesc& srcdesc);

  /** Destructor to delete all the subsequent objects that were set 
      using setNext()
  */
  ~SrcDesc();

  /** Create a copy of this object
      @return A copy of this object
  */
  SrcDesc& copy();

  /** Set pointer to the next object of this type in list. This allows us
      to create lists of source description objects
  */
  void setNext(SrcDesc* n) { next = n; }

  /** Return pointer to the next object of this type.
      @return Pointer to the next object. If not set, returns NULL.
  */
  SrcDesc* getNext() { return next; }

  // Items are public and of string type so a user can access
  // them directly and use normal C++ string type operations
  std::string cname;        // Canonical end-point identifier
  std::string name;         // Name
  std::string email;        // e-mail address
  std::string phone;        // Phone number
  std::string loc;          // Location
  std::string tool;         // RTP program name etc
  std::string note;         // Additional note
  std::string priv_prefix;  // Private description prefix
  std::string priv_value;   // Private description value
  unsigned long src;   // SSRC/CSRC id number

private:
  class SrcDesc* next; // Pointer to the next object
};

#endif


