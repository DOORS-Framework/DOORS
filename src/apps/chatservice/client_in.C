
/* Example implementation of inbox. Just the basics, nothing fancy.
 * At SIGINT(^C) inbox unregisters from Broker, so a zombie box will
 * not be left there. Also sending SIGINT is the only way of ending
 * this program.
 */

#include <fstream>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

#include <doors/cmb_impl.h>
#include <doors/slpwrap.h>
#include "cosnaming.h"

// Some fixed variables for locating services
const std::string ns_name = "namingservice";
const std::string broker_name = "CorbaMessageBroker";
const std::string broker_kind = "cn";

// "Account" info
std::string whoami = "";
CORBA::ULong key;

// Handle to the actual broker
CMB::Broker_ptr broker;

// int to string. 
static std::string itos(const CORBA::ULong value)
{
  CORBA::ULong work = value;

  char chars[10] = {'0','1','2','3','4','5','6','7','8','9'};

  std::string temp = "";

  do 
  {
      temp = chars[ (work % 10) ] + temp;
      work = work / 10;
  }
  while (work>0 );

  if( value < 10 )
      temp = '0' + temp;


  return temp;
}

void own_catch_int(int)
{
    std::cout << "\bSIGINT received!" << std::endl;
    std::cout << "\nBailing out...just a sec, unbinding..";
    std::cout.flush();

    // Signalhandler can't be called before initialaising ns,
    // so it can't fail :/

    broker->Unregister(whoami.c_str(), key);

    std::cout << "..there, now good bye." << std::endl;
    exit(1);
}


int main( int argc, char *argv[] )
{
    // The usual corba stuff
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();


    // Determine our name(argc must be 2 _after_ orb has taken it's args
    assert(argc==2);
    whoami = argv[1];

    std::cout << "You are " << whoami << "." << std::endl;
    // We'll start our inbox first

    std::cout << "Creating inbox" << std::endl;
    In_impl *inbox = new In_impl;
    PortableServer::ObjectId_var inbox_id = poa->activate_object( inbox );


    std::cout << "Finding Corba Message Broker" << std::endl;

    // We need the namingservice, let's use slp

    CORBA::String_var ns_str_ior = 
	CORBA::string_dup(FindIORwithSLP(ns_name).c_str());

    // Change the string to object
    CORBA::Object_var namingservice = orb->string_to_object(ns_str_ior);


    // Now narrow it
    CosNaming::NamingContext_var ns = 
	CosNaming::NamingContext::_narrow(namingservice);

    // We are looking for a CMB(Corba Messaging Broker)
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


    // NamingService will give it to us
    CORBA::Object_var obj = ns->resolve(name);
    broker = CMB::Broker::_narrow(obj);


    std::cout << "Registering as " << whoami <<  std::endl;
    key = broker->Register( whoami.c_str(), inbox->_this() );

    assert(key);

    // Give the key to outbox, for now just use a file
    std::cout << "Saving key and name" << std::endl;
    // Put the key to a file
    std::ofstream oFile("doors.key");
    std::string str_key = itos(key);
    oFile << str_key << std::endl << whoami << std::endl;
    oFile.close();

    std::cout << "Inbox ready for messages!" << std::endl;
    inbox->PrintMessage(whoami.c_str(), "Welcome to the inbox");

    // Register signal handler
    signal(SIGINT, own_catch_int);


    mgr->activate ();
    orb->run();


    // We won't get here though..
    poa->destroy (TRUE, TRUE);
    delete inbox;
    delete broker;

    return 0;
}
