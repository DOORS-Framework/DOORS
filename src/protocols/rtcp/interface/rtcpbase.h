
/*
  Copyright 2002 
      Telecoms Lab, Tampere University of Technology.  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY TAMPERE UNIVERSITY OF TECHNOLOGY AND 
  CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY 
  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


  rtcpbase.h has the base classes used in RTCP protocols message
  definitions in rtcppdu.pdu and rtcpsap.sap

  $Log: rtcpbase.h,v $
  Revision 1.6  2003/05/07 13:26:32  bilhanan
  hsi.h included.

  Revision 1.5  2003/05/07 13:18:03  bilhanan
  SAP base classes now need to be derived from Message. Constructors added inline for symbol handling.

  Revision 1.4  2002/07/23 16:34:25  hartmanj

  Development

  Revision 1.3  2002/07/23 11:11:26  hartmanj
  xcg modifications

  Revision 1.2  2002/07/23 09:25:44  hartmanj
  Licence clause added


*/

#include <doors/inetaddr.h>
#include <doors/frame.h>
#include <doors/message.h>
#include <doors/hsi.h>

class RtcpUserBase : public Message {
	public:

        RtcpUserBase (void);
        RtcpUserBase (const RtcpUserBase &other);

		InetAddr destIP;
		Frame extension;
};


class RtcpProvBase : public RtcpUserBase {
	public:

        RtcpProvBase (void);
        RtcpProvBase (const RtcpProvBase &other);

		InetAddr srcIP;
};

class RtcpPduBase {
	public:
		InetAddr destIP;
		InetAddr srcIP;
		Uint16 length;
		Uint8  padding;
		Frame  extension;
		Frame  nextpdu;
};

inline RtcpUserBase :: RtcpUserBase (void) {
  STATIC_SYMBOL (destIP, this);
  STATIC_SYMBOL (extension, this);
}

inline RtcpUserBase :: RtcpUserBase (const RtcpUserBase &other) 
: destIP (other.destIP), extension (other.extension){
  STATIC_SYMBOL (destIP, this);
  STATIC_SYMBOL (extension, this);
}


inline RtcpProvBase :: RtcpProvBase (void) {
  STATIC_SYMBOL (srcIP, this);
}

inline RtcpProvBase :: RtcpProvBase (const RtcpProvBase &other) 
: srcIP (other.srcIP){
  STATIC_SYMBOL (srcIP, this);
}
