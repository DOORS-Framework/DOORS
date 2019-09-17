#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

#include "CMB.h"
#include "cosnaming.h"
#include <doors/cmb_impl.h>
#include <doors/slpwrap.h>

CosNaming::NamingContext_var ns;

const string ns_name = "namingservice";
string broker_name = "CorbaMessageBroker";
string broker_kind = "cn";


void own_catch_int(int)
{
    std::cout << "\bSIGINT received!" << std::endl;
    std::cout << "\nBailing out...just a sec, unbinding..";
    std::cout.flush();

    // Signalhandler can't be called before initialaising ns,
    // so it can't fail :/
    CosNaming::Name name;
    name.length (4);
    name[0].id = "FourthFloor";
    name[0].kind = "ou";
    name[1].id = "Telecommunications";
    name[1].kind = "ou";
    name[2].id = "HC414";
    name[2].kind = "ou";
    name[3].id = broker_name.c_str();
    name[3].kind = broker_kind.c_str();

    ns->unbind(name);

    std::cout << "..there, now good bye." << std::endl;
    exit(1);
}


int main( int argc, char *argv[] )
{
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();

   Broker_impl *broker = new Broker_impl;

   std::cout << "Activating broker" << std::endl;
   PortableServer::ObjectId_var oid = poa->activate_object( broker );

   CORBA::Object_ptr temp_obj = poa->servant_to_reference(broker);
   std::cout << orb->object_to_string(temp_obj) << std::endl;

   //   CORBA::String_var ns_str_ior = CORBA::string_dup(FindIORwithSLP(ns_name).c_str());

   // CORBA::Object_var ns_ref = orb->string_to_object(ns_str_ior);

   // ns = CosNaming::NamingContext::_narrow(ns_ref);


   CosNaming::Name name;
   name.length (4);
   name[0].id = "FourthFloor";
   name[0].kind = "ou";
   name[1].id = "Telecommunications";
   name[1].kind = "ou";
   name[2].id = "HC414";
   name[2].kind = "ou";
   name[3].id = broker_name.c_str();
   name[3].kind = broker_kind.c_str();


   std::cout << "Binding to NS" << std::endl;
   // ns->bind( name, broker->_this() );

   std::cout << "Activating signal handler" << std::endl;
   // signal(SIGINT, own_catch_int);
 
   std::cout << "Broker up and running" << std::endl;
   mgr->activate ();
   orb->run();
   
   
   poa->destroy (TRUE, TRUE);
   delete broker;
   return 0;
}

