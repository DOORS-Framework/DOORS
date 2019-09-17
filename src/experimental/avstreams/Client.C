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

$Log: Client.C,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/08/31 13:58:57  ik
AVStreams changes and modifications to configure.in accordingly.

Revision 1.1  2001/08/15 10:08:47  ik
Renaming some files to conform other DOORS files.

Revision 1.2  2001/06/08 16:46:45  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:51  bilhanan
Full licence clause added into doors 0.1


*/

#include <doors/Audio_impl.h>
#include <string>
#include <MpegAudioCodec.h>
#include "CosNaming.h"
#include <doors/slpwrap.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

static CORBA::String_var ctrlstr;
static CORBA::String_var sinkstr;
static int argcount = 0;
static char **argvector = NULL;
static CORBA::Short _service_type = 0;
static CORBA::Boolean _multicasting = FALSE;

static void* 
server_thread(void* arg)
{
  ::CORBA::ORB_ptr orb = 
    ::CORBA::ORB_init(argcount, argvector, "mico-local-orb");
  ::CORBA::Object_ptr poaobj = orb->resolve_initial_references("RootPOA");
  PortableServer::POA_ptr poa = PortableServer::POA::_narrow(poaobj);
  PortableServer::POAManager_ptr mgr = poa->the_POAManager();

  MpegAudioCtrl_impl* pCtrlImpl = new MpegAudioCtrl_impl;
  PortableServer::ObjectId_var ctrloid = poa->activate_object(pCtrlImpl);
  CORBA::Object_var ctrlref = poa->id_to_reference(ctrloid.in());
  ctrlstr = orb->object_to_string(ctrlref.in());

#if 0
  MpegAudioSink_impl* pSinkImpl = new MpegAudioSink_impl;
  PortableServer::ObjectId_var sinkoid = poa->activate_object(pSinkImpl);
  CORBA::Object_var sinkref = poa->id_to_reference(sinkoid.in());
  sinkstr = orb->object_to_string(sinkref.in());
#endif

  mgr->activate();
  orb->run();

  pthread_exit(NULL);
  return NULL;
}

void
create_qos(AVStreams::streamQoS* qos)
{
  qos->length(1);
  (*qos)[0].QoSType = CORBA::string_dup("audio_QoS");
  (*qos)[0].QoSParams.length(2);
  (*qos)[0].QoSParams[0].property_name = CORBA::string_dup("ServiceType");
  (*qos)[0].QoSParams[0].property_value <<= _service_type;
  (*qos)[0].QoSParams[1].property_name = CORBA::string_dup("Multicast");
  (*qos)[0].QoSParams[1].property_value <<= 
    CORBA::Any::from_boolean(_multicasting);
}

int
main(int argc, char* argv[], char*[])
{
  register int i, j;
  CORBA::String_var ref;
  pthread_t thr;
  bool connected = false;

  argcount = argc;
  argvector = argv;

  //
  // Create ORB
  //
  ::CORBA::ORB_ptr orb = ::CORBA::ORB_init(argc, argv, "mico-local-orb");

  // Read from a file
  char pwd[256], uri[300];
#if 0
  sprintf(uri, "file://%s/mpegaudiocodec.ref", getcwd(pwd, 256));
  CORBA::Object* codecobj = orb->string_to_object (uri);
  if (CORBA::is_nil(codecobj)) {
    cerr << "Cannot bind to MpegAudioCodec." << endl;
    exit(1);
  }
  MpegAudioCodec_ptr pCodec = MpegAudioCodec::_narrow(codecobj);
#endif

#if 0
  sprintf(uri, "file://%s/mpegaudiosource.ref", getcwd(pwd, 256));
  CORBA::Object* srcobj = orb->string_to_object (uri);
  if (CORBA::is_nil(srcobj)) {
    cerr << "Cannot bind to MpegAudioSource." << endl;
    exit(1);
  }
  MpegAudioSource_ptr pSrc = MpegAudioSource::_narrow(srcobj);
#endif

#if 1
  const std::string ns_name = "namingservice";
  // Resolve source name from NamingService
  std::string str_ior = FindIORwithSLP(ns_name);
  CORBA::Object_ptr namingserver = orb->string_to_object(str_ior.c_str());
  CosNaming::NamingContext_ptr ns = CosNaming::NamingContext::_narrow(namingserver);
  const CORBA::String_var server_name = "MpegAudioSource";
  const CORBA::String_var server_kind = "cn";
  CosNaming::Name srcname;
  srcname.length (4);
  srcname[0].id = "FourthFloor";
  srcname[0].kind = "ou";
  srcname[1].id = "Telecommunications";
  srcname[1].kind = "ou";
  srcname[2].id = "HB420";
  srcname[2].kind = "ou";
  srcname[3].id = CORBA::string_dup(server_name);
  srcname[3].kind = CORBA::string_dup(server_kind);
  std::cout << "Resolving MpegAudioSource..." << std::flush;
  CORBA::Object_ptr srcobj = ns->resolve(srcname);
  MpegAudioSource_ptr pSrc = MpegAudioSource::_narrow(srcobj);
  if (pSrc) {
    std::cout << "ok." << std::endl;
  } else {
    std::cout << "not ok." << std::endl;
  }
  // Resolve sink name from NamingService
  const CORBA::String_var sink_name = "MpegAudioSink";
  const CORBA::String_var sink_kind = "cn";
  CosNaming::Name sinkname;
  sinkname.length (4);
  sinkname[0].id = "FourthFloor";
  sinkname[0].kind = "ou";
  sinkname[1].id = "Telecommunications";
  sinkname[1].kind = "ou";
  sinkname[2].id = "HB420";
  sinkname[2].kind = "ou";
  sinkname[3].id = CORBA::string_dup(sink_name);
  sinkname[3].kind = CORBA::string_dup(sink_kind);
  std::cout << "Resolving MpegAudioSink..." << std::flush;
  CORBA::Object_ptr sinkobj = ns->resolve(sinkname);
  MpegAudioSink_ptr pSink = MpegAudioSink::_narrow(sinkobj);
  if (pSink) {
    std::cout << "ok." << std::endl;
  } else {
    std::cout << "not ok." << std::endl;
  }
#endif

#if 0
  sprintf(uri, "file://%s/mpegaudioctrl.ref", getcwd(pwd, 256));
  CORBA::Object* ctrlobj = orb->string_to_object (uri);
  if (CORBA::is_nil(ctrlobj)) {
    cerr << "Cannot bind to MpegAudioCtrl." << endl;
    exit(1);
  }
  MpegAudioCtrl_ptr pCtrl = MpegAudioCtrl::_narrow(ctrlobj);
  if (pCtrl) {
    std::cout << "Ctrl narrowed." << std::endl << std::flush;
  }
#endif

  pthread_create(&thr, NULL, server_thread, NULL);
  sleep(1);

  CORBA::Object* ctrlobj = orb->string_to_object (ctrlstr);
  if (CORBA::is_nil(ctrlobj)) {
    cerr << "Cannot bind to MpegAudioCtrl." << endl;
    exit(1);
  }
  MpegAudioCtrl_ptr pCtrl = MpegAudioCtrl::_narrow(ctrlobj);
  if (pCtrl) {
    std::cout << "Ctrl narrowed." << std::endl << std::flush;
  }

#if 0
  CORBA::Object* sinkobj = orb->string_to_object (sinkstr);
  MpegAudioSink_ptr pSink = MpegAudioSink::_narrow(sinkobj);

  if (pSink) {
    std::cout << "Sink narrowed." << std::endl << std::flush;
  }
#endif

  // Try to bind source and sink
  AVStreams::flowSpec* flows = new AVStreams::flowSpec;
  AVStreams::streamQoS* qos = new AVStreams::streamQoS;

  string line;
  PlayList* playlist = NULL;
  for ( ; ; ) {
    std::cout << "C(onnect), D(isconnect), Q(uit), L(ist), S(elect), P(lay), ";
    std::cout << "B(reak)" << std::endl;
    std::cout << "Client$ ";
    std::cin >> line;
    if (line == "q") {
      break;
    } else if (line == "c") {
      _multicasting = FALSE;
      _service_type = 1;
      create_qos(qos);
      if (pCtrl->bind_mpegaudio(pSrc, pSink, *qos, *flows)) {
	std::cout << "Connected." << std::endl << std::flush;
	connected = true;
      }
    } else if (line == "cm") {
      _multicasting = TRUE;
      _service_type = 1;
      create_qos(qos);
      if (pCtrl->bind_mpegaudio(pSrc, pSink, *qos, *flows)) {
	std::cout << "Connected." << std::endl << std::flush;
	connected = true;
      }
    } else if (line == "d") {
      pCtrl->unbind_mpegaudio();
      std::cout << "Disconnected." << std::endl << std::flush;
      connected = false;
    } else if (line == "l") {
      if (pCtrl->get_list(playlist)) {
	int len = playlist->length();
	if (len > 0) {
	  for (register int i = 0; i < len; i++) {
	    char buf[80];
	    char* tmp = (*playlist)[i];
	    snprintf(buf, 80, "%02d: %s", i + 1, tmp);
	    cout << buf << endl;
	  }
	} else {
	  cout << "Empty list received." << endl;
	}
      }
    } else if (line.substr(0, 1) == "s") {
      std::string select = (char*)(line.c_str() + 1);
      pCtrl->select_item(CORBA::string_dup(select.c_str()));
    } else if (line == "p") {
      pCtrl->start(*flows);
    } else if (line == "b") {
      pCtrl->stop(*flows);
    }
  }

#if 0
  Ui ui;
  ui.redirect(); // Redirect cout etc to UI
  ui.set_statusbar("Not connected");
  AVStreams::flowSpec flowspec;
  char cmdbuf[CMDBUFSIZE];
  PlayList* playlist = NULL;
  string selected_file;
  for ( ; ; ) {
    ui.get_command(cmdbuf, CMDBUFSIZE);
    if (*cmdbuf == '/') { // Command?
      char* cmd = cmdbuf + 1;
      if (!strcmp(cmd, "quit")) {
	break;
      } else if (!strncmp(cmd, "list", 2)) {
	if (!connected) {
	  continue;
	}
	if (pCtrl->get_list(playlist)) {
	  int len = playlist->length();
	  if (len > 0) {
	    for (register int i = 0; i < len; i++) {
	      char buf[80];
	      char* tmp = (*playlist)[i];
	      snprintf(buf, 80, "%02d: %s", i + 1, tmp);
	      cout << buf << endl;
	    }
	  } else {
	    cout << "Empty list received." << endl;
	  }
	}
      } else if (!strncmp(cmd, "play", 2)) {
	if (!connected) {
	  continue;
	}
	pCtrl->start(flowspec);
	cout << "Playing." << endl;
      } else if (!strncmp(cmd, "stop", 2)) {
	if (!connected) {
	  continue;
	}
	pCtrl->stop(flowspec);
	cout << "Stopped." << endl;
      } else if (!strncmp(cmd, "rewind", 2)) {
	if (!connected) {
	  continue;
	}
	pCtrl->rewind(flowspec);
	cout << "Rewinding." << endl;
      } else if (!strncmp(cmd, "clear", 2)) {
	ui.enter_nowindow();
	ui.leave_nowindow();
      } else if (!strncmp(cmd, "sel", 3)) {
	if (!connected) {
	  continue;
	}
	char *p = strchr(cmd, ' ');
	if (p) {
	  pCtrl->select_item(CORBA::string_dup(p));
	} else {
	  pCtrl->select_item("");
	}
      } else if (!strncmp(cmd, "bind", 4)) {
	char *p = strchr(cmd, ' ');
	if (p) {
	  if (strchr(p, 'm') || strchr(p, 'M')) {
	    _service_type = 0/*best effort*/;
	    _multicasting = TRUE;
	  } else if (strchr(p, 'u') || strchr(p, 'U')) {
	    _service_type = 0;
	    _multicasting = FALSE;
	  } else if (strchr(p, 't') || strchr(p, 'T')) {
	    _service_type = 1/*guaranteed*/;
	    _multicasting = FALSE;
	  }
	} else {  
	  _service_type = 1;
	  _multicasting = false;
	}
	create_qos(qos);
	ui.enter_nowindow();
	if (pCtrl->bind_mpegaudio(pSrc, pSink, *qos, *flows)) {
	  ui.set_statusbar("Connected");
	  connected = true;
	}
	sleep(2);
	ui.leave_nowindow();
      }
    }
  }
#endif
  
  return 0;
}
