#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <doors/ptb.h>
#include <doors/lemon.h>
#include <doors/udp.h>
#include <doors/udptask.h>
#include <doors/inetaddr.h>
#include "coapservertask.hh"

int main (int argc, char** argv)
{


  if (argc < 2) {
    std::cout << "Usage: server <listening port number>" << std::endl;
    return 0;
  }

  int port_number = atoi(argv[1]);
  std::cout<< "Port number:" << port_number << std::endl;
  // Create I/O Handler
  IoHandler *io = IoHandler :: Instance ();

  // Create main scheduler
  Scheduler *s = Scheduler :: Instance ();

  // Create TimerTask
  TimerTask *tt = TimerTask :: instance (s, io);
  
  UiTask ui (s, io);

  // Environment task
  EnvTask env (s);

  // Scheduler for debugging
  DebugScheduler ds (0, 0, s);


  CoAPServerSM coapsm;
  CoAPserverTask coap(std::string("CoAP Server"), &ds, &coapsm);

  // This notifies the debugger of a new task to trace
  STATIC_SYMBOL (coap, 0);

  InetAddr own;
  own.set (port_number); // Just set the port number; any host will match
  UdpTask udp("udp", &ds, io, &own);

  STATIC_SYMBOL (udp, 0);
  
  CoAPSAP :: User *usr = new CoAPSAP :: User(&env, 0);
  env.add (usr, "usr");

  // Connect usr environment SAP to altbit's up SAP
  coap.up.connect(usr);

  // Connect UDP task to Altbit
  udp.up.connect (&coap.down);

  while (ui.isTerminated () == false) {
    s->run ();
  }

  DELETE_SYMBOL (&env);
  DELETE_SYMBOL (&coap);
  DELETE_SYMBOL (&udp);
  
  env.remove ();
  
  return 0;
}




