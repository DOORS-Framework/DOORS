/* cmb_impl.hh: Corba Message Broker system. This file defines classes
 * and attributes for In_impl, Out_impl and Broker_impl classes that
 * implement the CMB-interface declared int CMB.idl. Implementations in
 * cmb_impl.cc.  Author: Joona Hartman
 */


#ifndef CMB_IMPL_HH
#define CMB_IMPL_HH

// For broker logfile
#include <fstream>
#include <vector>
#include <map>
#include <string>

#include "CMB.h"

// Class for outgoing messages interface. That is for sending messages.
// The actual client program should do some checkings before sending
// messages to Broker. Also a wrapper of somekind could be build for this. :)
class Out_impl : virtual public POA_CMB::Out {
    
 public:
    
    Out_impl();
    ~Out_impl();
    CORBA::Boolean Ping() { return true; }
    
 private:

};


// Class for receiving messages from other Outboxes.
class In_impl : public POA_CMB::In {

public:

    In_impl();
    ~In_impl();

    // Print the message to the user. 
    void PrintMessage( const char *fromwho, const char *msg );

    CORBA::Boolean Ping() { return true; }


 private:
    // Our own int-to-string function(by Jani).
    std::string itos( CORBA::ULong value );

    // Help function for getting the time of message
    std::string GetTime();

};


// Class for brokering the messages. Outbox sends Broker the
// message and Broker then resolves the address of the Inbox
// and prints the message with In_impl::PrintMessage().
// No overflow checks are performed, do them in outbox and/or
// inbox. Also Broker presumes legitime inputs.
class Broker_impl : public POA_CMB::Broker {

 public:

    Broker_impl( std::string logfilename );
    Broker_impl();
    ~Broker_impl();

    // Registers a new Inbox to the system. Name is given in who
    // and obj should be given as inbox->_this().
    // Register returns a key to be used when sending msgs and unregistering.
    CORBA::ULong Register( const char *who, CORBA::Object_ptr obj );

    // Unregisters who if key is correct. Returns true if successfull
    CORBA::Boolean Unregister( const char *who, CORBA::ULong key );

    CORBA::Boolean Ping() { return true; }

    // Outbox calls SendMessage() when sending a message to someone.
    // rec_list holds a list of recipients(a comma/space separated list)
    // fromwho and key must match.
    CORBA::Boolean SendMessage( const char *rec_list,
				const char *fromwho,
				CORBA::ULong key, const char *message );

 private:
    // Our own int-to-string function(by Jani).
    std::string itos( CORBA::ULong value );

    // A struct to hold key, object pairs
    typedef struct{ CORBA::Object_ptr objRef; CORBA::ULong key; } User;

    // Map of users with objectrefs and keys. Indexed by username.
    std::map< CORBA::String_var, User > userlist_;

    // Fetch an object reference for user who.
    CORBA::Object_ptr GetObj( std::string who );

    // Fetch key for who.
    CORBA::ULong GetKey( std::string who );

    // Log an event. No timestamps though yet..
    bool PutToLog(std::string msg);

    // Build a list of receivers from rec_list to a more convenient
    // std::vector form.
    void MakeRecList(const char *rec_list, std::vector<std::string> &recipients);

    // A stream handle to logfile.
    std::ofstream logfile_;
    std::string logfilename_; // Name of the logfile.
};

#endif
