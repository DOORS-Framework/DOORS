//  Copyright 2004
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

$Log: movementtest.C,v $
Revision 1.2  2004/05/21 06:07:51  hartmanj
To use MobilitySAP::User::forget() instead of .disconnect()

Revision 1.1  2004/05/19 13:52:02  hartmanj
Initial revision


*/

#include <doors/lemon.h>
#include <doors/debug.h>
#include "testsap.h"
#include "mobmodule.h"
#include "mobilitysap.h"

class MobTestTask : public EventTask {

public:

    MobTestTask( std::string n, Scheduler *s );
    ~MobTestTask( );

    virtual bool execute( EventType type, Message *msg );
    bool ready_movement( Message *msg);
    bool ready_start( Message *msg);
    bool ready_stop( Message *msg);

    TestSAP :: Provider up;

private:

    MobilitySAP :: User mobapi;
    InetAddr6 home_addr;
    InetAddr6 careof_addr;
};

MobTestTask :: MobTestTask( std::string n, Scheduler *s )
  : EventTask (n,s), mobapi(this), up(this) {
    STATIC_SYMBOL(home_addr,this);
    STATIC_SYMBOL(careof_addr,this);
}

MobTestTask :: ~MobTestTask( )
{
    mobapi.forget();
}

bool MobTestTask :: ready_movement( Message *msg )
{
    _DOORS_DEBUG(0, "MobTestTask :: ready_movement ["<<getName() <<"]");

    MobilitySAP :: Movement *mov = (MobilitySAP :: Movement*)msg;

    _DOORS_DEBUG(0, "coa: " << mov->coa.getHostAddr());
    _DOORS_DEBUG(0, "home_addr: " << mov->ha.getHostAddr());

    home_addr = mov->ha;
    careof_addr = mov->coa;

    return true;
}

bool MobTestTask :: ready_start( Message *msg )
{
    _DOORS_DEBUG(0, "MobTestTask :: ready_start");

    mobapi.notify();

    return true;
}

bool MobTestTask :: ready_stop( Message *msg )
{
    _DOORS_DEBUG(0, "MobTestTask :: ready_stop");

    mobapi.forget();

    return true;
}

bool MobTestTask :: execute( EventType type, Message *msg )
{
    msg->unlock();
    if( msg->getTarget() == &mobapi ) {

        if( msg->getType() == MobilitySAP :: movement ) {
            return ready_movement( msg );
        }

    }

    if( msg->getTarget() == &up ) {

        if( msg->getType() == TestSAP :: start ) {
            return ready_start( msg );
        }

        if( msg->getType() == TestSAP :: stop ) {
            return ready_stop( msg );
        }

    }

    return true;
}

int main (int argc, char **argv)
{

     // create input-output handler for tasks
     IoHandler *io = IoHandler :: Instance ();

     // create main scheduler
     Scheduler *sched = Scheduler :: Instance();
     // create task for managing timers
     TimerTask *tt  = TimerTask :: instance (sched, io);
     UiTask ui ( sched, io );
     DebugScheduler ds (0, 0, sched);
     EnvTask env ( sched );


     MobTestTask dummy_task1( "test.task1", sched );
     MobTestTask dummy_task2( "test.task2", sched );
     MobTestTask dummy_task3( "test.task3", sched );
     MobTestTask dummy_task4( "test.task4", sched );
     STATIC_SYMBOL(dummy_task1,0);
     STATIC_SYMBOL(dummy_task2,0);
     STATIC_SYMBOL(dummy_task3,0);
     STATIC_SYMBOL(dummy_task4,0);

     TestSAP :: User *usr = new TestSAP :: User(&env,0);
     dummy_task1.up.connect(usr);
     env.add (usr, "usr1");
     TestSAP :: User *usr2 = new TestSAP :: User(&env,0);
     dummy_task2.up.connect(usr2);
     env.add (usr2, "usr2");
     TestSAP :: User *usr3 = new TestSAP :: User(&env,0);
     dummy_task3.up.connect(usr3);
     env.add (usr3, "usr3");
     TestSAP :: User *usr4 = new TestSAP :: User(&env,0);
     dummy_task4.up.connect(usr4);
     env.add (usr4, "usr4");

     // Let the DOORS OPEN!
     while( ui.isTerminated () == false ) {
          sched->run ();
     }

     delete usr;
     delete usr2;
     delete usr3;
     delete usr4;

     return 0;
}
