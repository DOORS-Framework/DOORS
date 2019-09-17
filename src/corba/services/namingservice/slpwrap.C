
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

// Implementation for class SLPWrapper
// Declarations are in slpwrap.hh

#include <string>
#include <vector>
#include <doors/slpwrap.h>

//#include <cstdio>

static std::vector<std::string> SLPWresults; // result strings are kept here
static std::vector<int> SLPWresulti; // result integers are kept here ( timeouts )

// vector<int> SLPWblah; // hmm, just some test, i don't remember :)

static SLPHandle slph; // handle for slp operations
static SLPError slpe; // result for slp operations



// bool SLPWOpen(string language,bool asyncronous)

bool SLPWOpen(std::string language)
{

  const char* lang=language.c_str(); // convert language tag to c_string
  SLPBoolean async; 
 
  bool asyncronous = false;  // use slp syncronously
 
  if (asyncronous == true) 
    {
      async = SLP_TRUE;
    }
  else
    {
      async = SLP_FALSE;
    }
  
  slpe = SLPOpen(lang,async,&slph);  // open slp connection
   
  return (slpe == SLP_OK); // return result of operation
}

void SLPWClose()
{
  SLPClose(slph); // close opened slp connection
}





void SLPWRegCallback( SLPHandle, SLPError error, void* cookie)
{
  *(SLPError*)cookie = error; // just return the error code
}

bool SLPWReg( std::string serviceurl, unsigned short lifetime, std::string attrs)
{

  SLPError cbe;

  const char* servicec = serviceurl.c_str(); // convert strings to c_strings
  const char* attrc = attrs.c_str();
  
  slpe = SLPReg( slph, servicec, lifetime, "default", attrc, SLP_TRUE,
		 SLPWRegCallback, &cbe);  // register to slp

  //  cout << slpe << " " << cbe << endl;

  return ( (slpe == SLP_OK) && (cbe == SLP_OK) ); // return result of operation

}

bool SLPWDereg( std::string serviceurl)
{

  SLPError cbe;

  const char* servicec = serviceurl.c_str();
  
  slpe = SLPDereg( slph, servicec, SLPWRegCallback, &cbe);  // deregister from slp

  return ( (slpe == SLP_OK) && (cbe == SLP_OK) ); // return result of operation

}








SLPBoolean SLPWrapperSrvURLCallback( SLPHandle , const char* s,
				     unsigned short t, SLPError e,
				     void* cookie)
{

  //  if (h);
//  if (c);

  CookieType *idiootti;

  idiootti = static_cast <CookieType*>(cookie); // some mysterious cast 

  if (e == SLP_OK) // if this is a valid call
    {

      //      cout << "url_callback" << endl;

      //      SLPWrapper::results.push_back(s);
      //SLPWrapper::resulti.push_back(t);
      
      SLPWresults.push_back(s); // save url
      SLPWresulti.push_back(t); // save timeout
      


      if ( idiootti->onlyfirst == true ) // do we want more or just first
	{
	  return SLP_FALSE;
	} 
      else
	{
	  return SLP_TRUE;
	}

    }
  
  return SLP_FALSE; // not valid call or last call
}





bool SLPWFindSrvs(std::string servicetype, std::string scope, std::string filter, bool justone)
{  

  CookieType Parms;
  Parms.onlyfirst=justone;
  
  SLPWresults.clear(); // clear result variables
  SLPWresulti.clear();
  
  //SLPWrapper::results.clear();
  //SLPWrapper::resulti.clear();
  
  const char* servicec = servicetype.c_str(); // string conversions
  const char* scopec = scope.c_str();
  const char* filterc = filter.c_str();
  
  //  SLPBoolean (*testptr)( SLPHandle, const char *, unsigned short, SLPError, void*); 
  //testptr = &SLPWrapper::SLPWrapperSrvURLCallback;
  //slpe = SLPFindSrvs( slph, servicec, scopec, filterc, 
  
  // call slp

  slpe = SLPFindSrvs( slph, servicec, scopec, filterc, 
		      SLPWrapperSrvURLCallback, &Parms );
  
  return (slpe == SLP_OK); // return result of operation
  
}






SLPBoolean SLPWrapperAttrCallback(SLPHandle , const char* att, SLPError e, void* cookie)
{

  //  if (h);
  //if (c);
  
  CookieType *idiootti;

  idiootti = static_cast <CookieType*>(cookie);  // more weird casting

  if ( e == SLP_OK ) // a valid result call
    {

      SLPWresults.push_back(att); // save result

      //      std::cout << "attr_callback" << std::endl;
      
      if ( idiootti->onlyfirst == true ) // do we want more or just the first
	{
	  return SLP_FALSE;
	}
      else
	{
	  return SLP_TRUE;
	} 

    }

  return SLP_FALSE; // not valid or last call
}

bool SLPWFindAttrs(std::string serviceurl, std::string scopelist, std::string attrids, bool justone)
{

  CookieType Parms;
  Parms.onlyfirst = justone;

  SLPWresults.clear(); // clear previous results
  SLPWresulti.clear();
    
  const char* serviceurlc = serviceurl.c_str(); // string conversions
  const char* scopec = scopelist.c_str();
  const char* attridsc = attrids.c_str();

  // do the magic slp call

  slpe = SLPFindAttrs(slph, serviceurlc, attridsc, scopec, SLPWrapperAttrCallback, &Parms);

  return (slpe == SLP_OK); // return result of operation

}


SLPBoolean SLPWSrvTypeCallback( SLPHandle, const char* srvtypes, SLPError e, void* cookie)
{

  CookieType *idiootti;

  idiootti = static_cast <CookieType*>(cookie);  // more weird casting

  if ( e == SLP_OK ) // a valid result call
    {
      SLPWresults.push_back(srvtypes); // save result

      //      cout << "srvtype_callback" << endl;
      
      if ( idiootti->onlyfirst == true ) // do we want more or just the first
	{
	  return SLP_FALSE;
	}
      else
	{
	  return SLP_TRUE;
	} 
    }

  return SLP_FALSE; // not valid or last call

}

bool SLPWFindSrvTypes(std::string namingauthority, std::string scopelist, bool justone)
{
  
  CookieType Parms;
  Parms.onlyfirst = justone;
  
  SLPWresults.clear(); // clear previous results
  SLPWresulti.clear();
  
  const char*  na = namingauthority.c_str(); 
  const char*  sc = scopelist.c_str();

  slpe = SLPFindSrvTypes(slph, na, sc, SLPWSrvTypeCallback, &Parms );

  return ( slpe == SLP_OK );
}


std::string SLPWFindScopes()
{

  std::string tmps;
  char* ls ;
  
  slpe = SLPFindScopes( slph, &ls);

  //  if (slpe != SLP_OK)
  //    {
  //      std::cout << "!!!" << slpe << std::endl;
  //    }

  //ls = lista;
  //  cout << ls << endl;

  tmps=ls;

  SLPFree( &ls );  //??? OR 
  //  SLPFree( ls );

  return tmps;

}


std::string SLPWGetProperty( std::string propertyname)
{

  std::string tmps = "";

  const char*  na = propertyname.c_str(); 

  const char* res;

  res = SLPGetProperty( na );

  if ( res != NULL )
    {
      tmps = res;
    }

  return tmps;

}

int SLPWError()
{
  return slpe;
}

void SLPWPrintResults()
{
  
  //  SLPWblah.clear();
  
  if( SLPWresulti.size() > 0)
    {
      
      int i = SLPWresults.size();
      
      if (i > 0 )
	{
	  int j = 0;
	  while (j<i)
	    {
	      //	      std::cout << SLPWresults[j] << " " << SLPWresulti[j] << std::endl;
	      ++j;
	    }
	}
    }
  else
    {
      int i = SLPWresults.size();
      
      if (i > 0 )
	{
	  int j = 0;
	  while (j<i)
	    {
	      //	      std::cout << SLPWresults[j] << std::endl;
	      ++j;
	    }
	}
    }
}

std::string SLPWGetResults(unsigned int pos)
{
  if (pos < SLPWresults.size())
    return SLPWresults[pos];    // return result number #

  return "";
}


std::string FindIORwithSLP(std::string service)
{
  
  
  std::string url = "";
  std::string ior = "";
  std::string atr = "";
  
  //  std::cout << "Opening SLP" << std::endl;
  

  if (SLPWOpen("default"))  // open slp connection with language tag
    {
      //      std::cout << "Opened" << std::endl;
      //      std::cout << "Finding service" << std::endl;


      // find given service with given scope and filter
      // and return immediately with the first result

      if (SLPWFindSrvs(service,"default","",true)) 
	{

	  SLPWPrintResults();
	  
	  url = SLPWGetResults(0); // take first result

	  //	  std::cout << url << std::endl;
	  //	  std::cout << "Finding attributes" << std::endl;
	  

	  // find attributes of first url with given filter & scope
	  // returns after getting the first attribute
	  
	  if (SLPWFindAttrs(url, "", "default",true))
	    {
	      
	      SLPWPrintResults();
	      
	      atr = SLPWGetResults(0); // get first attribute

	      //	      std::cout << atr << std::endl;

	      int len = atr.length();	    
	      
	      // parse IOR from IOR attribute "(IOR=IOR:xxx)" -> "IOR:xxx"

	      if (len>0) {
		
		int pos = atr.find("IOR:");
		if (pos > 0 )
		  {
		    
		    while( pos < (len-1) )
		      {
			
			ior = ior + atr[pos];
			++ pos;
		      }
		    
		  }
	      }
	      
	      //	      std::cout << ior << std::endl;
	      
	    }
	  else
	    {
	      //cout << "FindAttrs failed" << endl;
	    }
	  
	}
      else
	{
	  //cout << "FindSrvs failed" << endl;
	}
      
      SLPWClose();
    }
  else
    {
      //cout << "Open error" << endl;
    }


  return ior; // return with found IOR or empty string


}

bool RegisterIORtoSLP(std::string url, unsigned short life, std::string ior)
{

  bool res=true; // success or not?


  //  cout << "opening" << endl;
  //  if (SLPWOpen("default",false))
 
  if (SLPWOpen("default"))  // open slp connection with given language tag
    {
      //cout << "open ok" << endl;
      //cout << "registering" << endl;
      
      std::string atts = "(IOR="+ior+")"; // make given IOR to a SLP attribute
      
      if (SLPWReg(url,life,atts)) // register given service url&lifetime&iorattr to slp
	{
	  //cout << "register ok" << endl;
	}
      else
	{
	  // cout << "register error" << endl;
	  res=false;
	}

      SLPWClose();

      return res;

    }
  else
    {
      //cout << "open error" << endl;
      return false; // open error
    }

  return true;

}

bool DeregisterIORfromSLP(std::string url)
{

  bool res=true;

  //  cout << "opening" << endl;
  
  //  if (SLPWOpen("default",false))


  if (SLPWOpen("default")) // open slp connection with language tag
    {
      //      cout << "open ok" << endl;
      //cout << "deregistering" << endl;
      
      if (SLPWDereg(url)) // deregister with given URL
	{
	  //	  cout << "deregister ok" << endl;
	}
      else
	{
	  //	  cout << "deregister error" << endl;
	  res=false;
	}

      SLPWClose();

      return res;

    }
  else
    {
      //      cout << "open error" << endl;
      return false;
    }

  return true;

}



bool FindServiceHost( std::string service, std::string &host, int &port )
{

  std::string url = "";
  host = "";
  port = 0;

  //  std::cout << "Opening SLP" << std::endl;
  

  if (SLPWOpen("default"))  // open slp connection with language tag
    {
      //      std::cout << "Opened" << std::endl;
      //      std::cout << "Finding service" << std::endl;

      // find given service with given scope and filter
      // and return immediately with the first result

      if (SLPWFindSrvs(service,"default","",true)) 
	{

	  SLPWPrintResults();
	  
	  url = SLPWGetResults(0); // take first result

	  //	  std::cout << url << std::endl;
	  

	  // find attributes of first url with given filter & scope
	  // returns after getting the first attribute
	  
	  int len = url.length();	    
	      
	  if (len>0) 
	    {
	      
	      int pos = url.find(':' + service+"://") ; // search if valid url
	      
	      if (pos > 5 ) // found as valid
		{
		  
		  pos= pos + service.length() + 4;  // point to host start
		  
		  while( (pos < len) && (url[pos] != ':')  ) // copy until end or :
		    {
		      
		      host = host + url[pos];
		      ++ pos;
		    }

		  if ( (url[pos] == ':') && (pos < len) )
		    {

		      ++pos;
		      std::string tmps = "";
		      int tmpi = 0;

		      while (pos < len)
			{
			  tmps=tmps+url[pos];
			  ++pos;
			}

		      tmpi = atoi( tmps.c_str() );

		      if (tmpi != 0 ) port = tmpi;

		    }

		  
		  
		  
		}
	    }
	  
	  //	  std::cout << host << std::endl;
	  
	  
	  
	}
      else
	{
	  //cout << "FindSrvs failed" << endl;
	}
      
      SLPWClose();
    }
  else
    {
      //cout << "Open error" << endl;
      return false;
    }


  return true; // return with found IOR or empty string


}

