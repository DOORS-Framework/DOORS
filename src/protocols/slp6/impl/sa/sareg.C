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

$Log: sareg.C,v $
Revision 1.29  2004/04/14 11:19:16  hartmanj
Development

Revision 1.28  2004/04/14 09:16:48  hartmanj
Bug fixing

Revision 1.27  2004/04/05 06:55:59  hartmanj
Renamed samain* to saent*

Revision 1.26  2004/04/01 10:48:38  hartmanj
Moved sending indication of registration to SAMain. SAReg doesn't have up port.

Revision 1.25  2004/04/01 09:29:42  hartmanj
Send service registration indication to user in ready_srvAck()

Revision 1.24  2004/03/22 11:10:56  hartmanj
Moved attribute request handling from SAMain to SAReg.

Revision 1.23  2004/03/19 13:25:58  hartmanj
Fixed a memory-oops that slipped in

Revision 1.22  2004/03/19 11:23:47  hartmanj
Adapting new SLPCommon API.

Revision 1.21  2004/03/15 15:22:01  hartmanj
SA joins two mcast groups if necessary

Revision 1.20  2004/03/09 09:51:20  bilhanan
changes in getMCAddr

Revision 1.19  2004/02/25 13:09:10  hartmanj
Fixed to reflect API change in ServiceURL.

Revision 1.18  2004/02/20 12:39:50  hartmanj
Timer fixes, removed unnecessary comments

Revision 1.17  2004/02/18 15:21:47  hartmanj
Added a check to see if we already registered to the DA that just sent DAADV

Revision 1.16  2004/02/18 14:54:17  hartmanj
Yet Another XID handling fix

Revision 1.15  2004/02/18 09:27:54  bilhanan
DAFinder::getDAs function parameter changes to prevent crashes with sun
c++ compiler.

Revision 1.14  2004/02/12 08:04:51  hartmanj
Changed XID to be handled in SLPCommon. Removed lastregxid_ and added bool hasXID(Uint16 XID)

Revision 1.13  2004/02/10 18:10:17  bilhanan
joining multicast groups changed.

Revision 1.12  2004/02/10 13:30:37  bilhanan
bugfixes

Revision 1.11  2004/02/04 15:03:05  hartmanj
Complete rewrite of DA-SA interaction code.

Revision 1.10  2004/02/02 09:30:19  hartmanj
Couple of bugfixes and more _DOORS_DEBUGs in

Revision 1.9  2003/11/27 12:32:57  bilhanan
bug fixing

Revision 1.8  2003/10/06 12:48:28  bilhanan
Hack needed for Sun WorkShop 6 update 2 C++ 5.3 Patch 111685-07

Revision 1.7  2003/10/03 14:11:35  bilhanan
improper initialisation caused runtime crash

Revision 1.6  2003/08/24 22:23:53  bilhanan
multiple -> single inheritance, uses samain's dafinder now.

Revision 1.5  2003/08/12 00:07:46  bilhanan
const char* in constructor -> std::string

Revision 1.4  2003/06/04 11:09:59  kalliosa
Licence clause added

  
*/

#include <string>
#include <doors/sareg.h>
#include <doors/slptypes.h>
#include <netinet/in.h>
#include <sys/socket.h>


#include <doors/debug.h>

const int SAReg::MulticastTimeouts[5] = 
{
     3, 3, 3, 3, 3
}; 

// costructor 
SAReg :: SAReg( std::string n, SASAP::registerService *reg, SAEnt *saent,  SARegSM *sm )
     :  PTask (n, saent->getScheduler (), sm), 
        saent_( saent ),
        entity_( this ),
        down_( saent->down_ ),
        peer_( saent->peer_ ),
        regTimer_( this, OTime( 5 ) ),
        registration_(0),
        myScope_("DEFAULT"),
        defaultRefreshFreq(61),
        newReplies_( false ),
        resendTime_(CONFIG_RETRY)
//        myMCaddr_ (saent_->getMCAddr (reg->URL.getServiceType()))
{
    if( reg->attrs.length() ) {
        char *a = new char[reg->attrs.length()];
        unsigned int i = 0;
        while( reg->attrs.length() ) {
            a[i] = reg->attrs.getFirst();
            if(a[i] == ',') {
                a[i] = 0;
                if( i ) {
                    attrs_.push_back(std::string(a));
                }
                i = 0;
                continue;
            }
            i++;
        }
        if( i ) {
            a[i]=0;
            attrs_.push_back(std::string(a));
        }
        delete [] a;
    }

    saent_->createAttributeList( attributes_, attrs_ );

    Byte *urlBytes = (Byte*)reg->URL;
    std::string urlStr((char*)urlBytes);
    delete urlBytes;
    url_ = ServiceURL(urlStr, reg->lifetime);

    if( ! url_.isOk() ) {
        good_ = false;
        std::cout << "Invalid service registration! (" << url_.getServiceType() << ")" << std::endl;
    } else {
        good_ = true;
        std::cout << "Service " << url_.getServiceType() << " registered by user" << std::endl;
    }
    
     createRegistration();
     // join multicast group for service
     if(saent_->joinMCGroup(url_.getServiceType())) {
       std::cout << "Joining multicast group for service " << registration_->serviceType << std::endl;
     } else {
       std::cout << "Couldn't join multicast group for service " << registration_->serviceType << std::endl;
     }

     // join abstract type group also
     if( url_.getServiceType() != url_.getAbstractType() ) {
       saent_->joinMCGroup(url_.getAbstractType());
     }
     
     saent_->dafinder.getDAs( myScope_, myDAs_ );

     myDARegInfo EmptyRegInfo;
     if( reg->lifetime > 0 ) {
         defaultRefreshFreq = reg->lifetime;
     }
     EmptyRegInfo.refreshFreq = defaultRefreshFreq;
     EmptyRegInfo.lifetime = -1;
     EmptyRegInfo.wfAck = false;
     EmptyRegInfo.resendCount = 0;
     EmptyRegInfo.resendTimer.init(this, OTime(resendTime_) );
     myDAInfos_.resize(myDAs_.size(), EmptyRegInfo);

     updateDARegistrations( );
     reg->unlock();
     regTimer_.start();
}

bool SAReg :: isGood() const
{
    return good_;
}
/*
 * The works is mostly done here when timers fire.
 * 1) Check if registrations expire and reregister.
 * 2) Check if wfAck timeouts for registrations and resend.
 *     - If resendcount reaches max, remove DA from list.
 */

bool SAReg :: ready_timer( Message *msg )
{
     if( msg->getSender() == &regTimer_ ) {
          // Time to reregister some of the services

          // Subtract 5 secs from lifetimes
          for(unsigned int i = 0 ; i < myDAInfos_.size() ; i++ ) {
               if(myDAInfos_[i].lifetime >= 0)
                    myDAInfos_[i].lifetime -= 5;
               _DOORS_DEBUG(0, "  myDAInfos_["<<i <<"].lifetime == " << myDAInfos_[i].lifetime);
          }
          
          updateDARegistrations( );

          // Check for reregistration again in 'bout 5 secs
          regTimer_.start();
          
     } else {

          unsigned int idx = 0;

          // Find which timer fired
          while(idx < myDAInfos_.size() && msg->getSender() != &myDAInfos_[idx].resendTimer) {
               idx++;
          }

          if( idx < myDAInfos_.size() ) {
               _DOORS_DEBUG(0, "  SAREG [resend timer fired for " << myDAs_[idx]->addr.addrToString());

               if( myDAInfos_[idx].resendCount > CONFIG_RETRY_MAX ) {
                    _DOORS_DEBUG(0, "       DA not responding after CONFIG_RETRY_MAX, cancelling resendCount == " << myDAInfos_[idx].resendCount);
                    // DA doesn't respond, remove from list
                    myDAInfos_.erase(myDAInfos_.begin()+idx);
                    myDAs_.erase(myDAs_.begin()+idx);
               } else {
                    _DOORS_DEBUG(0, "       DA not responding, resending SRVREG. resendCount == "
                                 << myDAInfos_[idx].resendCount);
                    // No ack from DA, resend registration
                    sendRegToDA( myDAs_[idx], myDAInfos_[idx].XID );
                    myDAInfos_[idx].resendCount +=myDAInfos_[idx].resendTimer.getInitial().getSeconds();
                    myDAInfos_[idx].resendTimer.start();
               }
          } else {
               _DOORS_DEBUG(0, "Unknown timer fired in SAREG!");
          }
          
     }
     
     return false;
}

// destructor
SAReg :: ~SAReg (void) 
{
     if( registration_ != 0 )
     {      
          delete registration_;
          registration_ = 0;
     }

//   leaveMCastGroup(
     DELETE_SYMBOL (this);   
}

bool SAReg :: hasXID( Uint16 XID )
{
     for( unsigned int i = 0; i < myDAInfos_.size(); i++ ) {
          if( myDAInfos_[i].XID == XID )
               return true;
     }
     return false;
}

void SAReg :: createRegistration()
{
     SLPPeer :: SRVREG *reg 
          = (SLPPeer :: SRVREG* ) peer_.create ( SLPPeer :: srvreg );

     reg->XID = saent_->getNewXID();
     reg->url = url_;
     std::string temp = url_.getServiceType();
     reg->serviceType = temp;
     StringVector slist;
     slist.push_back( "DEFAULT" );
     reg->scopeList = slist;
     // copying empty vecror to attributelist
     //slist.clear();
     reg->attributeList = attrs_;

     registration_ = reg;
}

bool SAReg :: isOfType( const std::string &type ) const
{
     if( url_.isOfServiceType(type) )
          return true;

     return false;
}

InetAddr6 SAReg :: getMCaddr(void)
{
     return myMCaddr_;
}

ServiceURL SAReg :: getURL()
{
     return url_;
}

// message handlers

bool SAReg :: ready_default (Message *msg)
{
     return false;
}

void SAReg :: updateDARegistrations( void )
{
     for( unsigned int i = 0 ; i < myDAs_.size() ; i++ )
     {
          if( myDAInfos_[i].lifetime < 0 && myDAInfos_[i].wfAck == false ) {
               _DOORS_DEBUG(0, "  updating reg for DA " << myDAs_[i]->addr.addrToString());
               std::cout << "Refreshing DA registration for DA " << myDAs_[i]->addr.getHostAddr()
                         << url_.getServiceType() << std::endl;
               myDAInfos_[i].XID = saent_->getNewXID();
               sendRegToDA( myDAs_[i], myDAInfos_[i].XID );
               myDAInfos_[i].wfAck = true;
               myDAInfos_[i].resendCount = 0;
               myDAInfos_[i].resendTimer.start();
          }
     }
}

void SAReg :: sendRegToDA( const struct DAFinder::DAInfo *DA, Uint16 sendXID )
{
     //TODO: should personalise scopes list according to respective DA
#ifdef __SUNPRO_CC 

     // ugly SUN C++ kludge needed to prevent runtime crash.
     // Copy each field explicitly to prevent nullvector copies

     SLPPeer :: SRVREG *copy = (SLPPeer :: SRVREG* ) peer_.create ( SLPPeer :: srvreg );

     // SLP Header
     copy->decoded_ = registration_->decoded_;
     copy->version = registration_->version;
     copy->functionID = registration_->functionID;
     copy->length = registration_->length;
     copy->overflow = registration_->overflow;
     copy->requestMcast = registration_->requestMcast;
     copy->fresh = registration_->fresh;
     copy->XID = sendXID;
     copy->slpHandle = registration_->slpHandle;
     copy->dest.set (DA->addr);
     copy->source.set (registration_->source);

     // SRV Reg 
     copy->url = registration_->url;
     copy->serviceType = registration_->serviceType;
     copy->errorCode = registration_->errorCode;

     if (!registration_->scopeList.empty())
          copy->scopeList = registration_->scopeList;

     if (!registration_->attributeList.empty())
          copy->attributeList = registration_->attributeList;

     if (!registration_->AttrAuthBlocks.empty())
          copy->AttrAuthBlocks = registration_->AttrAuthBlocks;

#else

     Message *copy = registration_->clone();   
     ((SLPPeer::SRVREG*)copy)->dest.set(DA->addr);
     ((SLPPeer::SRVREG*)copy)->XID = sendXID;

#endif

     peer_.putMessage( copy );
}

bool SAReg :: ready_daAdvert(Message *msg)
{
     _DOORS_DEBUG(0,"SAReg :: ready_daadvert()");
     
     SLPPeer :: DAADVERT *Advert = (SLPPeer :: DAADVERT*) msg;

     std::cout << "DA Advert received from " << Advert->url.getHost() << std::endl;

     InetAddr6 adv_addr = Advert->url.getHostAddr();
     for(unsigned int i=0; i < myDAs_.size(); i++) {
          if( adv_addr == myDAs_[i]->addr ) {
               _DOORS_DEBUG(0, "  already registered!");
               return false;
          }
     }
     
     DAFinder::DAInfo* newDA = saent_->dafinder.getDAInfo(Advert->source);

     if( newDA != NULL ) {
          myDAs_.push_back(newDA);

          unsigned int newDAIdx = myDAs_.size() - 1;
 
          myDARegInfo newInfo;
          myDAInfos_.push_back(newInfo);
          myDAInfos_[newDAIdx].wfAck = true;
          myDAInfos_[newDAIdx].resendCount = 0;
          myDAInfos_[newDAIdx].resendTimer.init(this, OTime(resendTime_) );
          myDAInfos_[newDAIdx].resendTimer.start();
          myDAInfos_[newDAIdx] .refreshFreq = defaultRefreshFreq;
          myDAInfos_[newDAIdx].XID = saent_->getNewXID();
          myDAInfos_[newDAIdx].lifetime = -1;
          sendRegToDA( myDAs_[newDAIdx], myDAInfos_[newDAIdx].XID );
     } else {
          _DOORS_DEBUG(0, "No DAInfo found from DAFinder for DA " << Advert->source.addrToString());
     }
     return false;
}

bool SAReg :: ready_srvAck( Message *msg )
{
     _DOORS_DEBUG(0, "SAReg :: ready_srvAck( )" );

     SLPPeer :: SRVACK * ack = (SLPPeer::SRVACK*) msg;
     unsigned int idx = 0;

     std::cout << "SRVACK received from " << ack->source.getHostAddr() << std::endl;
     while( idx < myDAs_.size() && ack->source != myDAs_[idx]->addr ) {
          idx++;
     }

     if( idx < myDAs_.size() ) {
          myDAInfos_[idx].wfAck = false;
          myDAInfos_[idx].resendTimer.stop();
          myDAInfos_[idx].lifetime = myDAInfos_[idx].refreshFreq;
          
     } else {
          _DOORS_DEBUG(0, "SRVACK received from unknown DA in SAReg!");
     }
     
     return false;
}

bool SAReg :: ready_attrRqst( Message *msg )
{
    SLPPeer :: ATTRRQST * rqst = (SLPPeer :: ATTRRQST *) msg;
   
    SLPPeer :: ATTRRPLY * rply = (SLPPeer :: ATTRRPLY *)peer_.create (SLPPeer :: attrrply);

    // If request is for a service type (not a URL), return all attributes
    // if we are of this requested type
    if( rqst->URL.isPureType() ) {
        if( url_.isOfServiceType(rqst->URL.getServiceType()) ) {
            rply->attributeList = attrs_;
        }
    } else {
        // If taglist is omitted in request, return all attributes
        if( rqst->tagList.size() == 0 ) {
            rply->attributeList = attrs_;
        } else {
            // Note: going through the taglist and the attribute list could be done
            // faster by keeping attribute list sorted and then comparing it against the taglist
            for( unsigned int i = 0 ; i < rqst->tagList.size() ; i++ ) {
                for( unsigned int j = 0; j < attributes_.size(); j++ ) {
                    if( rqst->tagList[i] == attributes_[j].AttributeName ) {
                        
                        if( attributes_[j].AttributeValue.length() ) {
                            rply->attributeList.push_back(
                                std::string(
                                    attributes_[j].AttributeName+'=' + attributes_[j].AttributeValue
                                    )
                                );
                        } else {
                            rply->attributeList.push_back(attributes_[j].AttributeName);
                        }
                        
                    }
                }
            }
        }
    }

    if( rply->attributeList.size() ) {
        rply->XID = rqst->XID;
        rply->dest = rqst->source;
        peer_.putMessage( rply );
    } else {
        delete rply;
    }
    
    return false;
}
