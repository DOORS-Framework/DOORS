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

$Log: slpheader.C,v $
Revision 1.13  2004/04/14 09:11:42  hartmanj
Port of PRList address in ATTRRQST from 427 to 0. Helps matching later.

Revision 1.12  2004/04/07 13:43:34  hartmanj
slpheader.h steps in

Revision 1.11  2004/03/25 09:11:19  bilhanan
copy constructor: fixed string copy function to avoid sun c++ crashes.

Revision 1.10  2004/02/26 11:03:53  hartmanj
Cleaning

Revision 1.9  2004/02/10 11:17:36  hartmanj
Removed redundant ntohx and htonx and added some more _DOORS_DEBUGs.

Revision 1.8  2004/02/10 08:19:59  hartmanj
Bug fixing and rewriting

Revision 1.7  2004/02/03 14:43:41  bilhanan
printFrame() removed.

Revision 1.6  2004/02/03 14:30:20  bilhanan
network byte ordering fix.

Revision 1.5  2004/02/02 09:55:54  hartmanj
Added some error checking and debug msgs

Revision 1.4  2004/02/01 22:51:23  bilhanan
introduced signedness into lifetime variable.

Revision 1.3  2003/06/04 11:09:55  kalliosa
Licence clause added

  
*/

#include <assert.h>
#include <arpa/inet.h>
#include <doors/frame.h> 
#include <doors/slpheader.h>
#include <doors/serviceurl.h>

#include <doors/debug.h>


//constructor
SLPHeader :: SLPHeader ()
  : decoded_( 0), version(2), functionID(0), length(0), overflow(false),
requestMcast( false ), fresh(false), XID(0), slpHandle(0),langTag("en"),
dest(), source()
{
}

// copy constructor 
SLPHeader :: SLPHeader ( const SLPHeader &src )
  : decoded_(src.decoded_), version(src.version), functionID(src.functionID),
length(src.length), overflow(src.overflow), requestMcast( src.requestMcast ),
fresh(src.fresh), XID(src.XID), slpHandle(src.slpHandle),
dest(src.dest), source(src.source)
{
  if (!src.langTag.empty())
    langTag = src.langTag;
}

//destructor
SLPHeader :: ~SLPHeader()
{
   // nothing to clean
}

void SLPHeader ::  putLength( Frame &data )
{
   length = data.length();
   Uint32 n = length;

   data.write( (n >> 16) & 0x0FF, 2 );  // put MSB
   data.write( (n >> 8) & 0x0FF,3 );
   data.write( n & 0x0FF,4 );  // put LSB
}

// gets a comma separated list string and returns it as an address vector
void SLPHeader :: getAddrVector( Frame &data, AddrVector &vc )
{
     _DOORS_DEBUG(0,"SLPHeader :: getAddrVector()");
     StringVector tmp;
   getStringVector( data, tmp );
   
   // TODO: check this.. should []'s be removed
   _DOORS_DEBUG(0, "converting strvector of size " << tmp.size() );
   
   if( tmp.size() != 0 )
   {
        for( StringVector::iterator i = tmp.begin(); i != tmp.end(); ++i)
        {
             _DOORS_DEBUG(0, "decoding addr: '" << *i<< "'"  );
             std::string tempAddrStr = (*i);
             unsigned int idx1,idx2;
             idx1 = tempAddrStr.find("[");
             idx2 = tempAddrStr.find("]");
             if( idx1 != std::string::npos && idx2 != std::string::npos) {
                  tempAddrStr = tempAddrStr.substr( tempAddrStr.find("[" ) +1, tempAddrStr.find("]") -1 );
             } else {
                  tempAddrStr = "0::0"; 
             }
             
             InetAddr6 addr;
             try
             {
                  // port is ignored 
                  addr.set( 0,tempAddrStr );
                  vc.insert(  addr );
             }
             catch ( ... )
             {
             }
             
        }   
   }
}



/*
InetAddr6  SLPHeader :: getAddr( const std::string &s )
{
      return InetAddr6( tmp );
}
_*/
/*
void SLPHeader :: putAddr( Frame &data, InetAddr &a )
{

/*
 * no port information needed
 PRString += ":" + a.getPortNumber();
   _DOORS_DEBUG(0, "Putting addr to pdu: " << PRString );
     
}
*/
// puts an address vector to a frame as a comma separated list string
void SLPHeader :: putAddrVector( Frame &data, AddrVector &vc )
{

   std::string PRString;
   
   for( AddrVector::iterator i = vc.begin(); i != vc.end(); ++i)
   {
      std::string tmp = (*i).getHostAddr();
      _DOORS_DEBUG(0, "gethostaddr: " << tmp );
      PRString += ("[" + tmp + "]" );
      PRString += ",";
   }
   // remove last comma
   if( PRString.size() > 0)
   {
      PRString.erase( PRString.size() -1 );
   }
   
   putString(data, PRString );
	 
}

// gets URL entry blocks and returns them as a vector

void SLPHeader :: getServiceURLVector( Frame &data, URLVector &vc )
{
   Uint16  count = get16(data); // how many urls
   for( Uint16 i = 0; i < count; ++i ) // read all urls to vector
   {
      vc.push_back( getUrlEntry( data ) );
   }
}

// puts a vector of URLs as separate entry blocks

void SLPHeader :: putServiceURLVector( Frame &data, URLVector &URLs )
{
  put16( data, URLs.size()); // put url block count
  
  for ( Uint16 i = 0; i < URLs.size() ; i++ ) // go thru all url blocks
    {
      putUrlEntry( data, URLs[i] ); // put a block to a frame
    }

}

// gets one URL entry block from a frame

ServiceURL SLPHeader :: getUrlEntry( Frame &data )
{
   Uint16 dummy = data.getFirst( ); // reserved, not used
   
   
   Sint16 lifetime = get16(data);
   std::string urlstr;
   getString( data, urlstr ); // reads length and url-string
   _DOORS_DEBUG(0, "SLPHeader :: getUrlEntry - urlstr == " << urlstr);
   ServiceURL URL( urlstr, lifetime );
   Uint8 authCount = data.getFirst ();
   _DOORS_DEBUG(0, "SLPHeader :: getUrlEntry - authCount == " << (int)authCount);

   // rest is actually ignored. Just reading through everything
   for( Uint32 i = 0; i < authCount; ++i )
   {
      _DOORS_DEBUG(0, "SLPHeader :: getUrlEntry - authLOOP!");
      Byte dummy = data.getFirst(); // read bsd
      dummy = data.getFirst();   
      Uint16 auBlockLength = get16(data);
      auBlockLength -= 2; // bsd was already read
      
      std::string str = ""; 
      // read auth block
      str = (char* )data.getFirst( auBlockLength );
   }

   return URL;
}

// puts one URL entry block to a frame

void SLPHeader :: putUrlEntry( Frame &data, ServiceURL &service )
{

  data.putLast( 0 ); // put the "reserved" byte
  put16( data, service.getLifetime() ); // put 16 bit lifetime
  putString( data, service.getURL() ); // put 16 bit length and the url string
  data.putLast( 0 ); // put authblock count, no auth blocks

}

// gets a comma separated string list from a frame and returns a StringVector

void SLPHeader :: getStringVector( Frame &data, StringVector &vc )
{
     _DOORS_DEBUG(0,"SLPHeader :: getStringVector()");
   std::string tmp;
  
   getString( data, tmp);
   _DOORS_DEBUG(0,"  tmp = \"" << tmp << "\"");
   strlist2vec( tmp, vc);

}

// puts a StringVector in a frame as a comma separated list string

void SLPHeader :: putStringVector( Frame &data, StringVector &vc )
{

  std::string tmp;

  vec2strlist( vc, tmp);
  putString( data, tmp);

}

// gets a 16-bit string length and the next length chars to a string 

void  SLPHeader :: getString( Frame &data, std::string &str )//  Uint32 length )
{
   Uint16 strLength = get16(data);
   _DOORS_DEBUG(0,"SLPHeader :: getString() - of length " << strLength);
   
   char *strBuf_ = 0;
   
   if (strLength != 0)
   {
        strBuf_ =  new char[strLength+1];
        for( Uint32 i = 0; i < strLength; ++i )
        {
             strBuf_[i] = data.getFirst();
        }
        strBuf_[strLength] = 0;
        str = std::string(strBuf_);
        delete [] strBuf_;
   }
   else
   {
        str = "";
   }

}

// puts a 16-bit string length and length chars to a frame

void SLPHeader :: putString ( Frame &data, std::string str )
{
   put16( data, str.length()); // put string length into a frame
  
   if (str.length() != 0 ) // if not an empty string
   {
      // put the string also into a frame
      data.putLast( (const Byte *) str.c_str(), str.length() );
   }
   
}

// returns true if given bit on value is 1
bool SLPHeader :: getFlag( Uint16 flags, Uint16 n )
{   
   if ( ( ( 1 << n ) & flags ) > 0 )
     return true;
   else 
     return false;
}

// sets a bit n in flags to a given state

void SLPHeader :: setFlag( Uint16 &flags, Uint16 n, bool state)
{
  flags|=(1<<n); // set bit
  if (!state) flags^=(1<<n); // clear if we have to
}
// decodes a header from a pdu frame

Uint16 SLPHeader :: decodeHeader( Frame &data )
{
     _DOORS_DEBUG(0,"SLPHeader :: decodeheader()");
   Uint32 count; // for header fields that are ignored or not stored

   //printFrame( data );
   
   version = data.getFirst();
   functionID = data.getFirst(); // propably not needed at this point

   length = get24(data);
   
   _DOORS_DEBUG(0, "decoding length: " << (int)length);

   Byte flags = data.getFirst();
   assert(data.getFirst() == 0); // reserved bits MUST be 0
   assert((flags & 0x1f) == 0);
   
   if( flags & 0x80 )
     overflow = true;
   
   if( flags & 0x40 )
     fresh = true;
   
   if( flags & 0x20 )
     requestMcast = true;
   
   count = get24(data); // next extension offset, ignored, not implemented
   XID = get16(data);

   langTag = "";
   getString( data, langTag );
   _DOORS_DEBUG(0, "SLPHeader :: decodeheader() - langTag == " << langTag);
   return 14 + count ; // number of decoded bytes
}

Uint32 SLPHeader :: getRqstLength()
{
   return 14 + langTag.length();
}

// adds a header to a pdu frame
void SLPHeader :: encodeHeader( Frame &data )
{
   data.destroy(); // clear frame

   data.putLast(version);
   
   data.putLast(functionID);
   length = 0; // length will be updated later!!
   put24(data, length);
  
   // set flags
   Uint16 flags = 0;
   if( overflow )
     flags = flags | ( 1 << 15 );
   if( fresh )
     flags = flags | ( 1 << 14 );
   if( requestMcast )
     flags = flags | ( 1 << 13 );
   
   put16( data, flags); 

   put24( data, 0 ); // next extension offset, null because not supported
   put16( data, XID);
   putString( data, langTag);
   _DOORS_DEBUG(0, "header encoded." );
}

// gets given amount of bytes from frame, but max value is 32 bits

Uint32 SLPHeader :: getn( Frame &data, Uint16 n  )
{
   Uint32 rv = 0;
   // get n octets MSB first
   for( int i = 0; i < n; ++i)
   {
      rv = ( rv << (n*8) );   
      rv = rv | ( data.getFirst() );
   }   
   return rv;
}

// gets a 16 bit value from frame

Uint16 SLPHeader :: get16( Frame &data )
{
   
   Uint16 rv = data.getFirst(); // MSB
   rv = rv << 8;
   rv = rv | data.getFirst(); // LSB
   return rv;
}

// gets a 24 bit value from frame

Uint32 SLPHeader :: get24( Frame &data )
{
   
   Uint32 rv = data.getFirst(); // MSB
   rv = rv << 8;
   rv = rv | data.getFirst(); 
   rv = rv << 8;
   rv = rv | data.getFirst(); // LSB
   return rv;
}

// gets a 32 bit value from frame

Uint32 SLPHeader :: get32( Frame &data )
{
   Uint32 rv = data.getFirst(); // MSB
   rv = rv << 8;
   rv = rv | data.getFirst(); 
   rv = rv << 8;
   rv = rv | data.getFirst(); 
   rv = rv << 8;
   rv = rv | data.getFirst(); // LSB
   return rv;
}

// puts a 16 bit value to end of frame

void SLPHeader :: put16 ( Frame &data, Uint16 n )
{
  data.putLast( (n >> 8) & 0x0FF ); // put MSB
  data.putLast( n & 0x0FF );  // put LSB 
}

// puts a 24 bit value to end of frame

void SLPHeader :: put24 ( Frame &data, Uint32 n )
{
  data.putLast( (n >> 16) & 0x0FF );  // put MSB
  data.putLast( (n >> 8) & 0x0FF );
  data.putLast( n & 0x0FF );  // put LSB 
}

// puts a 32 bit value to end of frame

void SLPHeader :: put32 ( Frame &data, Uint32 n )
{
  data.putLast( (n >> 24) & 0x0FF ); // put MSB
  data.putLast( (n >> 16) & 0x0FF );  
  data.putLast( (n >> 8) & 0x0FF );
  data.putLast( n & 0x0FF );  // put LSB 
}

// converts a comma separated list from string to a vector

bool SLPHeader :: strlist2vec( std::string &st, StringVector &vc)
{
     _DOORS_DEBUG(0, "SLPHeader :: strlist2vec() - string st == " << st );
   if( st.length() == 0 )
     return false;
   
   vc.clear(); // clear passed vector
   
   
   int  commapos = 0; // store list delimiter position here
   
   while ( (commapos = st.find(",",0)) != -1 ) // find first delimiter
   {
      //      std::cout << st << " _ " <<  commapos << std::endl;
 
      vc.push_back(st.substr(0,commapos) ); // store from beginning, NO delim
      
      
      //      std::cout << st.substr(0,commapos-1) << std::endl;
 
      st=st.erase(0,commapos+1); // delete beginning AND delim
   }
   if( st != "" )
   {
      _DOORS_DEBUG(0, "pushing back string '" << st << "'" );
      vc.push_back(st); // last or only string
   }
   
   return true; 
}

// converts a vector list to a comma separated string list

bool SLPHeader :: vec2strlist( StringVector &vc, std::string &st)
{
  st="";

  Uint32 pos = 0;

  if (vc.size() > 0 ) // if not empty
    {
      while ( pos < (vc.size()-1) ) // go thru all except last
	{
	  st=st+vc[pos]+','; // add comma delimiters in between
	  pos++;
	}
      st=st+vc[pos]; // add last after comma, no comma after last
    }
//   _DOORS_DEBUG(0, "converted stringlist: '" << st << "'" );
return true;
}

// prints all entries from a vector to screen

void SLPHeader :: printstrvec( StringVector vc )
{
  Uint32 pos = 0; // store string pos here

  if (vc.size() > 0) // if not empty
    {
      while (pos < vc.size() ) // go through all strings
        {
          std::cout << vc[pos] << '!'; // print it
          pos++;                  // move to next
        }
//      std::cout << vc[pos] ;
    }
  //  else
//    std::cout << "!";
  std::cout << "!"; // after all is done, write termination
  std::cout << std::endl; // and change line
}

