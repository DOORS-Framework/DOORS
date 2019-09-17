
#ifndef __AVSTREAMS_IMPL_H__
#define __AVSTREAMS_IMPL_H__

#include "AVStreams.h"


// Implementation for interface Basic_StreamCtrl
class Basic_StreamCtrl_impl : virtual public POA_AVStreams::Basic_StreamCtrl
{
  public:

    CORBA::Boolean modify_QoS( AVStreams::streamQoS& new_qos, const AVStreams::flowSpec& the_spec )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow,
        ::AVStreams::QoSRequestFailed)
    ;

    void push_event( const AVStreams::streamEvent& the_event )
      throw(
        ::CORBA::SystemException)
    ;
};


// Implementation for interface StreamEndPoint
class StreamEndPoint_impl : virtual public POA_AVStreams::StreamEndPoint
{
  public:

    CORBA::Boolean modify_QoS( AVStreams::streamQoS& new_qos, const AVStreams::flowSpec& the_flows )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow,
        ::AVStreams::QoSRequestFailed)
    ;

};


// Implementation for interface StreamEndPoint_A
class StreamEndPoint_A_impl : 
  virtual public StreamEndPoint_impl,
  virtual public POA_AVStreams::StreamEndPoint_A
{
  public:

    CORBA::Boolean connect( AVStreams::StreamEndPoint_ptr responder, AVStreams::streamQoS& qos_spec, const AVStreams::flowSpec& the_spec )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow,
        ::AVStreams::QoSRequestFailed,
        ::AVStreams::streamOpFailed)
    ;

    CORBA::Boolean multiconnect( AVStreams::streamQoS& the_qos, AVStreams::flowSpec& the_spec )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow,
        ::AVStreams::QoSRequestFailed,
        ::AVStreams::streamOpFailed)
    ;
};


// Implementation for interface StreamEndPoint_B
class StreamEndPoint_B_impl : 
  virtual public StreamEndPoint_impl,
  virtual public POA_AVStreams::StreamEndPoint_B
{
  public:

    void stop( const AVStreams::flowSpec& the_spec )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow)
    ;

    void start( const AVStreams::flowSpec& the_spec )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow)
    ;

    void destroy( const AVStreams::flowSpec& the_spec )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow)
    ;

    CORBA::Boolean request_connection( AVStreams::StreamEndPoint_ptr initiator, CORBA::Boolean is_mcast, AVStreams::streamQoS& qos, AVStreams::flowSpec& the_spec )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::streamOpDenied,
        ::AVStreams::noSuchFlow,
        ::AVStreams::QoSRequestFailed,
        ::AVStreams::FPError)
    ;

    CORBA::Boolean multiconnect( AVStreams::streamQoS& the_qos, AVStreams::flowSpec& the_spec )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::streamOpFailed,
        ::AVStreams::noSuchFlow,
        ::AVStreams::QoSRequestFailed,
        ::AVStreams::FPError)
    ;
};


// Implementation for interface StreamCtrl
class StreamCtrl_impl : 
  virtual public Basic_StreamCtrl_impl,
  virtual public POA_AVStreams::StreamCtrl
{
  public:

    CORBA::Boolean bind( AVStreams::StreamEndPoint_A_ptr a_party, AVStreams::StreamEndPoint_B_ptr b_party, AVStreams::streamQoS& the_qos, const AVStreams::flowSpec& the_flows )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::streamOpFailed,
        ::AVStreams::noSuchFlow,
        ::AVStreams::QoSRequestFailed)
    ;

    void unbind_party( AVStreams::StreamEndPoint_ptr the_ep, const AVStreams::flowSpec& the_spec )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::streamOpFailed,
        ::AVStreams::noSuchFlow)
    ;

    void unbind()
      throw(
        ::CORBA::SystemException,
        ::AVStreams::streamOpFailed)
    ;
};


#endif
