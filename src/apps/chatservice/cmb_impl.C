
#include <map>
#include <ctime>
#include <string>
#include <cstdlib>

#include <doors/cmb_impl.h>


// int to string. 
std::string In_impl::itos(const CORBA::ULong value)
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

// int to string. 
std::string Broker_impl::itos(const CORBA::ULong value)
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



// Class "In"

In_impl::In_impl()
{
}

In_impl::~In_impl()
{
}

void In_impl::PrintMessage( const char *fromwho, const char *msg )
{
    std::cout << GetTime() << fromwho << ": " << msg << std::endl;
//    return true;
}

// Get the time of message print. The time is then actually the time
// of arrival, not the time when message was sent.
std::string In_impl::GetTime( void )
{
    time_t tt;
    time(&tt);
    struct tm *t = localtime(&tt);

    std::string s="(";
    s+=itos(t->tm_hour);
    s+=":";
    s+=itos(t->tm_min);
    s+=")";

    return s;
}


// Class "Out"

Out_impl::Out_impl()
{
}

Out_impl::~Out_impl()
{
}


// Broker_impl class to handle the messaging
Broker_impl::Broker_impl() : logfilename_("broker.log")
{
    // Initialize random seed and logfile, erases old log.
    srand48( time(NULL) );
    logfile_.open(logfilename_.c_str());
}

Broker_impl::Broker_impl( std::string logfilename )
    : logfilename_(logfilename)
{
    srand48( time(NULL) );
    logfile_.open(logfilename_.c_str());
}

Broker_impl::~Broker_impl()
{
    // Close the logfile
    logfile_.close();
}

CORBA::ULong Broker_impl::Register( const char *who, CORBA::Object_ptr obj )
{
    // Put some log to logfile
    std::string log = "Register request from ";
    log +=who;
    PutToLog(log);

    // First check if name is already used
    if( userlist_.find(who) != userlist_.end() )
    {
	return 0;
    }


    // Let's create a user key
    CORBA::ULong k = lrand48();
    // log,log,loog
    log="Assigning key ";
    log+=itos(k);
    log+=" to user ";
    log+=who;
    PutToLog(log);

    // Then we insert everything in the map

    User uusi;
    uusi.objRef = obj->_duplicate(obj);
    uusi.key = k;

    userlist_.insert( std::pair<CORBA::String_var,User>(who,uusi) );

    log="Registering user ";
    log+=who;
    log+=" successfull";
    PutToLog(log);
    return k;
}

CORBA::Boolean Broker_impl::Unregister( const char *who, CORBA::ULong key )
{
    std::string log = "Unregister request from ";
    log+=who;
    log+=" with key ";
    log+=itos(key);
    PutToLog(log);

    // Check that key is valid
    CORBA::ULong k = GetKey(who);

    if( key != k || k == 0 )
    {
	PutToLog("Unregister failed, invalid key");
	return false;
    }

    // Erase the user from map
    User u;
    std::map<CORBA::String_var,User>::iterator iter = userlist_.begin();
    while(iter != userlist_.end())
    {
	if( !strcmp((*iter).first,who) )
	    userlist_.erase(iter);
	iter++;
    }

    log = "Unregistering user ";
    log+=who;
    log+=" was successful";
    PutToLog(log);

    return true; // success
}

CORBA::ULong Broker_impl::GetKey( std::string who )
{
    // Arghh. Fix this awfull code
    User u;
    std::map<CORBA::String_var,User>::iterator iter = userlist_.begin();
    while(iter != userlist_.end())
    {
	if( !strcmp((*iter).first,who.c_str()) )
	    return (*iter).second.key;
	iter++;
    }

    return 0;
}

CORBA::Object_ptr Broker_impl::GetObj( std::string who )
{
    // Arghh. Fix this awfull code
    User u;
    std::map<CORBA::String_var,User>::iterator iter = userlist_.begin();
    while(iter != userlist_.end())
    {
	if( !strcmp((*iter).first,who.c_str()) )
	    return (*iter).second.objRef->_duplicate((*iter).second.objRef);
	iter++;
    }

    return NULL;
}

CORBA::Boolean Broker_impl::SendMessage( const char *rec_list,
				    const char *fromwho,
				    CORBA::ULong key,
				    const char *message )
{
    std::string log="Message from ";
    log+=fromwho;
    log+=" to ";
    log+=rec_list;
    log+=" with key ";
    log+=itos(key);
    log+=". Message:";
    log+=message;
    PutToLog(log);

    // First we check that who is who he(or she) says he(or she) is :)
    CORBA::ULong k = GetKey(fromwho);

    if( k != key || !k )
	return false;

    std::vector<std::string> recipients;
    int rec_offs=0;

    MakeRecList(rec_list,recipients);

    while( rec_offs < recipients.size() )
    {
	// Make a reference to the recipient
	CORBA::Object_ptr rec_obj = GetObj(recipients[rec_offs]);

	// If we got an object reference(things can go wrong)
	if( rec_obj )
	{
	    CMB::In_var recipient = CMB::In::_narrow(rec_obj);
	    recipient->PrintMessage( fromwho, message );
	}
	PutToLog("Message sent successfully.");
	rec_offs++;
    }

    return true;
}

void Broker_impl::MakeRecList( const char *rec_list, std::vector<std::string> &recipients )
{
    unsigned int rec_offs=0, src_offs=0;

    // First ixnay possible spaces in beginning of rec_list
    while( rec_list[rec_offs] == ' ' )
	rec_offs++;

    // Check if magic word "all" is found (could be done more wisely)
    // Btw, includes the sender to the recipients list..
    std::string temp(rec_list);
    // some variations..
    std::string::size_type pos1 = temp.find("all");
    std::string::size_type pos2 = temp.find("ALL");
    std::string::size_type pos3 = temp.find("All");
    if( pos1 != std::string::npos || pos2 != std::string::npos || pos3 != std::string::npos )
    {
	PutToLog("Sending message to all");
	// Send message to all
	unsigned int i = userlist_.size();
	std::map< CORBA::String_var, User >::iterator iter=userlist_.begin();
	while( iter != userlist_.end() )
	    recipients.push_back( static_cast<std::string>((*iter++).first) );

	return; // done
    }

    // Then read comma and/or space separated list of names and make the vector
    while( rec_list[src_offs] != '\0' )
    {
	recipients.push_back("");
	while( rec_list[src_offs] != ',' && rec_list[src_offs] != ' '
	       && rec_list[src_offs] != '\0' )
	{
	    recipients[rec_offs].insert( recipients[rec_offs].end(),
					 rec_list[src_offs] );
	    src_offs++;
	}
	src_offs++;
	rec_offs++;
    }
}

bool Broker_impl::PutToLog( std::string msg )
{
    if( logfile_ ) // If we have a handle
    {
	logfile_ << msg << std::endl;
	return true;
    }

    std::cerr << "Error printing to logfile(" << logfilename_ << ")" << std::endl;
    return false;
}

