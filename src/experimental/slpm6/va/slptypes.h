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

$Log: slptypes.h,v $
Revision 1.2  2004/05/20 07:00:09  hartmanj
Manual fix to last comment

Revision 1.1  2004/01/28 09:23:30  hartmanj
Update

Revision 1.2  2003/06/04 11:09:56  kalliosa
Licence clause added

  
*/

 // typedefs for DOORS' SLP component

#ifndef SLPTYPES_H
#define SLPTYPES_H

#include <doors/debug.h>

#include <vector>
#include <string>
#include <set>
#include <doors/hsi.h>
#include <doors/ptb.h>
//#include <doors/datatypes.h>
#include <doors/timer.h>
#include <doors/frame.h>

//#include "serviceurl.h"
//#include "servicetype.h"

class ServiceURL;
class ServiceType;
using std::vector;
using std::string;
using std::set;
// constants
//const int RQST_RETRANS = 10; // request retransmission time
const int CONFIG_MC_MAX =15;
const int CONFIG_START_WAIT = 3;
const int CONFIG_RETRY = 2;
const int CONFIG_RETRY_MAX = 15;

  
// own datatypes
typedef std::string ServiceLocationAttribute;
typedef std::string AuthBlock;


typedef vector<AuthBlock> AuthBlockVector;
typedef vector<string> StringVector;
typedef vector<ServiceURL> URLVector;
typedef set<InetAddr6> AddrVector;
typedef vector<ServiceLocationAttribute> AttrVector;
//typedef string ServiceType;

// base class for all SLP messages
class SLPHeader
{
 public:
   Uint8 version;
   Uint8 functionID;
   Uint32 length;
   bool overflow;
   bool requestMcast;
   bool fresh;
   Uint16 XID;
   Uint32 slpHandle;
   std::string langTag;

   // not really a part of the header..
   InetAddr6 dest;
   InetAddr6 source;


   SLPHeader ();
   SLPHeader ( const SLPHeader &src );
   virtual ~SLPHeader ();

   //   virtual Uint32 getLength()=0;

 
// protected: for testing
   
   Uint16 decodeHeader( Frame &data );
   Uint16 decoded_; // amount of decoded octets
   
   /**
    * @return number on octets in decoded header
    */
   virtual Uint32 getRqstLength();
   
   bool getFlag( Uint16 flags, Uint16 n );
   void SLPHeader :: setFlag( Uint16 &flags, Uint16 n, bool state);
   Uint16 get16( Frame &data );
   Uint32 get24( Frame &data );
   Uint32 get32( Frame &data );
   Uint32 getn( Frame &data, Uint16 n  );
   void getServiceURLVector( Frame &data, URLVector &vc );
   void putServiceURLVector( Frame &data, URLVector &vc );

   void putAddr( Frame &data, InetAddr &a );
   InetAddr6 getAddr( Frame &data );
   void getAddrVector( Frame &data, AddrVector &vc );
   void putAddrVector( Frame &data, AddrVector &vc );
   
   bool strlist2vec( std::string &st, StringVector &vc);
   void printstrvec( StringVector vc );
   /**
    * parses length of string from Frame and reads comma-separated 
    * list of strings to vector. 
    **/
   void getStringVector( Frame &data, StringVector &vc );
   /**
    * parses string from Frame. By default assumes that first 16 bits in frame 
    * indicate the length of the string. If length is given, getString just 
    * reads that many octets and stuffs them into the string
    * 
    **/
   void getString( Frame &data, std::string &str ); // Uint32 length = 0 );
   ServiceURL getUrlEntry( Frame &data );
   void putUrlEntry( Frame &data, ServiceURL &url);

   bool vec2strlist( StringVector &vc, std::string &st);
   void putString( Frame &data, std::string str );
   void putStringVector( Frame &data, StringVector &vc );
   void put16 ( Frame &data, Uint16 n );
   void put24 ( Frame &data, Uint32 n );
   void put32 ( Frame &data, Uint32 n ); 
   void encodeHeader( Frame &data );
   void putLength(Frame &data);
   void printFrame(Frame &data);
 private:
};                                                                          

#endif // SLPTYPES_H
