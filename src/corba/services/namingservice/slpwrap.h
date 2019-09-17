
//  Copyright 2001
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

// Declarations for class SLPWrapper.
// Implementation in slpwrap.cc

#ifndef SLPWRAP_HH
#define SLPWRAP_HH

#include <string>
#include <vector>
#include <slp.h>

// SLP HANDLE FUNCTIONS

// opens a SLP connection with specified language tag
//bool SLPWOpen(std::string language, bool asyncronous);
bool SLPWOpen(std::string language);

// closes a SLP connection 
void SLPWClose();



// SERVICE REGISTRATION FUNCTIONS

// registers a service url to SLP with given lifetime and attributes
bool SLPWReg(std::string serviceurl, unsigned short lifetime, std::string attrs);

// deregisters a service url from SLP
bool SLPWDereg(std::string serviceurl);


// SERVICE LOCATION FUNCTIONS

// finds service url:s with given service type, scope and filter
bool SLPWFindSrvs(std::string servicetype, std::string scope, std::string filter, bool justone);

// finds what kind of services there are at given scope
bool SLPWFindSrvTypes(std::string namingauthority, std::string scopelist, bool justone);

// finds attributes of a given service url
bool SLPWFindAttrs(std::string serviceurl, std::string scope, std::string attrids, bool justone);


// CONFIGURATION FUNCTIONS

// finds scopes that can be used
std::string SLPWFindScopes();

// returns value of given SLP property name
std::string SLPWGetProperty(std::string propertyname);


// WRAPPER PROVIDED FUNCTIONS

// returns the result code of last SLP function call
// use this to get more info on failed wrapper calls
int SLPWError();

// prints some internal mumbo-jumbo result variable, results are from SLP calls
void SLPWPrintResults();

// returns a given entry of a internal result mumbo-jumbo variable
std::string SLPWGetResults(unsigned int pos);

// returns a CORBA IOR related to service
std::string FindIORwithSLP(std::string service);

// registers a CORBA IOR to service url
bool RegisterIORtoSLP(std::string url, unsigned short life, std::string ior);

// deregisters a CORBA IOR service url
bool DeregisterIORfromSLP(std::string url);

// find jooojoo
bool FindServiceHost( std::string service, std::string &host, int &port );


// CALLBACK FUNCTIONS

// Registering and de-registering callback, basically an error handler
void SLPWRegCallback( SLPHandle, SLPError error, void* cookie);

// URL fetcher callback, cookie has special use
SLPBoolean SLPWrapperSrvURLCallback( SLPHandle , const char* s,
				     unsigned short t, SLPError e,
				     void* cookie);

// attribute fetcher callback, cookie has special use 
SLPBoolean SLPWrapperAttrCallback(SLPHandle , const char* att, SLPError e, void* cookie);

// callback for fetching service types, cookie has special use
SLPBoolean SLPWSrvTypeCallback( SLPHandle, const char* srvtypes, SLPError e, void* cookie);

// cookie structure for passing parameters to callback functions
typedef struct {
  bool onlyfirst;
} CookieType;

#endif

