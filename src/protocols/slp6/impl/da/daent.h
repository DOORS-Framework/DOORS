//  Copyright 2003
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.

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

$Log: daent.h,v $
Revision 1.1  2004/04/05 06:53:45  hartmanj
Renamed damain* to daent*

Revision 1.13  2004/03/22 11:08:10  hartmanj
Moved createAttributeList() to SLPCommon

Revision 1.12  2004/03/19 11:22:44  hartmanj
Adapting new SLPCommon API. Service type request and attribute request implemented (but not yet tested)

Revision 1.11  2004/03/16 08:52:55  hartmanj
Loads of cleaning up and bugfixing. Added lifetime expiration detection. Indentation cleaned. Mcast joining properly implemented etc etc

Revision 1.10  2004/03/09 11:16:01  hartmanj
Changed services data structure. Added std::couts to report actual DA operations when debug-messages are not enabled

Revision 1.9  2004/03/01 18:25:42  bilhanan
general tidying and removal of readConfig() member function.

Revision 1.8  2004/03/01 09:04:44  hartmanj
Fixed XID handling when replying to SRVRQST

Revision 1.7  2004/02/26 10:58:46  hartmanj
Fixing and rewriting

Revision 1.6  2004/02/12 11:44:11  bilhanan
DA Stateless Boot Timestap fix

Revision 1.5  2004/02/10 18:11:48  bilhanan
joining multicast groups changed
removed dependency to UDP6Task in constructor

Revision 1.4  2003/08/24 22:25:07  bilhanan
multiple inheritance reduced.

Revision 1.3  2003/08/12 00:05:01  bilhanan
const char* in constructor -> std::string

Revision 1.2  2003/06/04 11:09:57  kalliosa
Licence clause added

  
*/

#ifndef DAENT_H
#define DAENT_H

#include <string>
#include <vector>
#include <doors/ptb.h>
#include <doors/udp6task.h>
#include <doors/slppdu.h>
#include <doors/daentsm.h>
#include <doors/serviceurl.h>
#include <doors/slpcommon.h>

class DAEntSM;

class DAEnt : public PTask, public SLPCommon
{
public :
    DAEnt();
    DAEnt(  std::string n, 
             Scheduler *s, 
             DAEntSM *sm,
             InetAddr6 &addr  );
   
    ~DAEnt();

    bool ready_srvRqst (Message *msg );
    bool ready_srvReg(Message *msg );
    bool ready_srvDeReg(Message *msg );
    bool ready_attrRqst(Message *msg );
    bool ready_srvTypeRqst(Message *msg );
    bool ready_srvAck(Message *msg );
   
    bool ready_DATimer( Message *msg );
   
    bool ready_default(Message *msg );

    static const InetAddr6 SERVLOC;
    static const InetAddr6 SERVLOCDA;

    SLPPeer peer_;
    Udp6SAP :: User down_;
   
private:

    void sendReply ( InetAddr6 dest, URLVector &URLs, Uint16 replyXID );
    void sendDAAdv( InetAddr6 & dest );

    struct ServiceInfo {
        ServiceURL url;
        Uint16 XID;
        std::string languageTag;
        StringVector scopes;
        std::vector<Attribute> attributes;
        Uint32 lastRegTime;
    };

    std::vector<ServiceInfo> services_;

    InetAddr6 ownAddr_;
    InetAddr6 SRVLOCDAGroup_;

    int heartBeat_;
    Timer advTimer_;
    Uint32 bootstamp;
    Uint32 daTime_; // Own "time" for cleaning stale services
    bool firstAdv_;
    Uint32 loggedadvsend_;
  
};



#endif //  DAENT_H
