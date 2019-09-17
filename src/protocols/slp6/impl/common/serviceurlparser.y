%{

/*
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

$Log: serviceurlparser.y,v $
Revision 1.7  2004/06/17 08:49:47  hartmanj
Added support for transient service URL:s

Revision 1.6  2004/03/22 11:05:47  hartmanj
Added support for pure servicetype strings such as service:lpr without the URL. Especially helpful in attribute requests.

Revision 1.5  2004/02/26 09:45:02  hartmanj
Changed serviceurl.C to include serviceurlparser.h instead of serviceurlparser.C

Revision 1.4  2004/02/25 14:17:58  hartmanj
Replaced string::compare() with == operator

Revision 1.3  2004/02/25 13:33:02  hartmanj
Repositioned license clause, added log


*/

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "serviceurlparser.h"


void yyerror(char *s);
int yylex(void);

DOORS_ServiceURL_Parser_Class DOORS_ServiceURL_Parser;

%}

%union {
     const char* strval;
     struct DOORS_ServiceURL_Parser_Class::Span span;
     char chval;
}

%token STRING HEXSTRING NUM IPSTART IPXSTART ATSTART SERVICESTR TRANSKEY
%type <span> STRING
%type <span> HEXSTRING
%type <span> NUM
%type <span> IPSTART
%type <span> IPXSTART
%type <span> ATSTART
%type <span> SERVICESTR
%type <span> servicetype
%type <span> hex
%type <span> user
%type <span> serviceurl
%type <span> sap
%type <span> ipsite
%type <span> site
%type <span> urlpart
%type <span> urlpath
%type <span> hostport
%type <span> host
%type <span> hostname
%type <span> domainlabel
%type <span> toplabel
%type <span> hostnumber
%type <span> ipv6address
%type <span> hexlist
%type <span> atsite
%type <span> attype
%type <span> atobject
%type <span> atzone
%type <span> ipxsite
%type <span> ipxnet
%type <span> ipxnode
%type <span> ipxsocket
%type <span> attrval

%%

serviceurl: SERVICESTR servicetype ':' sap attrlist
		{
			DOORS_ServiceURL_Parser.setAddrSpec($4);
		}
	;

serviceurl: SERVICESTR servicetype
		{
			DOORS_ServiceURL_Parser.setPureType();
		}
	;

servicetype: STRING
		{
			DOORS_ServiceURL_Parser.setConcreteType($1);
			$$ = $1;
		}
	 | servicetype ':' STRING
		{
			DOORS_ServiceURL_Parser.setAbstractType($1);
			DOORS_ServiceURL_Parser.setConcreteType($3);
			$$.a = $1.a;
			$$.b = $3.b; 
		}
	;

sap: site | site urlpart
		{
			$$.a = $1.a;
			$$.b = $2.b; 
		}
	;

site: ipsite | atsite | ipxsite
	;

ipsite: IPSTART hostport
		{
			$$.a = $1.a;
			$$.b = $2.b; 
		}
	| IPSTART user '@' hostport
		{
			$$.a = $1.a;
			$$.b = $4.b; 
		}
	;

user: STRING
	;

urlpart: '/' urlpath { $$.a = $2.a - 1; $$.b = $2.b; }
	;

urlpath:STRING
	| NUM
	| urlpath '/' STRING { $$.a = $1.a; $$.b = $3.b; }
	| urlpath '/' NUM { $$.a = $1.a; $$.b = $3.b; }
 	| HEXSTRING
	| urlpath '/' HEXSTRING { $$.a = $1.a; $$.b = $3.b; }
	;

hostport: host | host ':' NUM
		{
			$$.a = $1.a;
			$$.b = $3.b;
                        		DOORS_ServiceURL_Parser.setPort($3);
		}
	;

host: hostname { DOORS_ServiceURL_Parser.setHostAddr($1); }
	| hostnumber { DOORS_ServiceURL_Parser.setHostAddr($1); }
	| TRANSKEY { DOORS_ServiceURL_Parser.setTransient(); }
	;

hostname: toplabel
      	| domainlabel '.' toplabel { $$.a = $1.a; $$.b = $3.b; }
	;

toplabel: STRING
	;

domainlabel: STRING
	| domainlabel '.' STRING { $$.a = $1.a; $$.b = $3.b; }
	;

hostnumber: NUM '.' NUM '.' NUM '.' NUM
	{	
		$$.a = $1.a;
		$$.b = $7.b;
	}
	  | ipv6address
;

ipv6address: '[' hexlist ']' { $$.a = $2.a; $$.b = $2.b; }
	;

hexlist: hexlist ':' hex { $$.a = $1.a; $$.b = $3.b; }
 	| hexlist ':' ':' hex { $$.a = $1.a; $$.b = $4.b; } | hex
	;

hex: HEXSTRING
	| NUM
	;

atsite: ATSTART atobject ':' attype ' ' atzone
		{
			$$.a = $1.a;
			$$.b = $6.b;
		}
	;

atobject: STRING
	;

attype: STRING
	;

atzone: STRING
	;

ipxsite: IPXSTART ipxnet ':' ipxnode ':' ipxsocket
		{
			$$.a = $1.a;
			$$.b = $6.b;
		}
	;

ipxnet: HEXSTRING | NUM
	;

ipxnode: HEXSTRING | NUM
	;

ipxsocket: HEXSTRING | NUM
	;

attrlist: /* empty */ | attrlist ';' attr
	;

attr: attrval '=' attrval
		{
			DOORS_ServiceURL_Parser.addAttribute($1,$3);
 		}
	;

attrval: STRING | NUM | HEXSTRING
	;
%%

int yylex(void)
{
     return DOORS_ServiceURL_Parser.yylex();
}

DOORS_ServiceURL_Parser_Class::DOORS_ServiceURL_Parser_Class()
     : url(""),
       pos(0), port_(-1), serviceIsPureType_(false)
{
}

DOORS_ServiceURL_Parser_Class::~DOORS_ServiceURL_Parser_Class()
{
     attributes_.clear();
}

void DOORS_ServiceURL_Parser_Class::addAttribute(struct DOORS_ServiceURL_Parser_Class::Span a, struct DOORS_ServiceURL_Parser_Class::Span b)
{
     std::string type, value;

     type = url.substr(a.a, a.b-a.a);
     value = url.substr(b.a, b.b-a.a);

     attributes_.insert(std::map<std::string, std::string>::value_type
                        (type, value));
}

void DOORS_ServiceURL_Parser_Class::setAddrSpec(struct DOORS_ServiceURL_Parser_Class::Span a)
{
     address_ = url.substr(a.a, a.b-a.a);
}

void DOORS_ServiceURL_Parser_Class::setConcreteType(struct DOORS_ServiceURL_Parser_Class::Span a)
{
     access_ = url.substr(a.a, a.b-a.a);
}

void DOORS_ServiceURL_Parser_Class::setAbstractType(struct DOORS_ServiceURL_Parser_Class::Span a)
{
     service_ = url.substr(a.a, a.b-a.a);
}

void DOORS_ServiceURL_Parser_Class::setPureType( )
{
    serviceIsPureType_ = true;
}

std::string DOORS_ServiceURL_Parser_Class::getAbstractType(void)
{
     return service_;
}

void DOORS_ServiceURL_Parser_Class::setHostAddr(struct DOORS_ServiceURL_Parser_Class::Span a)
{
     host_ = url.substr(a.a, a.b-a.a);
}

std::string DOORS_ServiceURL_Parser_Class::getHostAddr(void)
{
     return host_;
}

int DOORS_ServiceURL_Parser_Class::getPort(void)
{
     return port_;
}

void DOORS_ServiceURL_Parser_Class::setPort(struct DOORS_ServiceURL_Parser_Class::Span a)
{
     port_ = strtol(url.substr(a.a, a.b-a.a).c_str(),0,10);
}

std::string DOORS_ServiceURL_Parser_Class::getConcreteType(void)
{
     return access_;
}

std::string DOORS_ServiceURL_Parser_Class::getAddrSpec(void)
{
     return address_;
}

std::map<std::string,std::string> DOORS_ServiceURL_Parser_Class::getAllAttributes()
{
     return attributes_;
}

void DOORS_ServiceURL_Parser_Class::setURL(std::string URLToParse)
{
     url = URLToParse;
     pos = 0;
     isTransient = false;
     serviceIsPureType_ = false;
     attributes_.clear();
     service_ = access_ = address_ = host_ = "";
     port_ = 0;
}

void DOORS_ServiceURL_Parser_Class :: setTransient() {
    isTransient = true;
}

bool DOORS_ServiceURL_Parser_Class :: getTransient() {
    return isTransient;
}

int DOORS_ServiceURL_Parser_Class::yylex()
{
     if( pos >= url.size() )
          return 0;
     if( pos == 0 && url.size()>8 && url.substr(0, 8)==std::string("service:") ) {
          pos+=8;
          yylval.span.a = 0;
          yylval.span.b = pos;
          return SERVICESTR;
     }

     if( pos > 0 && url.size() >= pos + 4 && url.substr(pos,5).compare("[coa]") == 0 ) {
         yylval.span.a = pos;
         yylval.span.b = pos+5;
         pos += 5;
         return TRANSKEY;
     }

     yylval.span.a = pos;
     if( isalpha(url.at(pos)) ) {
          bool ishex = true;
          while(pos < url.size() && (isalpha(url.at(pos))||isdigit(url.at(pos))||url.at(pos)=='-'||url.at(pos)=='_') ) {
               if(ishex&&(!isxdigit(url.at(pos))))
                    ishex = false;
               pos++;
          }
          yylval.span.b = pos;
          if( ishex )
               return HEXSTRING;
          return STRING;
     } else if( isdigit(url.at(pos)) || (pos<url.size()-1 && url.at(pos) == '-' && isdigit(url.at(pos+1))) ) {
          if(url.at(pos) == '-') {
               pos++;
          }
          bool ispuredigit = true;
          while(pos < url.size() && isxdigit(url.at(pos))) {
               if(ispuredigit && !isdigit(url.at(pos)))
                    ispuredigit = false;
               pos++;
          }
          yylval.span.b = pos;
          if(ispuredigit)
               return NUM;
          return HEXSTRING;
     } else if( url.size() > pos+1 
                && url.at(pos) == '/'
                && url.at(pos+1) == '/' ) {
          pos+=2;
          yylval.span.b = pos;
          return IPSTART;
     } else if( url.size() > pos+3 
                && url.at(pos) == '/' && url.at(pos+1) == 'a' && url.at(pos+2) == 't'
                && url.at(pos+3) == '/' ) {
          pos+=4;
          yylval.span.b = pos;
          return ATSTART;
     } else if( url.size() > pos+4 
                && url.at(pos) == '/' && url.at(pos+1) == 'i' && url.at(pos+2) == 'p'
                && url.at(pos+3) == 'x' && url.at(pos+4) == '/' ) {
          pos+=5;
          yylval.span.b = pos;
          return IPXSTART;
     } else {
          yylval.chval = url.at(pos);
          pos++;
          return yylval.chval;
     }

     // eof
     return 0;
}

void
yyerror (char *s)  /* Called by yyparse on error */
{
  std::cout << "  ERROR: " << s << std::endl;
}
