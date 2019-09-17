//  Copyright 2000 
//  	Telecoms Lab, Tampere University of Technology.  All rights reserved.

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

$Log: propertyd.C,v $
Revision 1.2  2002/05/16 15:45:10  bilhanan
General development

Revision 1.1.1.1  2002/04/30 12:32:48  bilhanan
Development

Revision 1.5  2001/08/31 15:02:15  ik
Typos in Makefile.am's of AVStreams and PropertyService. TRACE-
commands removed from PropertyService_impl.C because SunC could
not understand them like gcc.

Revision 1.4  2001/08/03 15:22:10  ik
PropertyService uses SLP to resolve NamingService and binds itself
there. Added SLP wrapper and CosNaming files temporarily so that
propertyd compiles.

Revision 1.3  2001/07/27 12:24:39  ik
Makefiles added for propertyservice. Some lines of DSI code there.

Revision 1.2  2001/06/08 16:46:42  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:49  bilhanan
Full licence clause added into doors 0.1


*/
//
// +------------------------------------------------------------------------+
// !  file:     server.C
// !  date:     24.3.2000
// !  subject:  Server of DOORS PropertyService
// !  desc:     
// !  author:   ilkka karvinen, TTKK/DMI
// +------------------------------------------------------------------------+

#include <fstream>
#include <signal.h>
#include <doors/PropertyService_impl.h>

// Function declarations
void own_catch_int(int);

void 
own_catch_int(int)
{
  // TODO: Unbind NamingService
  // Unbind our service from NamingService

  exit(1);
}


int 
main (int argc, char *argv[])
{

  // ORB initialization
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  // Obtain reference to RootPOA and POAManager
  CORBA::Object_var poaobj = orb->resolve_initial_references("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow(poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  // Create factory for PropertySets
  PropertySetFactory_impl* factory = new PropertySetFactory_impl(poa);

  // Activate factory
  PortableServer::ObjectId_var oid = poa->activate_object(factory);

  CORBA::Object_var ref = poa->id_to_reference (oid.in());
  CORBA::String_var str = orb->object_to_string (ref.in());

  // TODO: Bind to NamingService

  std::cout << "PropertyFactory is running" << std::endl;

  // Catch interrupts to make sure that our service is removed on exit
  signal(SIGINT, own_catch_int);

  // Activate and run
  mgr->activate();
  orb->run();

  // Destroy POA and delete factories
  poa->destroy(TRUE, TRUE);

  delete factory;

  return 0;
}
