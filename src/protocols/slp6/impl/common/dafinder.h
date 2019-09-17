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

$Log: dafinder.h,v $
Revision 1.9  2007/09/03 12:27:42  bilhanan
extra qualifiers removed for G++ 4.x compatibility.

Revision 1.8  2004/03/25 08:47:09  hartmanj
Reorganising DAFinder and removing some unnecessary fields and methods. sendSingleDARequest() moved to SLPCommon::solicitDirectoryAgent()

Revision 1.7  2004/03/01 09:02:36  hartmanj
Changed type of DAs_ member from vector<DAInfo*> to list<DAInfo*>

Revision 1.6  2004/02/18 09:27:54  bilhanan
DAFinder::getDAs function parameter changes to prevent crashes with sun
c++ compiler.

Revision 1.5  2004/02/04 15:05:27  hartmanj
Removed class DAInfo in favour of struct DAInfo. Cleaning and bugfixes.

Revision 1.4  2003/10/03 14:33:48  bilhanan
tidying up

Revision 1.3  2003/08/24 22:13:52  bilhanan
constructor changed.

Revision 1.2  2003/06/04 11:09:54  kalliosa
Licence clause added

  
*/

#ifndef DAFINDER_H
#define DAFINDER_H

#include <list>
#include <doors/ptb.h>
#include <doors/inetaddr6.h>
#include <doors/slppdu.h>


class DAFinder {

 public:
     DAFinder ();
     virtual ~DAFinder();
     bool updateDAList ( SLPPeer :: DAADVERT *advert );
     InetAddr6 getDA( const std::string &scope = "DEFAULT" );

     struct DAInfo {
          InetAddr6 addr;
          StringVector scopes;
          AttrVector attrs;
     };

    bool insertDA( DAInfo *newDA );
    bool insertDA( InetAddr6 daAddr, const std::string &scope = "DEFAULT" );
    bool insertDA( std::string daAddr, Uint16 port, std::string scope, std::string attrString );

     void getDAs( const std::string &scope, std::vector<DAInfo*> &fitScopeDAs);
     DAInfo* getDAInfo( const InetAddr6 &addr );

     bool isDAPresent();

private:

     std::list<DAInfo*> DAs_;
};

#endif // DAFINDER_H
