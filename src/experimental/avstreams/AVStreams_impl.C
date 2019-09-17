
#include <doors/AVStreams_impl.h>


// Implementation for interface Basic_StreamCtrl

CORBA::Boolean
Basic_StreamCtrl_impl::modify_QoS( AVStreams::streamQoS& new_qos, const AVStreams::flowSpec& the_spec )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::noSuchFlow,
    ::AVStreams::QoSRequestFailed)

{
  CORBA::Boolean retval;

  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

  return retval; 
}


void
Basic_StreamCtrl_impl::push_event( const AVStreams::streamEvent& the_event )
  throw(
    ::CORBA::SystemException)

{
  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

}


// Implementation for interface StreamEndPoint

CORBA::Boolean
StreamEndPoint_impl::modify_QoS( AVStreams::streamQoS& new_qos, const AVStreams::flowSpec& the_flows )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::noSuchFlow,
    ::AVStreams::QoSRequestFailed)

{
  CORBA::Boolean retval;

  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

  return retval; 
}




// Implementation for interface StreamEndPoint_A

CORBA::Boolean
StreamEndPoint_A_impl::connect( AVStreams::StreamEndPoint_ptr responder, AVStreams::streamQoS& qos_spec, const AVStreams::flowSpec& the_spec )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::noSuchFlow,
    ::AVStreams::QoSRequestFailed,
    ::AVStreams::streamOpFailed)

{
  CORBA::Boolean retval;

  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

  return retval; 
}

CORBA::Boolean
StreamEndPoint_A_impl::multiconnect( AVStreams::streamQoS& the_qos, AVStreams::flowSpec& the_spec )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::noSuchFlow,
    ::AVStreams::QoSRequestFailed,
    ::AVStreams::streamOpFailed)

{
  CORBA::Boolean retval;

  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

  return retval; 
}


// Implementation for interface StreamEndPoint_B

void 
StreamEndPoint_B_impl::stop( const AVStreams::flowSpec& the_spec )
  throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow)
{

  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

}

void 
StreamEndPoint_B_impl::start( const AVStreams::flowSpec& the_spec )
  throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow)
{

  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

}

void 
StreamEndPoint_B_impl::destroy( const AVStreams::flowSpec& the_spec )
  throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow)
{

  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

}

CORBA::Boolean 
StreamEndPoint_B_impl::request_connection( AVStreams::StreamEndPoint_ptr initiator, CORBA::Boolean is_mcast, AVStreams::streamQoS& qos, AVStreams::flowSpec& the_spec )
  throw(
        ::CORBA::SystemException,
        ::AVStreams::streamOpDenied,
        ::AVStreams::noSuchFlow,
        ::AVStreams::QoSRequestFailed,
        ::AVStreams::FPError)
{
  CORBA::Boolean retval;

  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

  return retval; 
}



CORBA::Boolean
StreamEndPoint_B_impl::multiconnect( AVStreams::streamQoS& the_qos, AVStreams::flowSpec& the_spec )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::streamOpFailed,
    ::AVStreams::noSuchFlow,
    ::AVStreams::QoSRequestFailed,
    ::AVStreams::FPError)

{
  CORBA::Boolean retval;

  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

  return retval; 
}


// Implementation for interface StreamCtrl

CORBA::Boolean
StreamCtrl_impl::bind( AVStreams::StreamEndPoint_A_ptr a_party, AVStreams::StreamEndPoint_B_ptr b_party, AVStreams::streamQoS& the_qos, const AVStreams::flowSpec& the_flows )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::streamOpFailed,
    ::AVStreams::noSuchFlow,
    ::AVStreams::QoSRequestFailed)

{
  CORBA::Boolean retval;

  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

  return retval; 
}


void
StreamCtrl_impl::unbind_party( AVStreams::StreamEndPoint_ptr the_ep, const AVStreams::flowSpec& the_spec )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::streamOpFailed,
    ::AVStreams::noSuchFlow)

{
  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

}


void
StreamCtrl_impl::unbind()
  throw(
    ::CORBA::SystemException,
    ::AVStreams::streamOpFailed)

{
  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

}

