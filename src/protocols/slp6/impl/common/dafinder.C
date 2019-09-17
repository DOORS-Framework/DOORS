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

$Log: dafinder.C,v $
Revision 1.14  2004/03/25 09:19:16  bilhanan
fixed insertDA to prevent emmpty string copying.

Revision 1.13  2004/03/25 08:47:09  hartmanj
Reorganising DAFinder and removing some unnecessary fields and methods. sendSingleDARequest() moved to SLPCommon::solicitDirectoryAgent()

Revision 1.12  2004/03/19 11:21:49  hartmanj
Adapting new SLPCommon API

Revision 1.11  2004/03/10 07:10:31  bilhanan
fixed getMCAddr call

Revision 1.10  2004/03/01 09:02:36  hartmanj
Changed type of DAs_ member from vector<DAInfo*> to list<DAInfo*>

Revision 1.9  2004/02/26 08:57:57  hartmanj
A quick fix to sendSingleDARequest()

Revision 1.8  2004/02/18 09:27:54  bilhanan
DAFinder::getDAs function parameter changes to prevent crashes with sun
c++ compiler.

Revision 1.7  2004/02/04 15:05:26  hartmanj
Removed class DAInfo in favour of struct DAInfo. Cleaning and bugfixes.

Revision 1.6  2003/10/15 10:47:20  bilhanan
msg locking functions removed.

Revision 1.5  2003/10/03 14:33:08  bilhanan
map insertion changed to prevent default initialization and explicit value
assignment.

Revision 1.4  2003/08/24 22:13:52  bilhanan
constructor changed.

Revision 1.3  2003/06/04 11:09:54  kalliosa
Licence clause added

  
*/

#include <doors/slpcommon.h>
#include <doors/dafinder.h>


#include <doors/debug.h>

DAFinder :: DAFinder ()
{
}

DAFinder :: ~DAFinder()
{
     std::list<DAInfo*>::iterator daIter = DAs_.begin();

     while(daIter != DAs_.end())
     {
          delete (*daIter);
     }

     DAs_.clear();
}


bool DAFinder :: updateDAList ( SLPPeer :: DAADVERT *advert )
{
  _DOORS_DEBUG(0,"DAFinder :: updateDAList");

   InetAddr6 NewAddr = advert->url.getHostAddr();
   _DOORS_DEBUG(0,"DAFinder :: ready_daAdvert () - " << NewAddr.addrToString());

   // Find the DA from our list
   unsigned int daIdx = 0;
   std::list<DAInfo*>::iterator daIter = DAs_.begin();
   for( ; daIdx < DAs_.size() && (*daIter)->addr != NewAddr ; daIdx++) {
        daIter++;
   }

   if( daIdx < DAs_.size() ) {
        // If the DA was in the list
        _DOORS_DEBUG(0, "DAFinder :: ready_daAdvert [ updating DA info from "
                     << NewAddr.addrToString() );
        (*daIter)->scopes = advert->scopeList;
        (*daIter)->attrs = advert->attributeList;
   } else {
        // And if the DA was not yet in the list
        _DOORS_DEBUG(0, "DAFinder :: ready_daAdvert [ saving new DA address and info from "
                     << NewAddr.addrToString() );
        DAInfo *newDA = new DAInfo;
        newDA->addr = NewAddr;
        newDA->scopes = advert->scopeList;
        newDA->attrs = advert->attributeList;
        DAs_.push_back(newDA);
        return true;
   }

   return false;
}

InetAddr6 DAFinder :: getDA( const std::string &scope )
{
     if( scope != "DEFAULT" ) {
          std::list<DAInfo*>::iterator daIter = DAs_.begin();
          for( ; daIter != DAs_.end() ; daIter++ ) {
               for( unsigned int s = 0 ; s < (*daIter)->scopes.size() ; s++ ) {
                    if( (*daIter)->scopes[s] == scope ) {
                         return (*daIter)->addr;
                    }
               }
          }
     } else {
          if( DAs_.size() > 0 )
          {
               return (*DAs_.begin())->addr;
          }
     }

     _DOORS_DEBUG(0, "DAFinder :: getDA() - [ No DAs in network and DA requested from list ]");
     return InetAddr6(0,"0::0");
}

bool DAFinder :: insertDA( DAInfo *newDA )
{
    DAInfo *copy = new DAInfo;
    copy->addr = newDA->addr;
    copy->scopes = newDA->scopes;
    copy->attrs = newDA->attrs;

    DAs_.push_back(copy);

    return true;
}

bool DAFinder :: insertDA( InetAddr6 daAddr, const std::string &scope )
{
    DAInfo *copy = new DAInfo;
    copy->addr= daAddr;
    copy->scopes.push_back(scope);

    DAs_.push_back(copy);

    return true;
}

bool DAFinder :: insertDA( std::string daAddr, Uint16 port, std::string scope, std::string attrString )
{
    DAInfo *copy = new DAInfo;
    copy->addr.set(port, daAddr);
    if (!scope.empty())
      copy->scopes.push_back(scope);
    if (!attrString.empty())
      copy->attrs.push_back(attrString);
    
    DAs_.push_back(copy);

    return true;
}

DAFinder::DAInfo* DAFinder::getDAInfo( const InetAddr6 &addr )
{
     for(std::list<DAInfo*>::iterator daIter = DAs_.begin();
         daIter != DAs_.end() ; daIter++ ) {
          if( (*daIter)->addr == addr )
               return (*daIter);
     }

     return NULL;
}

void DAFinder :: getDAs( const std::string &scope, std::vector<DAInfo*> &fitScopeDAs)
{
     for(std::list<DAInfo*>::iterator daIter = DAs_.begin();
         daIter != DAs_.end() ; daIter++ ) {
          for( unsigned int s = 0 ; s < (*daIter)->scopes.size() ; s++ ) {
               if( (*daIter)->scopes[s] == scope )
                    fitScopeDAs.push_back((*daIter));
          }
     }

}

bool DAFinder :: isDAPresent()
{
     _DOORS_DEBUG(0, "DAFinder :: isDAPresent() - DAs_.size() == " << DAs_.size() );
     if( DAs_.size() != 0 )
          return true;

     return false;
}
