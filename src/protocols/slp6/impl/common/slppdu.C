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

$Log: slppdu.C,v $
Revision 1.17  2006/12/01 09:07:35  bilhanan
Added ADDRRQST and ADDRRPLY

Revision 1.16  2006/07/29 21:41:10  bilhanan
groundwork for AA imporovements

Revision 1.15  2004/06/17 08:52:12  hartmanj
Fixed AAADVERT :: decode. Advert URLs were not stored in aa_url and da_url.

Revision 1.14  2004/04/14 09:18:15  hartmanj
Bug fixing

Revision 1.13  2004/04/08 10:51:55  hartmanj
Small fixes

Revision 1.12  2004/04/07 13:43:34  hartmanj
slpheader.h steps in

Revision 1.11  2004/03/22 11:07:37  hartmanj
Fixed attribute request decoding

Revision 1.10  2004/03/01 09:03:52  hartmanj
Fixed cut-n-paste typo in _DOORS_DEBUG statement

Revision 1.9  2004/02/10 11:36:39  hartmanj
Added putLength() call to SRVACK::encode()

Revision 1.8  2004/02/10 11:25:31  hartmanj
Removed ntohx and htonx:s

Revision 1.7  2004/02/10 11:16:16  hartmanj
Removed errorCode = ntohs (get16(data)); which shouldn't be there as there is no field errorCode in SRVREG pdu

Revision 1.6  2004/02/03 14:43:41  bilhanan
printFrame() removed.

Revision 1.5  2004/02/03 14:30:20  bilhanan
network byte ordering fix.

Revision 1.4  2004/01/27 07:49:04  hartmanj
Fixed address handling in decode() functions.

Revision 1.3  2003/07/23 10:10:59  bilhanan
AA Advert + encoding/decoding functions introduced

Revision 1.2  2003/06/04 11:09:56  kalliosa
Licence clause added

  
*/

#include <arpa/inet.h>
#include <doors/udp6.h>
#include <doors/frame.h>
#include <doors/slptypes.h>
#include <doors/slppdu.i>
#include <doors/slppdu.h>




#include <doors/debug.h>



Sint16 SLPPeer :: identify (Message *msg) 
{
   _DOORS_DEBUG(0, "slppeer::identify" );

   if( msg->getType() == Udp6SAP :: errind )
   {

      _DOORS_DEBUG(0, "errind identified.");
      Udp6SAP :: Errind *err = (Udp6SAP :: Errind *) msg;
      Byte *d = err->data;
      
      std::string reason = (char *) d;
      
      _DOORS_DEBUG(0, "code: " << err->code << " reason: " <<  reason );
      
      return -1;
   }
   Udp6SAP :: Dtind * dti = 
     dynamic_cast< Udp6SAP :: Dtind *> (msg);

   Frame & datax = ( dti->data );

   _DOORS_DEBUG(0,"identified data from " << dti->source.addrToString() );
   _DOORS_DEBUG(0,"destination of data " << dti->dest.addrToString() );

   Uint16 version = datax.read(0);
   if( version != 2 ) {
	 _DOORS_DEBUG(0, "Received a packet with invalid version number.");
	 return other;
   }

   // function id is the second octet, indexing is zero based
   Uint16 fid = datax.read ( 1 );   

   return fid;
}


Uint16 SLPPeer :: SRVRQST :: decode (Message *msg)
{
   _DOORS_DEBUG(0, "SRVRQST::decode()" );
   decoded_ = 0;
   Udp6SAP :: Dtind *dti = (Udp6SAP :: Dtind *)msg;
   _DOORS_DEBUG(0, "  Udp6SAP :: Dtind *dti -> source==" << dti->source.addrToString() );

   source = dti->source;
   dest  = dti->dest;
     
   Frame &data = dti->data;
   
   decoded_ = decodeHeader( data );    
   
   getAddrVector( data, PRList );
   getString( data, servicetype );
   getStringVector( data, scopeList );
   getString( data, predicate );
   getString( data, spi );
   
   return decoded_;      
}


Message*  SLPPeer :: SRVRQST :: encode (void)
{
   _DOORS_DEBUG(0, "SRVRQST::encode: servicetype: " << servicetype  );
   // create new message for Udp6SAP 
   Udp6SAP :: Dtreq *dtr = new Udp6SAP :: Dtreq;

//   dtr->unlock();
   // create data frame
   Frame data;
   
   // put all necessary stuff to newly created frame
   // put common stuff to header
   functionID = 1;
   encodeHeader( data );
  
  putAddrVector( data, PRList );
  putString( data, servicetype );
  putStringVector( data, scopeList );
  putString( data, predicate );
  putString( data, spi );

   putLength( data ); 
   
   dtr->dest = dest; // set destination adddress
   dtr->data = data; // copy data frame to outgoing message
   
   return dtr;
}      



Uint16 SLPPeer :: SRVRPLY :: decode (Message *msg)
{
   _DOORS_DEBUG(0, "SRVRPLY::decode" );

   decoded_ = 0;
   
   Udp6SAP :: Dtind *message = (Udp6SAP :: Dtind *)msg; 
   source = message->source;
   std::cout << "SRVRPLY :: decode - source: " << source.getHostAddr() << std::endl;
   dest  = message->dest;
   
   _DOORS_DEBUG(0, "decoding srvrply from: " << message->source.addrToString() );
   _DOORS_DEBUG(0, "addressa assigned to variable: " << source.addrToString() );
   
   Frame &data = message->data;

   decoded_ = decodeHeader( data );
   
   errorCode = get16(data);
   getServiceURLVector( data, urlEntries);
   
   return decoded_;   
}

Message* SLPPeer :: SRVRPLY :: encode (void)
{
   _DOORS_DEBUG(0, "SRVRPLY::encode" );

   Udp6SAP :: Dtreq *dtr = new Udp6SAP :: Dtreq;
   Frame data;
   
   // put common stuff to header
   functionID = 2;   encodeHeader( data );
  
   put16( data, errorCode);
   putServiceURLVector( data, urlEntries);


   
   putLength( data ); 

  dtr->dest = dest; // set destination adddress
  dtr->data = data; // copy data frame to outgoing message
   
  return dtr;

}      


Uint16 SLPPeer :: SRVREG :: decode (Message *msg)
{
   _DOORS_DEBUG(0, "SRVREG::decode" );

   decoded_ = 0;

   Udp6SAP :: Dtind *message = (Udp6SAP :: Dtind *)msg; 
   source = message->source;
   dest  = message->dest;
     
   Frame &data = message->data;

   decoded_ = decodeHeader( data );
   

   url = getUrlEntry( data );
   getString( data, serviceType );
   _DOORS_DEBUG(0, "SRVREG::decode - serviceType == " << serviceType);
   getStringVector( data, scopeList );
   getStringVector( data, attributeList );

   //   getAuthBlockVector( data, AttrAuthBlocks );

   return decoded_;
}




Message* SLPPeer :: SRVREG :: encode (void)
{

   _DOORS_DEBUG(0, "SRVREG::encode" );

   Udp6SAP :: Dtreq *dtr = new Udp6SAP :: Dtreq;
  Frame data;

   functionID = 3;
   encodeHeader( data );

  putUrlEntry( data, url);

  putString( data, serviceType);

  putStringVector( data, scopeList );
  putStringVector( data, attributeList );
  put16( data, 0); // number of attribute auth blocks

  putLength( data );

  dtr->dest = dest; // set destination adddress
  dtr->data = data; // copy data frame to outgoing message
   
  return dtr;


}      


Uint16 SLPPeer :: SRVDEREG :: decode (Message *msg)
{
   decoded_ = 0;

   Udp6SAP :: Dtind *message = (Udp6SAP :: Dtind *)msg; 
   source = message->source;
   dest  = message->dest;
     
   Frame &data = message->data;

   decoded_ = decodeHeader( data );
   
   getStringVector( data, scopeList );
   url = getUrlEntry( data );
   getStringVector( data, tagList);

   return decoded_;
}




Message* SLPPeer :: SRVDEREG :: encode (void)
{
  Udp6SAP :: Dtreq *dtr = new Udp6SAP :: Dtreq;
  Frame data;
   functionID = 4;
   encodeHeader( data );

  putStringVector( data, scopeList );
  putUrlEntry( data, url );
  putStringVector( data, tagList );


  dtr->dest = dest; // set destination adddress
  dtr->data = data; // copy data frame to outgoing message
   
  return dtr;

}      


Uint16 SLPPeer :: SRVACK :: decode (Message *msg)
{
   _DOORS_DEBUG(0, "SRVACK::decode" );
   decoded_ = 0;

   Udp6SAP :: Dtind *message = (Udp6SAP :: Dtind *)msg; 
   source = message->source;
   dest  = message->dest;
     
   Frame &data = message->data;

   decoded_ = decodeHeader( data );
   
   errorCode = get16(data);

   return decoded_;
}




Message* SLPPeer :: SRVACK :: encode (void)
{
   _DOORS_DEBUG(0, "SRVACK::encode" );

  Udp6SAP :: Dtreq *dtr = new Udp6SAP :: Dtreq;
  Frame data;
   functionID = 5;
   encodeHeader( data );

  put16( data, errorCode);

  putLength( data );


  dtr->dest = dest; // set destination adddress
  dtr->data = data; // copy data frame to outgoing message
   
  return dtr;

} 

// newly put routines here

Uint16 SLPPeer :: ATTRRQST :: decode (Message *msg)
{
   decoded_ = 0;

   Udp6SAP :: Dtind *message = (Udp6SAP :: Dtind *)msg; 
   source = message->source;
   dest  = message->dest;
     
   Frame &data = message->data;
   std::string tmpurl;

   decoded_ = decodeHeader( data );

   getAddrVector( data, PRList);
   // Note: here URL can be "service:something.." or just "nfs://somewhere.."
   // [RFC2608 : 10.3]
   getString( data, tmpurl);
   // Check if we need to append "service:"
   if( tmpurl.size() ) {
       if( tmpurl.size() > 8 ) {
           if( tmpurl.substr(0,8) != std::string("service:") ) {
               tmpurl = "service:" + tmpurl;
           }
       } else {
           tmpurl = "service:" + tmpurl;
       }
   }
   URL = ServiceURL(tmpurl);
   getStringVector( data, scopeList);
   getStringVector( data, tagList);
   getString( data, spiString);

   return decoded_;
}




Message* SLPPeer :: ATTRRQST :: encode (void)
{
   Udp6SAP :: Dtreq *dtr = new Udp6SAP :: Dtreq;
   Frame data;
   functionID = 6;
   encodeHeader( data );

   
   std::string tmpurl = URL.getURL();

   putAddrVector( data, PRList);
   putString( data, tmpurl );
   putStringVector( data, scopeList);
   putStringVector(data, tagList);
   putString( data, spiString);

  putLength( data );

  dtr->dest = dest; // set destination adddress
  dtr->data = data; // move data frame to outgoing message
   
  return dtr;
} 


Uint16 SLPPeer :: ATTRRPLY :: decode (Message *msg)
{
   decoded_ = 0;

   Udp6SAP :: Dtind *message = (Udp6SAP :: Dtind *)msg; 
   source = message->source;
   dest  = message->dest;
     
   Frame &data = message->data;

   decoded_ = decodeHeader( data );

   errorCode = get16(data);
   getStringVector( data, attributeList );
   data.getFirst(); // attr auth block count, should be zero
   // rest is auth blocks if any, just ignore

   return decoded_;
}

Message* SLPPeer :: ATTRRPLY :: encode (void)
{
  Udp6SAP :: Dtreq *dtr = new Udp6SAP :: Dtreq;
  Frame data;
   functionID = 7;
   encodeHeader( data );

  put16( data, errorCode);
  putStringVector( data, attributeList);
  data.putLast(0);

  putLength( data );

  dtr->dest = dest; // set destination adddress
  dtr->data = data; // move data frame to outgoing message
   
  return dtr;
} 

Uint16 SLPPeer :: DAADVERT :: decode (Message *msg)
{
   _DOORS_DEBUG(0, "DAADVERT::decode" );

   decoded_ = 0;

   Udp6SAP :: Dtind *message = (Udp6SAP :: Dtind *)msg; 
   source = message->source;
   dest  = message->dest;
     
   Frame &data = message->data;
   std::string tmpurl;

   decoded_ = decodeHeader( data );

   errorCode = get16(data);
   DABootStamp = get32(data);
   getString( data, tmpurl );
   url = ServiceURL( tmpurl );
   getStringVector( data, scopeList );
   
   _DOORS_DEBUG(0, "decoded da url, host: " << url.getHost() );
   
   getStringVector( data, attributeList );
   getString( data, spi );
   data.getFirst(); // auth block count, should be zero
   // ignore following auth blocks
   
   return decoded_;
}


Message* SLPPeer :: DAADVERT :: encode (void)
{
   _DOORS_DEBUG(0, "DAADVERT::encode" );

   Udp6SAP :: Dtreq *dtr = new Udp6SAP :: Dtreq;
   Frame data;
   
   functionID = 8;
   encodeHeader( data );
   
   std::string tmpurl = url.getURL();
   _DOORS_DEBUG(0, "advertised url: " << tmpurl );
   put16( data, errorCode);
   put32( data, DABootStamp);
   
   putString( data, tmpurl );
   putStringVector( data, scopeList);
   putStringVector( data, attributeList);
   putString( data, spi);
   data.putLast(0); // auth block count, no data after that

   putLength( data );
   
   dtr->dest = dest; // set destination adddress
   dtr->data = data; // copy data frame to outgoing message
   
  return dtr;
} 


Uint16 SLPPeer :: SRVTYPERQST :: decode (Message *msg)
{
   decoded_ = 0;

   Udp6SAP :: Dtind *message = (Udp6SAP :: Dtind *)msg; 
   source = message->source;
   dest  = message->dest;
     
   Frame &data = message->data;

   decoded_ = decodeHeader( data );

   getAddrVector ( data, PRList);
   getString( data, namingA );
   getStringVector ( data, scopeList);
   
   return decoded_;
}

Message* SLPPeer :: SRVTYPERQST :: encode (void)
{
  Udp6SAP :: Dtreq *dtr = new Udp6SAP :: Dtreq;
  Frame data;
   functionID = 9;
   encodeHeader( data );

  putAddrVector( data, PRList);
  putString( data, namingA);
  putStringVector( data, scopeList);

  putLength( data );

  dtr->dest = dest; // set destination adddress
  dtr->data = data; // copy data frame to outgoing message
   
  return dtr;
} 

Uint16 SLPPeer :: SRVTYPERPLY :: decode (Message *msg)
{
   decoded_ = 0;

   Udp6SAP :: Dtind *message = (Udp6SAP :: Dtind *)msg; 
   source = message->source;
   dest  = message->dest;
     
   Frame &data = message->data;

   decoded_ = decodeHeader( data );

   errorCode = get16(data);
   getStringVector( data, typeList );
 
   return decoded_;
}

Message* SLPPeer :: SRVTYPERPLY :: encode (void)
{
  Udp6SAP :: Dtreq *dtr = new Udp6SAP :: Dtreq;
  Frame data;

   functionID = 10;
   encodeHeader( data );

  put16( data, errorCode);
  putStringVector( data, typeList);

  putLength(data);

  dtr->dest = dest; // set destination adddress
  dtr->data = data; // copy data frame to outgoing message
   
  return dtr;
} 

Uint16 SLPPeer :: SAADVERT :: decode (Message *msg)
{
   decoded_ = 0;

   Udp6SAP :: Dtind *message = (Udp6SAP :: Dtind *)msg; 
   source = message->source;
   dest  = message->dest;
     
   Frame &data = message->data;
   std::string tmpurl;
   decoded_ = decodeHeader( data );
   
   getString( data, tmpurl );
   getStringVector( data, scopeList );
   getStringVector( data, attributeList );
   data.getFirst(); // authblock count
   // ingnore auth blocks

   return decoded_;
}

Message* SLPPeer :: SAADVERT :: encode (void)
{
  Udp6SAP :: Dtreq *dtr = new Udp6SAP :: Dtreq;
  Frame data;
  std::string tmpurl=url.getURL();

   functionID = 11;
   encodeHeader( data );

  putString( data, tmpurl );
  putStringVector( data, scopeList );
  putStringVector( data, attributeList );
  data.putLast( 0 );

  putLength( data );

  dtr->dest = dest; // set destination adddress
  dtr->data = data; // copy data frame to outgoing message
   
  return dtr;
} 

Uint16 SLPPeer :: AAADVERT :: decode (Message *msg)
{
   decoded_ = 0;

   Udp6SAP :: Dtind *message = (Udp6SAP :: Dtind *)msg; 

   source = message->source;
   dest  = message->dest;
     
   Frame &data = message->data;

   std::string tmp_aa_url, tmp_da_url;
   decoded_ = decodeHeader( data );
   Uint8 prefixlength = data.getFirst();
   AAPrefix = data.split (prefixlength);
   Capabilities = data.getFirst();
   getString( data, tmp_aa_url );
   getString( data, tmp_da_url );

   aa_url = ServiceURL( tmp_aa_url );

   if( tmp_da_url.size() )
     da_url = ServiceURL( tmp_da_url );

   getStringVector( data, attributeList );
   data.getFirst(); // authblock count
   // ignore auth blocks

   return decoded_;
}

Message* SLPPeer :: AAADVERT :: encode (void)
{

  Udp6SAP :: Dtreq *dtr = new Udp6SAP :: Dtreq;

  Frame data;

  std::string tmp_aa_url = aa_url.getURL();
  std::string tmp_da_url = da_url.getURL();

  functionID = 12;
  encodeHeader( data );
  Uint8 prefixlength = AAPrefix.length();
  data.putLast (prefixlength);
  data.combine (AAPrefix);
  data.putLast (Capabilities);

  putString( data, tmp_aa_url );
  putString( data, tmp_da_url );
  putStringVector( data, attributeList );
  data.putLast( 0 );

  putLength( data );

  dtr->dest = dest; // set destination adddress
  dtr->data = data; // copy data frame to outgoing message
   
  return dtr;
} 

Message* SLPPeer :: ADDRRQST :: encode (void)
{
  Udp6SAP :: Dtreq *dtr = new Udp6SAP :: Dtreq;
  Frame data;
   functionID = 13;
   encodeHeader( data );

  putLength( data );

  dtr->dest = dest; // set destination adddress
  dtr->data = data; // copy data frame to outgoing message
   
  return dtr;
} 


Uint16 SLPPeer :: ADDRRQST :: decode (Message *msg)
{
   decoded_ = 0;

   Udp6SAP :: Dtind *message = (Udp6SAP :: Dtind *)msg; 
   source = message->source;
   dest  = message->dest;
     
   Frame &data = message->data;

   decoded_ = decodeHeader( data );

   return decoded_;
}

Message* SLPPeer :: ADDRRPLY :: encode (void)
{
  Udp6SAP :: Dtreq *dtr = new Udp6SAP :: Dtreq;
  Frame data;
  functionID = 14;
  encodeHeader( data );
  data.combine(PeerAddress);
  data.combine(NetPrefix);
  putLength( data );

  dtr->dest = dest; // set destination adddress
  dtr->data = data; // copy data frame to outgoing message
   
  return dtr;
} 

Uint16 SLPPeer :: ADDRRPLY :: decode (Message *msg)
{
   decoded_ = 0;

   Udp6SAP :: Dtind *message = (Udp6SAP :: Dtind *)msg; 
   source = message->source;
   dest  = message->dest;
     
   Frame &data = message->data;

   decoded_ = decodeHeader( data );

   PeerAddress = data.split(16);
   NetPrefix = data;
   return decoded_;
}
