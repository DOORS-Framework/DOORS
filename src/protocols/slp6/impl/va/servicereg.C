//  Copyright 2004
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

$Log: servicereg.C,v $
Revision 1.4  2007/01/15 11:03:19  bilhanan
devt

Revision 1.3  2007/01/11 11:35:48  bilhanan
devt

Revision 1.2  2006/12/21 13:13:57  bilhanan
devt

Revision 1.1  2006/08/10 15:39:06  bilhanan
initial commit

Revision 1.6  2004/07/16 09:10:58  hartmanj
Commented out DYNAMIC_SYMBOLs. Enable only if you want loads of Lemon output.

Revision 1.5  2004/07/12 07:26:00  hartmanj
General development

Revision 1.4  2004/06/17 08:56:30  hartmanj
Development.

Revision 1.3  2004/05/20 07:00:08  hartmanj
Manual fix to last comment

Revision 1.2  2004/05/19 13:52:02  hartmanj
Update

Revision 1.1  2004/05/12 08:33:20  hartmanj
Development

Revision 1.1  2004/01/28 09:23:31  hartmanj
Initial revision. Compiles.

  
*/

#include <string>
#include <doors/slptypes.h>
#include "servicereg.h"
#include "serviceregsm.h"
#include "vaent.h"
#include <doors/debug.h>


ServiceReg :: ServiceReg( std::string n, Uint32 slphandle, VAEntity *vaEnt,  ServiceRegSM *sm )
     :  PTask (n, vaEnt->getScheduler (), sm),
        entity( this ),
        up( vaEnt->up ),
	mdport (this),
        peer( vaEnt->peer ),
        slpHandle(slphandle),
        local_only(false),
        vaEntity( vaEnt ),
        registration(0),
        myconfig( VAConfigurator::Instance() ),
        taskid(0),
        aaconn(0), daconn(0),
        aaport(this), daport(this)
{
     _DOORS_DEBUG(0,"ServiceReg :: ServiceReg: "<< n );
     state = ServiceRegSM :: Ready;
     myScope = "DEFAULT";
}

bool ServiceReg :: ready_registerservice( Message *msg )
{
    _DOORS_DEBUG(0, "ServiceReg :: ready_registerservice");
    return Initialise( (VASAP :: registerService*) msg );
}

bool ServiceReg :: ready_deregisterservice( Message *msg )
{
    _DOORS_DEBUG(0, "ServiceReg :: ready_deregisterservice");
    delete this;
}

bool ServiceReg :: Initialise( VASAP :: registerService *RegMsg )
{
    Byte *urlBytes = (Byte*)RegMsg->URL;
    std::string urlStr((char*)urlBytes);
    delete urlBytes;

    url = ServiceURL(urlStr, RegMsg->lifetime);
    if( url.isTransient() ) {
        url.updateAddr( vaEntity->getCurrAddr().getHostAddr() );
        _DOORS_DEBUG(0, "Service URL: is ``" << url.getURL() << "'' from vaEntity: " <<vaEntity->getCOAAddr().getHostAddr() );
    }

    lifetime = RegMsg->lifetime;

    if( RegMsg->local_only != 0 ) {
        local_only = true;
    }

    if( RegMsg->attrs.length() ) {
        char *a = new char[RegMsg->attrs.length()];
        unsigned int i = 0;
        while( RegMsg->attrs.length() ) {
            a[i] = RegMsg->attrs.getFirst();
            if(a[i] == ',') {
                a[i] = 0;
                if( i ) {
                    attrs.push_back(std::string(a));
                }
                i = 0;
                continue;
            }
            i++;
        }
        if( i ) {
            a[i]=0;
            attrs.push_back(std::string(a));
        }
        delete [] a;
    }

    vaEntity->createAttributeList( attributes, attrs );

    vaEntity->joinMCGroup( url.getServiceType() );

    if( url.getServiceType() != url.getAbstractType() ) {
        vaEntity->joinMCGroup( url.getAbstractType() );
    }

    createRegistration( );

    if( vaEntity->isAAPresent() ) {
        aaconn = vaEntity->getNewAgentConn();
        aaport.connect( &aaconn->servicereg );
        aaconn->Initialise( &registration, lifetime );
        aaconn->setAgentHandleAA( );

        std::ostringstream cname;
        cname << getName () << ".agentconn.aaconn";
        //DYNAMIC_SYMBOL( aaconn, cname.str(), this );

        ServiceRegSAP :: AAUpdate *aaupdate = new ServiceRegSAP :: AAUpdate;
        aaupdate->newaddr = vaEntity->getAAAddr();
        aaport.putMessage(aaupdate);
    } else {
        aaconn = 0;
    }

    if( vaEntity->isDAPresent() ) {
        daconn = vaEntity->getNewAgentConn();
        daport.connect( &daconn->servicereg );
        daconn->Initialise( &registration, lifetime );
        daconn->setAgentHandleDA( );

        std::ostringstream cname;
        cname << getName () << ".agentconn.daconn";
        //DYNAMIC_SYMBOL( daconn, cname.str(), this );

        ServiceRegSAP :: DAUpdate *daupdate = new ServiceRegSAP :: DAUpdate;
        daupdate->newaddr = vaEntity->getDAAddr();
        daport.putMessage(daupdate);
    } else {
        daconn = 0;
    }

    std::cout << "*****DEBUG: aaconn: " << aaconn
              << ", daconn: " << daconn
              << ", taskid: " << taskid
              << ", mySCope: " << myScope
              << ", url: " << url.getURL()
              << ", attrs.size(): " << attrs.size() << std::endl;

    return false;
}

ServiceReg :: ~ServiceReg (void)
{
    //DELETE_SYMBOL(this);

    if( aaconn )
        vaEntity->releaseAgent(aaconn);

    if( daconn )
        vaEntity->releaseAgent(daconn);

}

bool ServiceReg :: isFree() const
{
    return ( registration == 0 );
}

void ServiceReg :: createRegistration()
{
    SLPPeer :: SRVREG *reg
        = (SLPPeer :: SRVREG* ) peer.create ( SLPPeer :: srvreg );

     reg->url = url;
     reg->serviceType = url.getServiceType();
     reg->attributeList = attrs;
     reg->scopeList.push_back(myScope);

     reg->errorCode = 0;

     registration = reg;
}

bool ServiceReg :: isOfType( const std::string &type )
{
     if( url.isOfServiceType(type) ) {
          return true;
     }
     
     return false;
}

Uint32 ServiceReg :: getSlpHandle()
{
    return slpHandle;
}

ServiceURL ServiceReg :: getURL()
{
     return url;
}

std::string ServiceReg :: getServiceType(void)
{
    return url.getServiceType();
}

bool ServiceReg :: usingXID( Uint16 xid )
{
    _DOORS_DEBUG(0, "ServiceReg :: usingXID: xid == " << xid);
    return ((aaconn!=0) && aaconn->usingXID(xid)) || ((daconn!=0) && daconn->usingXID(xid));
}

bool ServiceReg::ready_addattributes( Message *msg )
{
    _DOORS_DEBUG(0, "ServiceReg::ready_addAttributes");
    return false;
}

bool ServiceReg::ready_deleteattributes( Message *msg )
{
    _DOORS_DEBUG(0, "ServiceReg::ready_deleteAttributes");
    return false;
}

bool ServiceReg :: ready_movement( Message *msg )
{
    _DOORS_DEBUG(0, "ServiceReg :: ready_movement");

    if( !local_only ) {
        _DOORS_DEBUG(0, "! local_only");
    
        vaEntity->joinMCGroup( url.getServiceType() );

        if( url.getServiceType() != url.getAbstractType() ) {
            vaEntity->joinMCGroup( url.getAbstractType() );
        }

        if( url.isTransient() ) {
            url.updateAddr( vaEntity->getCurrAddr().getHostAddr() );
            _DOORS_DEBUG(0, "New Service URL: is ``"
                         << url.getURL() << "'' from vaEntity: "
                         <<vaEntity->getCOAAddr().getHostAddr());
        }
    }

    if( aaconn ) {
      ServiceRegSAP::Movement *aamsg = (ServiceRegSAP :: Movement *) vaEntity->sreg_sap.create (ServiceRegSAP::movement);
        aaport.putMessage(aamsg);
    }

    if( daconn ) {
      ServiceRegSAP::Movement *damsg = (ServiceRegSAP :: Movement *) vaEntity->sreg_sap.create (ServiceRegSAP::movement);
      daport.putMessage(damsg);
    }

    if( local_only ) {
        _DOORS_DEBUG(0, "Service was local only, deleting service: URL: " << url.getURL());
        delete this;
    }

    return false;
}

bool ServiceReg :: ready_aaupdate( Message *msg )
{
    _DOORS_DEBUG(0, "ServiceReg :: ready_aaupdate");
    bool rc = true;

    if( aaconn ) {
        rc = aaport.putMessage(msg->clone());
    } else {
      aaconn = vaEntity->getNewAgentConn();
      std::ostringstream cname;
      cname << getName () << ".agentconn.aaconn";
      //DYNAMIC_SYMBOL( aaconn, cname.str(), this );
      aaport.connect( &aaconn->servicereg );
      aaconn->Initialise( &registration, lifetime );
      aaconn->setAgentHandleAA( );
      rc = aaport.putMessage(msg->clone());
    }

    return rc;
}

bool ServiceReg :: ready_daupdate( Message *msg )
{
    bool rc = true;

    if( daconn ) {
        rc = daport.putMessage(msg->clone());
    } else {
      daconn = vaEntity->getNewAgentConn();
      std::ostringstream cname;
      cname << getName () << ".agentconn.daconn";
      //DYNAMIC_SYMBOL( aaconn, cname.str(), this );
      daport.connect( &daconn->servicereg );
      daconn->Initialise( &registration, lifetime );
      daconn->setAgentHandleDA(  );
      rc = daport.putMessage(msg->clone());
    }

    return rc;
}

bool ServiceReg :: ready_default (Message *m)
{
    _DOORS_DEBUG(0, "ServiceReg :: ready_default, " << m << m->isLocked() << m->getTarget() << &entity);
    return false;
}

bool ServiceReg :: ready_srvack( Message *msg )
{
    _DOORS_DEBUG(0, "ServiceReg :: ready_srvrack");
    bool rc = false;

    Uint16 xid = ((SLPPeer :: SRVACK*)msg)->XID;

    if( aaconn && aaconn->usingXID( xid ) ) {
        msg->lock();
        aaport.putMessage(msg);
        rc = true;
    } else if( daconn && daconn->usingXID( xid ) ) {
        msg->lock();
        daport.putMessage(msg);
        rc = true;
    }

    return rc;
}

bool ServiceReg :: ready_srvrqst( Message *msg )
{
    _DOORS_DEBUG(0, "ServiceReg :: ready_srvRqst");

    SLPPeer :: SRVRQST *req = (SLPPeer :: SRVRQST*) msg;

    if( !( req->servicetype == url.getServiceType() || req->servicetype == url.getAbstractType() ) ) {
        _DOORS_DEBUG(0, "[" << this->name << "]-Discarding request for " << req->servicetype);
        _DOORS_DEBUG(0, "[" << this->name << "]-I am " << url.getServiceType() );
        return false;
    }

    SLPPeer :: SRVRPLY *reply = (SLPPeer :: SRVRPLY*)peer.create(SLPPeer :: srvrply);

    reply->dest = req->source;
    reply->XID = req->XID;
    reply->errorCode = 0;
    reply->urlEntries.push_back( url );

    peer.putMessage(reply);
    
    return false;
}

bool ServiceReg :: ready_attrrqst( Message *msg )
{
    _DOORS_DEBUG(0, "ServiceReg :: ready_attrRqst");

    SLPPeer :: ATTRRQST *req = (SLPPeer :: ATTRRQST*) msg;

    if( req->URL == url ) {
        _DOORS_DEBUG(0, "[" << this->name << "]-Discarding request for attributes of " << req->URL.getURL() );
        _DOORS_DEBUG(0, "[" << this->name << "]-I am " << url.getURL() );
        return false;
    }
    
    SLPPeer :: ATTRRPLY *reply = (SLPPeer :: ATTRRPLY*)peer.create(SLPPeer :: attrrply);

    reply->dest = req->source;
    reply->XID = req->XID;
    reply->errorCode = 0;

    // If request is for a service type (not a URL), return all attributes
    // if we are of this requested type
    if( req->URL.isPureType() ) {
        if( url.isOfServiceType(req->URL.getServiceType()) ) {
            reply->attributeList = attrs;
        }
    } else {
        // If taglist is omitted in request, return all attributes
        if( req->tagList.size() == 0 ) {
            reply->attributeList = attrs;
        } else {
            // Note: going through the taglist and the attribute list could be done
            // faster by keeping attribute list sorted and then comparing it against the taglist
            for( unsigned int i = 0 ; i < req->tagList.size() ; i++ ) {
                for( unsigned int j = 0; j < attributes.size(); j++ ) {
                    if( req->tagList[i] == attributes[j].AttributeName ) {
                        
                        if( attributes[j].AttributeValue.length() ) {
                            reply->attributeList.push_back(
                                std::string(
                                    attributes[j].AttributeName+'=' + attributes[j].AttributeValue
                                    )
                                );
                        } else {
                            reply->attributeList.push_back(attributes[j].AttributeName);
                        }
                        
                    }
                }
            }
        }
    }

    if( reply->attributeList.size() ) {
        peer.putMessage( reply );
    } else {
        delete reply;
    }
    
    return false;
}
