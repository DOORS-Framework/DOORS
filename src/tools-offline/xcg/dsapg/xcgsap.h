//Editor-Info: -*- C++ -*-

//  Copyright 2000
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

$Log: xcgsap.h,v $
Revision 1.1  2002/06/17 13:29:55  bilhanan
*** empty log message ***

Revision 1.1  2002/06/17 07:32:33  hartmanj
Renamed sap.h/C to xcgsap.h/C

Revision 1.3  2002/06/14 13:41:30  hartmanj
Changed #includes

Revision 1.2  2002/06/07 12:19:08  hartmanj
Full licence clause added


*/


// SAP -class

//060202 petri Draft version
//110202 petri Class-name changed: OwnMain -> SAP
//250202 petri Commented and small non-functional changes.

// Class descripes the Service Access Pointf of Protocl Layer

#ifndef SAP_HH
#define SAP_HH

#include <vector>
#include <doors/structure.h>
#include <doors/sapinterface.h>
#include <doors/sapmessage.h>


extern int ilevel;


class SAP : public Structure
{
public:
  
  SAP(const std::string &name);
  virtual ~SAP();
  
  // addParent adds inheritance parents to SAP-class.
  // SAP can have as many parents as needed.
  virtual void addParent(const std::string &parent);
  
  // Links SAPInterface-class to SAP as a UserInterface.
  void setUserInterface(SAPInterface *user);
  // Links SAPInterface-class to SAP as a ProviderInterface.
  void setProviderInterface(SAPInterface *provider);
  
  // Return the user and provider interfaces.
  SAPInterface* getUser() const;
  SAPInterface* getProvider () const;
  
  // Prints the SAP header file.
  // Prints "recursevely" SAPInterface and SAPmessage headers as well.
  virtual void printHeader(std::ostream &header);
  
  // Prints the SAP implementation file.
  // Prints "recursevely" SAPInterface and SAPmessage-function's
  // implementations  as well.
  virtual void printImpl(std::ostream &impl);
    
private:
  // Copy-constructor (not implemented)
  SAP(const SAP &s);
  
  // SAP-name
  std::string name_;
  // SAP's parents as a vector of strings.
  std::vector<std::string> parents_;
  // User and Provider interfaces.
  SAPInterface *user_;
  SAPInterface *provider_;
};
#endif // SAP_HH
