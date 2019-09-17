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

$Log: slpcommon.C,v $
Revision 1.15  2004/07/15 09:34:02  hartmanj
Added another getMCAddr, now with scope parameter

Revision 1.14  2004/03/25 12:38:47  hartmanj
Compile fixes

Revision 1.13  2004/03/25 09:16:19  hartmanj
Added solicitDirectoryAgent()

Revision 1.12  2004/03/22 11:07:11  hartmanj
Moved createAttributeList() to SLPCommon

Revision 1.11  2004/03/22 07:05:30  hartmanj
Fixed getMCAddr() which got broken earlier

Revision 1.10  2004/03/19 11:20:44  hartmanj
Changes in API: joinMCGroup() and getMCAddr() changed.

Revision 1.9  2004/03/09 09:16:51  bilhanan
following through from earlier code changes.

Revision 1.8  2004/03/09 08:43:27  bilhanan
changes in constructors, destructor, removed DAActiveDiscoveryInterval_,
removed readConfig, removed uncessary #includes, code cleanup.

Revision 1.7  2004/02/12 10:25:56  bilhanan
XIDCounter initialised to 0 instead of 1.

Revision 1.6  2004/02/10 18:08:54  bilhanan
rewrote joinMCGroup and removed joinMCGroups

Revision 1.5  2004/02/10 16:29:00  hartmanj
Changed XID handling abit

Revision 1.4  2003/11/27 12:32:56  bilhanan
bug fixing

Revision 1.3  2003/08/24 22:13:11  bilhanan
merged in code for mcast group address calculation.

Revision 1.2  2003/06/04 11:09:55  kalliosa
Licence clause added

  
*/

#include <string>
#include <doors/udp6.h>
#include <doors/slppdu.h>
#include <doors/slpcommon.h>
#include <cstdlib>
#include <doors/debug.h>

Uint16 SLPCommon::XIDCounter_ = 0;
bool SLPCommon::FirstXID_ = true;

SLPCommon :: SLPCommon (Udp6SAP :: User &udp6sap) : udp6down(udp6sap) {

}

SLPCommon :: ~SLPCommon ( )
{
}

Uint16 SLPCommon ::  getNewXID()
{
  if( FirstXID_ == true ) {
    FirstXID_ = false;
    srand(static_cast<unsigned>(time(0)));
    XIDCounter_ = (rand() % 65534) + 1;
    return XIDCounter_;
  }

   ++XIDCounter_;
   if( XIDCounter_ > 65534 )
     XIDCounter_ = 1;
   return XIDCounter_;
}
int SLPCommon :: getNewHandle()
{
   ++HandleCounter_;
   if( HandleCounter_ > 400000000 )
     HandleCounter_ = 1;
   return HandleCounter_;
}

bool SLPCommon :: joinMCGroup (std::string stype)
{
    bool joined;
    int scope_to_join = ip6scope_;

    if (scope_to_join > 2) {
    
        // global mcast scope

        _DOORS_DEBUG(0,"Joining multicast group for scope 5...");

        Udp6SAP :: JoinMcast *g_msg = (Udp6SAP :: JoinMcast*)udp6down.create(Udp6SAP::joinmcast);
        g_msg->addr.set(slpPort_, getMCAddr(stype,scope_to_join));
        udp6down.putMessage (g_msg);
	// lower the scope
	scope_to_join = 2;
    }  //join to lower scopes too.


    if (scope_to_join > 1) {
    
        // link local mcast scope
    
        Udp6SAP :: JoinMcast *l_msg = (Udp6SAP :: JoinMcast*)udp6down.create(Udp6SAP::joinmcast);
        _DOORS_DEBUG(0,"Joining multicast group for scope 2...");
    
        l_msg->addr.set(slpPort_, getMCAddr(stype,scope_to_join));
        udp6down.putMessage (l_msg);
	scope_to_join = 1;
    }   //join to lower scopes too.

    if (scope_to_join > 0) {
    
        // node local mcast scope
    
        Udp6SAP :: JoinMcast *n_msg = (Udp6SAP :: JoinMcast*)udp6down.create(Udp6SAP::joinmcast);
        _DOORS_DEBUG(0,"Joining multicast group for scope 1...");
    
        n_msg->addr.set(slpPort_, getMCAddr(stype,scope_to_join));
        udp6down.putMessage (n_msg);
        joined = true;
    }

    return joined;
}

std::string SLPCommon :: getMCAddr(std::string sType, int scope) {

    if (sType == "SVRLOC" 
	||  sType == "SVRLOC-DA"
	|| sType == "service:directory-agent") {

	std::string addrStr;

	switch( scope ) {
	case 5:
	    addrStr = "ff05:0:0:0:0:0:0:";
	    break;
	case 2:
	    addrStr = "ff02:0:0:0:0:0:0:";
	    break;
	case 1:
	default:
	    addrStr = "ff01:0:0:0:0:0:0:";
	    break;
	}
	
	if (sType == "SVRLOC") {
	    addrStr += "116";
	} else {
	    addrStr += "123";
	}

	return addrStr;
    }
  
    const char *pc = sType.c_str();
    std::ostringstream addrStream;
    Uint32 h = 0;
    Uint32 len = sType.length();
  
    for (Uint32 i=0; i<len; ++i) {
    
	h *= 33;
	h += ((Uint8) sType[i]);
    }    
  
    h = (0x3FF & h); /* round to a range of 0-1023 */
    addrStream << "ff0";
    addrStream << scope;
    addrStream << "::1:";
    addrStream.setf(std::ios::hex, std::ios::basefield);
    addrStream << h;

    _DOORS_DEBUG(0, "returning address: " << addrStream.str() );
    return addrStream.str();

}

std::string  SLPCommon :: getMCAddr (std::string sType)
{
  return getMCAddr( sType, ip6scope_ );
}

void SLPCommon :: createAttributeList( std::vector<Attribute> &attrVec,
                                       const std::vector<std::string> &strVec )
{
    // TODO: escaping special chars
    for(unsigned int i=0;i<strVec.size();i++) {
        unsigned int eqidx;
        Attribute newAttr;
        if( (eqidx = strVec[i].find('=')) != std::string::npos ) {
            newAttr.AttributeName = strVec[i].substr(0,eqidx);
            newAttr.AttributeValue = strVec[i].substr(eqidx+1,strVec[i].size()-eqidx);
        } else {
            newAttr.AttributeName = strVec[i];
            newAttr.AttributeValue = "";
        }

        attrVec.push_back(newAttr);
    }
}

void SLPCommon :: solicitDirectoryAgent( std::string scope )
{
    SLPPeer :: SRVRQST *darequest = new SLPPeer :: SRVRQST;

    darequest->servicetype = "service:directory-agent";
    darequest->scopeList.push_back(scope);
    darequest->XID = getNewXID();
    darequest->dest.set(slpPort_,getMCAddr( darequest->servicetype ));
    Message *UdpDtreq = darequest->encode();
    udp6down.putMessage(UdpDtreq);
    delete darequest;
}


