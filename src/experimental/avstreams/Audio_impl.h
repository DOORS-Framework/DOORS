
#ifndef __AUDIO_IMPL_H__
#define __AUDIO_IMPL_H__

#include "Audio.h"
#include <string>
#include <doors/mpegudpreceiver.h>
#include <doors/mpegudpfilesender.h>
#include <doors/AVStreams_impl.h>


// Implementation for interface MpegAudioSource
class MpegAudioSource_impl : 
  virtual public StreamEndPoint_A_impl,
  virtual public POA_MpegAudioSource
{
  public:
    
    MpegAudioSource_impl();
    MpegAudioSource_impl(char* rootdir);

    virtual ~MpegAudioSource_impl();

    CORBA::Boolean request_connection( AVStreams::StreamEndPoint_ptr initiator, CORBA::Boolean is_mcast, AVStreams::streamQoS& qos, AVStreams::flowSpec& the_spec )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::streamOpDenied,
        ::AVStreams::noSuchFlow,
        ::AVStreams::QoSRequestFailed,
        ::AVStreams::FPError)
    ;

    void disconnect( const AVStreams::flowSpec& the_spec )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow,
        ::AVStreams::streamOpFailed)
    ;

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

    void rewind( const AVStreams::flowSpec& the_spec )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow,
        ::AVStreams::notSupported)
    ;

    void reposition( const AVStreams::flowSpec& the_spec, const Position& pos )
      throw(
        ::CORBA::SystemException,
        ::InvalidPosition,
        ::AVStreams::notSupported)
    ;

    void resend( const AVStreams::flowSpec& the_spec, CORBA::Long packetnum )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow,
        ::AVStreams::notSupported)
    ;

    CORBA::Boolean get_list( PlayList_out playlist )
      throw(
        ::CORBA::SystemException)
    ;

    CORBA::Boolean get_item( const char* item, PlayItem_out info )
      throw(
        ::CORBA::SystemException)
    ;

    void select_item( const char* item )
      throw(
        ::CORBA::SystemException,
        ::ItemNotFound)
    ;

private:
  PlayList*       _playlist;
  MpegUDPFileSender*         _sender;
  bool            _destroyed;
  std::string     _rootdir;

  void _destroy();
  CORBA::Boolean ls();

};


// Implementation for interface MpegAudioSink
class MpegAudioSink_impl : 
  virtual public StreamEndPoint_B_impl,
  virtual public POA_MpegAudioSink
{
  public:

    MpegAudioSink_impl();
    virtual ~MpegAudioSink_impl();

    CORBA::Boolean connect( AVStreams::StreamEndPoint_ptr responder, AVStreams::streamQoS& qos_spec, const AVStreams::flowSpec& the_spec )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow,
        ::AVStreams::QoSRequestFailed,
        ::AVStreams::streamOpFailed)
    ;

    void disconnect( const AVStreams::flowSpec& the_spec )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow,
        ::AVStreams::streamOpFailed)
    ;

    CORBA::Boolean preconnect()
      throw(
        ::CORBA::SystemException)
    ;

    CORBA::Boolean postconnect()
      throw(
        ::CORBA::SystemException)
    ;

private:
  bool _connected;
  MpegUDPReceiver* _receiver;
  CORBA::Short    _service_type;
  CORBA::Boolean  _multicasting;

  CORBA::Boolean get_fqhn(char* hostname);
  void parse_qos(AVStreams::streamQoS& qos);
};


// Implementation for interface MpegAudioCtrl
class MpegAudioCtrl_impl : 
  virtual public StreamCtrl_impl,
  virtual public POA_MpegAudioCtrl
{
  public:

    CORBA::Boolean bind_mpegaudio( MpegAudioSource_ptr a_party, MpegAudioSink_ptr b_party, AVStreams::streamQoS& the_qos, const AVStreams::flowSpec& the_flows )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::streamOpFailed,
        ::AVStreams::noSuchFlow,
        ::AVStreams::QoSRequestFailed)
    ;

    void unbind_mpegaudio()
      throw(
        ::CORBA::SystemException,
        ::AVStreams::streamOpFailed)
    ;

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

    void rewind( const AVStreams::flowSpec& the_spec )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow,
        ::AVStreams::notSupported)
    ;

    void resend( const AVStreams::flowSpec& the_spec, CORBA::Long packetnum )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow,
        ::AVStreams::notSupported)
    ;

    void reposition( const AVStreams::flowSpec& the_spec, const Position& pos )
      throw(
        ::CORBA::SystemException,
        ::InvalidPosition,
        ::AVStreams::notSupported)
    ;

    CORBA::Boolean get_list( PlayList_out playlist )
      throw(
        ::CORBA::SystemException)
    ;

    CORBA::Boolean get_item( const char* item, PlayItem_out info )
      throw(
        ::CORBA::SystemException)
    ;

    void select_item( const char* item )
      throw(
        ::CORBA::SystemException,
        ::ItemNotFound)
    ;

private:
    MpegAudioSource_ptr _source;
    MpegAudioSink_ptr   _sink;
    AVStreams::flowSpec _the_flows;  
};


#endif
