//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
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

$Log: Sink.C,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.1  2001/08/31 13:58:58  ik
AVStreams changes and modifications to configure.in accordingly.

Revision 1.1  2001/08/15 10:08:48  ik
Renaming some files to conform other DOORS files.

Revision 1.2  2001/06/08 16:46:45  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:52  bilhanan
Full licence clause added into doors 0.1


*/

#define WITH_SLP

#include <doors/doorsconf.h>
#include <doors/Audio_impl.h>
#ifdef WITH_SLP
#include <signal.h>
#include "CosNaming.h"
#include <doors/slpwrap.h>
#endif

#include <stdlib.h>
#include <errno.h>

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

#ifdef WITH_SLP
static CosNaming::NamingContext_ptr ns;

// Function declarations
void own_catch_int(int);
void bind_to_namingservice(CORBA::Object_ptr server_ref, 
                           MpegAudioSource_impl* psrc);

void 
own_catch_int(int)
{
  const CORBA::String_var server_name = "MpegAudioSink";
  const CORBA::String_var server_kind = "cn";

  std::cout << "Unbinding from NamingService..." << std::flush;

  // Unbind our service from NamingService
  CosNaming::Name name;
  name.length (4);
  name[0].id = "FourthFloor";
  name[0].kind = "ou";
  name[1].id = "Telecommunications";
  name[1].kind = "ou";
  name[2].id = "HB420";
  name[2].kind = "ou";
  name[3].id = server_name;
  name[3].kind = server_kind;
  ns->unbind(name);

  std::cout << "ok." << std::endl << std::flush;

  exit(1);
}

void
bind_to_namingservice(CORBA::Object_ptr server_ref,
                      MpegAudioSink_impl* psink)
{
  const CORBA::String_var server_name = "MpegAudioSink";
  const CORBA::String_var server_kind = "cn";

  CosNaming::Name name;
  name.length (4);
  name[0].id = "FourthFloor";
  name[0].kind = "ou";
  name[1].id = "Telecommunications";
  name[1].kind = "ou";
  name[2].id = "HB420";
  name[2].kind = "ou";
  name[3].id = server_name;
  name[3].kind = server_kind;
  
  std::cout << "Binding to NamingService..." << std::flush;

  // Get a reference
  ns = CosNaming::NamingContext::_narrow(server_ref);

  // Start DII invocation
  CORBA::Request_var request = ns->_request("bind");
  (void)request->arguments();
  
  // Add arguments
  request->add_in_arg("n") <<= name;
  request->add_in_arg("obj") <<= psink->_this();

  request->invoke();

  std::cout << "ok." << std::endl << std::flush;
}
#endif

int
main(int argc, char* argv[], char*[])
{
  string path = ".";

  if (argc > 1) {
    path = argv[1];
  }

  //
  // Create ORB and BOA
  //
  ::CORBA::ORB_ptr orb = ::CORBA::ORB_init(argc, argv, "mico-local-orb");
  ::CORBA::Object_ptr poaobj = orb->resolve_initial_references("RootPOA");
  PortableServer::POA_ptr poa = PortableServer::POA::_narrow(poaobj);
  PortableServer::POAManager_ptr mgr = poa->the_POAManager();
  
  //
  // Create implementation object(s)
  //

  // FileMAS = File MPEG Audio Server
  MpegAudioSink_impl* pSink = new MpegAudioSink_impl;
  //  FileMAS* pSink = new FileMAS(path);
  PortableServer::ObjectId_var oid2 = poa->activate_object(pSink);

  // Write sink to file
  ofstream srcf("mpegaudiosink.ref");
  CORBA::Object_var srcref = poa->id_to_reference (oid2.in());
  CORBA::String_var srcstr = orb->object_to_string (srcref.in());
  srcf << srcstr.in() << endl;
  srcf.close ();

#ifdef WITH_SLP
  const std::string service = "namingservice";

  cout << "Resolving NamingService using SLP..." << endl;
  // Get NamingService from SLP
  std::string ns_ior = FindIORwithSLP(service);

  if (ns_ior.empty()) {
    cout << "Could not resolve NamingService." << endl;
  } else {
    //    cout << "NamingService IOR: <" << ns_ior << ">" << endl;

    CORBA::Object_ptr server_ref = orb->string_to_object(ns_ior.c_str());
    bind_to_namingservice(server_ref, pSink);
  }

  // Catch interrupts to make sure that our service is removed on exit
  signal(SIGINT, own_catch_int);
#endif

#if 0
  // Control
  MpegAudioCtrl_impl* pCtrlImpl = new MpegAudioCtrl_impl;
  PortableServer::ObjectId_var ctrloid = poa->activate_object(pCtrlImpl);

  ofstream ctrlf("mpegaudioctrl.ref");
  CORBA::Object_var ctrlref = poa->id_to_reference(ctrloid.in());
  CORBA::String_var ctrlstr = orb->object_to_string(ctrlref.in());
  ctrlf << ctrlstr.in() << endl;
  ctrlf.close ();
#endif
  
  mgr->activate();
  orb->run();

  poa->destroy(TRUE, TRUE);
  delete pSink;
  return 0;
}


