
#include <doors/doorsconf.h>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <netdb.h>
#include <doors/filesender.h>
#include <doors/mpegudpfilesender.h>
#include <doors/mpegtcpfilesender.h>
#include <doors/mpegudpreceiver.h>
#include <doors/MpegAudioCodec_impl.h>
#include <doors/mpegframebuf.h>
#include <doors/Audio_impl.h>

#include <SDL/SDL.h>

// MpegAudioSink audio headers and stuff
#if defined (HAVE_LINUX_SOUNDCARD_H)
#define AUDIO_DEVICE "/dev/dsp"
#include <linux/soundcard.h>
#endif
#if defined (HAVE_MACHINE_SOUNDCARD_H)
#define AUDIO_DEVICE "/dev/dsp"
#include <machine/soundcard.h>
#endif
#if defined (HAVE_SOUNDCARD_H)
#define AUDIO_DEVICE "/dev/audio"
#include <soundcard.h>
#endif
#if defined (HAVE_SYS_AUDIOIO_H)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/audioio.h>
#include <stropts.h>
#define AUDIO_DEVICE "/dev/audio"
#endif

// MpegAudioSource stuff
static pthread_mutex_t _sender_mutex;   // Mutex for _sender
static pthread_t       _thr;     // Thread handle
static std::string     _address; // Calling address
static MpegAudioCodec_impl* _codec;
static int _initcodec = TRUE;

// MpegAudioSink stuff
static pthread_mutex_t _receiver_mutex;   // Mutex for _sender
static pthread_t       _thr_receiver;     // Thread handle
static pthread_t       _thr_player;    
static bool            _running_receiver = FALSE; // Rec thread is running?
static bool            _running_player = FALSE;   // Player thread ..
static int             _port = 0;
static MpegFrameBuf*   _framebuf = NULL;
static int             _speed = 0;
static int             _stereo = 0;

// MpegAudioSink functions
static int open_dsp();
static int set_audio_parameters(int stereo, int speed);
static int sync_audio(int fd);
static void* receiver_thread(void* arg);

#if defined (HAVE_LINUX_SOUNDCARD_H) || \
    defined (HAVE_MACHINE_SOUNDCARD_H) || \
    defined(HAVE_SOUNDCARD_H)

static int
open_dsp()
{
  return open(AUDIO_DEVICE, O_WRONLY, 0);
}

static int
set_audio_parameters(int fd, int stereo, int speed)
{
  int format, stereo0, speed0;

  sync_audio(fd);

  /* Set format */
  format = 16;
  if (ioctl(fd, SNDCTL_DSP_SETFMT, &format) == -1) {
    return 1;
  }

  if (format != 16) {
    cerr << "Error: Soundcard doesn't support requested sample size." << endl;
    return 1;
  }

  /* Set stereo mode */
  stereo0 = stereo;
  if (ioctl(fd, SNDCTL_DSP_STEREO, &stereo0) == -1) {
    return 1;
  }

  if (stereo0 != stereo) {
    if (stereo) {
      cerr << "Error: Soundcard doesn't support stereo." << endl;
    } else {
      cerr << "Error: Mono requested but stereo result. Weird!" << endl;
    }
    return 1;
  }

  /* Set sampling speed */
  speed0 = speed;
  if (ioctl(fd, SNDCTL_DSP_SPEED, &speed0) == -1) {
    return 1;
  }

  if (speed0 != speed) {
    cerr << "Warning: Soundcard doesn't support requested speed." << endl;
    cerr << "  Requested: " << speed << " Hz" << endl;
    cerr << "     Result: " << speed0 << " Hz" << endl;
  }

  sync_audio(fd);

  cerr << "Sound parameters: " << speed0 << " Hz, ";
  cerr << (char*)(stereo0 ? "stereo, " : "mono, ");
  cerr << format << " bits." << endl;

  return 0;
}

static int
sync_audio(int fd)
{
  if (ioctl(fd, SNDCTL_DSP_SYNC, NULL) < 0) {
    return 1;
  }

  return 0;
}

#elif defined (HAVE_SYS_AUDIOIO_H)
// Sun/Solaris audio

static int
open_dsp()
{
  return open(AUDIO_DEVICE, O_WRONLY);
}

static int
set_audio_parameters(int fd, int stereo, int speed) 
{ 
  audio_info_t auinfo;

#ifdef AUDIO_INITINFO
  AUDIO_INITINFO(&auinfo);
#else
  if (ioctl(fd, AUDIO_GETINFO, &auinfo) < 0) {
    return false;
  }
#endif

  auinfo.play.precision = 16;
  auinfo.play.encoding = 3/*linear bits*/;
  auinfo.play.channels = stereo ? 2 : 1;
  auinfo.play.sample_rate = speed;

  if (ioctl(fd, AUDIO_SETINFO, &auinfo) < 0) {
    return false;
  }

  return true;
}

static int
sync_audio(int fd) 
{ 
  if (ioctl(fd, AUDIO_DRAIN, NULL) < 0) {
    return false; 
  }

  return true;
}
#endif

void 
fillerup(void *unused, Uint8 *stream, int len)
{
  short fbuf[MFB_BUFSIZE];
  long counter;
  ItemProps props;
  int samples;

  samples = _framebuf->get(counter, fbuf, props);
  if (samples < 1152) {
    // Empty buffer received, do nothing
    return;
  }
  SDL_MixAudio(stream, (const unsigned char*)fbuf, 2304, SDL_MIX_MAXVOLUME);
  if (len == 4608) {
    // len is 1152 samples * 2 channels * 2 bytes (stereo)
    samples = _framebuf->get(counter, fbuf, props);
    // NOTE: Falling thru here even if too few samples received
    //       resulting first half of the stereo samples playing twice
    SDL_MixAudio(stream + 2304, (const unsigned char*)fbuf, 2304, SDL_MIX_MAXVOLUME);
  }
}

static void*
player_thread(void* arg)
{
  int fd = open_dsp();

  pthread_mutex_lock(&_receiver_mutex);
  _running_player = TRUE;
  pthread_mutex_unlock(&_receiver_mutex);

  int stereo0 = 1;
  int speed0 = 44100;
  int stereo = -1;
  int speed = -1;
  int bytes;
  int running;
  for ( ; ; ) {
    pthread_mutex_lock(&_receiver_mutex);
    running = _running_player;
    stereo0 = _stereo;
    speed0 = _speed;
    pthread_mutex_unlock(&_receiver_mutex);
    
    if (!running) {
      break;
    }

    if (fd < 0) {
      fd = open_dsp();
      if (fd < 0) {
	usleep(20000);
	continue;
      }
    }

    long counter;
    short fbuf0[MFB_BUFSIZE], fbuf1[MFB_BUFSIZE];
    ItemProps props;
    long bytes;
    for ( ; ; ) {
      bytes = _framebuf->get(counter, fbuf0, props);
      // Break the loop only if this is the first channel
      if (props.channel == 0) {
	break;
      }
    }
    int stereo0 = props.stereo;
    int speed0  = props.speed;
    int channel0 = props.channel;

    if ((stereo0 != stereo) || (speed0 != speed)) {
    std::cout << bytes << "," << stereo0 << "," << speed0 << ",";
      set_audio_parameters(fd, stereo0, speed0);
      stereo = stereo0; 
      speed  = speed0;
    }

    if (bytes > 0) {
      if (stereo) {
	// This is stereo, read the other channel and write to dsp
	bytes = _framebuf->get(counter, fbuf1, props);
	for (register int j = 0; j < bytes; j++) {
	  unsigned short tmp0 = fbuf0[j];
	  unsigned short tmp1 = fbuf1[j];
	  write(fd, &tmp0, 2);
	  write(fd, &tmp1, 2);
	}
      } else {
	// This is mono, write first channel to dsp
	write(fd, fbuf0, 2 * bytes);
      }
    }
  }

  pthread_exit(NULL);

  return NULL;
}

    
static void*
receiver_thread(void* arg)
{
  MpegAudioCodec_impl* codec = new MpegAudioCodec_impl;
  Unencoded unenc;
  Encoded enc;

  MpegUDPReceiver* receiver = (MpegUDPReceiver*)arg;

  // First wait for connection from sender
  char address[64];
  sprintf(address, ":%d", _port);

  if (!receiver->rcvr_listen(address)) {
    cerr << "rcvr_listen: Connection failed!" << endl;
    pthread_exit(NULL);
    return NULL;
  }

  pthread_mutex_lock(&_receiver_mutex);
  _running_receiver = TRUE;
  pthread_mutex_unlock(&_receiver_mutex);

  // Initialise SDL
  SDL_Init(SDL_INIT_AUDIO);
  SDL_InitSubSystem(SDL_INIT_AUDIO);

  // Initialise audio player
  SDL_AudioSpec desired;
  desired.channels = 2;
  desired.freq = 44100;
  desired.format = AUDIO_S16SYS;
  desired.samples = 1152;
  desired.callback = fillerup;
  desired.userdata = NULL;

  // Open audio device
  if (SDL_OpenAudio(&desired, NULL) < 0) {
    std::cerr << "Could not open audio: " << SDL_GetError() << std::endl;
  }

  // Release audio
  SDL_PauseAudio(0);

  // Connection is ok, start player loop
  mpeg_header header;
  CORBA::Any feed;
  CORBA::Any* result;
  int running;
  for ( ; ; ) {
    pthread_mutex_lock(&_receiver_mutex);
    running = _running_receiver;
    pthread_mutex_unlock(&_receiver_mutex);
    
    if (!running) {
      break;
    }

    // Take a new frame
    unsigned char* buffer;
    int bytes = receiver->mpegaudio_get_frame(buffer);
    // If no bytes received, take a nap and try again
    if (bytes <= 0) {
      usleep(20000);
      continue;
    }

    header = receiver->mpegaudio_get_header();
    int stereo = header.mode == 3 ? 0 : 1; // because mode 3 is mono
    int speed  = header.sampling_freq;
    ItemProps props(stereo, speed, 0);
    enc.length = bytes;
    memcpy(enc.buffer, buffer, bytes);
    feed <<= enc;
    codec->decode(feed, result);
    *result >>= unenc;
    
    short fbuf[MFB_BUFSIZE];
#if 1
    if (!stereo) {
      memcpy(fbuf, unenc.buffer[0], unenc.samples * sizeof(short));
      _framebuf->insert_back(fbuf, unenc.samples, props);
    } else {
      int i;
      int j = 0;
      for (i = 0; i < unenc.samples / 2 - 1; i++, j += 2) {
	fbuf[j]     = unenc.buffer[0][i];
	fbuf[j + 1] = unenc.buffer[1][i];
      }
      _framebuf->insert_back(fbuf, unenc.samples, props);
      j = 0;
      for (i = unenc.samples / 2; i < unenc.samples; i++, j += 2) {
	fbuf[j]     = unenc.buffer[0][i];
	fbuf[j + 1] = unenc.buffer[1][i];
      }
      _framebuf->insert_back(fbuf, unenc.samples, props);
    }
#else
    memcpy(fbuf, unenc.buffer[0], unenc.samples * sizeof(short));
    _framebuf->insert_back(fbuf, unenc.samples, props);
#if 0
    if (stereo) {
      memcpy(fbuf, unenc.buffer[1], unenc.samples * sizeof(short));
      props.channel = 1;
      _framebuf->insert_back(fbuf, unenc.samples, props);
    }
#endif
#endif

    delete result;
  }

  delete codec;
  
  SDL_CloseAudio();
  SDL_Quit();

  pthread_exit(NULL);

  return NULL;
}

static int
reencode_frame(unsigned char* buffer, int bytes)
{
  Encoded inframe, outframe;
  CORBA::Any feed;
  CORBA::Any *result;
  pthread_mutex_lock(&_sender_mutex);
  if (_initcodec) {
    pthread_mutex_unlock(&_sender_mutex);
    EncodingParameters param;
    param.version = MPEG1;
    param.layer = 3;
    param.bitrate = 112;
    param.sfreq = 44100;
    param.mode = joint_stereo;
    param.fast_mode = TRUE;
    param.hi_quality = FALSE;
    param.voice_mode = FALSE;
    param.vbr = FALSE;
    _codec->init_encoding(param);
    pthread_mutex_lock(&_sender_mutex);
    _initcodec = FALSE;
  }
  pthread_mutex_unlock(&_sender_mutex);

  inframe.length = bytes;
  memcpy(inframe.buffer, buffer, bytes);
  feed <<= inframe;

  _codec->reencode(feed, result);

  *result >>= outframe;
  delete result;

  if ((outframe.length > 0) && (outframe.length < MAX_MPEG_BUFFER)) {
    memcpy(buffer, outframe.buffer, outframe.length);
    return outframe.length;
  }

  return bytes;
}

static void*
sender_thread(void* arg)
{
  MpegFrameBuf* framebuf = new MpegFrameBuf(100, FALSE, FALSE);
  MpegUDPFileSender* sender = (MpegUDPFileSender*)arg;
  unsigned char* buffer;
  int bytes;

  // Try to open and if no success, kill this thread
  if (!sender->sndr_open(_address.c_str())) {
    pthread_exit(NULL);
    return NULL;
  }

  for ( ; ; ) {
    pthread_mutex_lock(&_sender_mutex);
    // If connection is closed, break the loop
    if (!sender->sndr_is_open()) {
      pthread_mutex_unlock(&_sender_mutex);
      break;
    }
    // If it is time to nap, take a nap and continue loop
    if (sender->sndr_is_naptime()) {
      pthread_mutex_unlock(&_sender_mutex);
      usleep(20000);
      continue;
    }
    bytes = sender->mpegaudio_get_frame(buffer);
    pthread_mutex_unlock(&_sender_mutex);

    if (bytes <= 0) {
      continue;
    }

    //    bytes = reencode_frame(buffer, bytes);

    short fbuf[MFB_BUFSIZE];
    memcpy(fbuf, buffer, bytes);
    ItemProps props;
    framebuf->insert_back(fbuf, bytes, props);
    sender->sndr_send_frame(buffer, bytes);
  }

  pthread_exit(NULL);

  return NULL;
}

// Implementation for interface MpegAudioSource

MpegAudioSource_impl::MpegAudioSource_impl()
  : _destroyed(true), _sender(0), _rootdir(".")
{
  _codec = new MpegAudioCodec_impl;
  _playlist = new PlayList;
  pthread_mutex_init(&_sender_mutex, NULL);
}

MpegAudioSource_impl::MpegAudioSource_impl(char* rootdir)
  : _destroyed(true), _sender(0), _rootdir(rootdir)
{
  _codec = new MpegAudioCodec_impl;
  _playlist = new PlayList;
  pthread_mutex_init(&_sender_mutex, NULL);
}

MpegAudioSource_impl::~MpegAudioSource_impl()
{
  delete _playlist;
  delete _codec;

  // Close connection and kill the thread
  this->_destroy();

  if (_sender) {
    delete _sender;
  }
  pthread_mutex_destroy(&_sender_mutex);
}

CORBA::Boolean
MpegAudioSource_impl::request_connection( AVStreams::StreamEndPoint_ptr initiator, CORBA::Boolean is_mcast, AVStreams::streamQoS& qos, AVStreams::flowSpec& the_spec )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::streamOpDenied,
    ::AVStreams::noSuchFlow,
    ::AVStreams::QoSRequestFailed,
    ::AVStreams::FPError)

{
  std::cout << "MpegAudioSource_impl::request_connection" << std::endl;
  CORBA::ULong len = the_spec.length();
  if (len == 0) { // There should be at least 1 stream
    return FALSE;
  }

  _address = the_spec[0];

  std::cout << "address = " << _address << std::endl;
  size_t pos;
  // Assuming best effort (type 0) to be UDP and
  // guaranteed (type 1) to be TCP but that may not be wise
  pos = _address.find("UDP=");
  if (pos != std::string::npos) {
    if (is_mcast) {
      cerr << "UDP multicasting requested." << endl;
    } else {
      cerr << "UDP unicasting requested." << endl;
    }
    _sender = new MpegUDPFileSender;
  } else {
    pos = _address.find("TCP=");
    if (pos != std::string::npos) {
      cerr << "TCP connection requested." << endl;
      //    _sender = new MpegTCPFileSender;
      // Hack: we use UDP only by now
      _sender = new MpegUDPFileSender;
    }
  }
  if (pos == std::string::npos) { //  not found?
    return FALSE;
  }
  _address = _address.substr(pos + 4);
 
  //  this->_destroy();

  _destroyed = false;
  std::cout << "Starting sender thread" << std::endl;
  pthread_create(&_thr, NULL, sender_thread, (void*)_sender);

  return TRUE;
}

void 
MpegAudioSource_impl::disconnect( const AVStreams::flowSpec& the_spec )
  throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow,
        ::AVStreams::streamOpFailed)
{

  // Just kill the sender
  delete _sender;
}


void
MpegAudioSource_impl::stop( const AVStreams::flowSpec& the_spec )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::noSuchFlow)

{
  pthread_mutex_lock(&_sender_mutex);
  if (_sender) {
    _sender->sndr_stop();
    cerr << "Stopping." << endl;
  }
  pthread_mutex_unlock(&_sender_mutex);
}


void
MpegAudioSource_impl::start( const AVStreams::flowSpec& the_spec )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::noSuchFlow)

{
  pthread_mutex_lock(&_sender_mutex);
  if (_sender) {
    _sender->sndr_start();
    cerr << "Playing." << endl;
  }
  pthread_mutex_unlock(&_sender_mutex);
}


void
MpegAudioSource_impl::destroy( const AVStreams::flowSpec& the_spec )
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
MpegAudioSource_impl::rewind( const AVStreams::flowSpec& the_spec )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::noSuchFlow,
    ::AVStreams::notSupported)

{
  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

}


void
MpegAudioSource_impl::reposition( const AVStreams::flowSpec& the_spec, const Position& pos )
  throw(
    ::CORBA::SystemException,
    ::InvalidPosition,
    ::AVStreams::notSupported)

{
  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

}


void
MpegAudioSource_impl::resend( const AVStreams::flowSpec& the_spec, CORBA::Long packetnum )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::noSuchFlow,
    ::AVStreams::notSupported)

{
  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

}


CORBA::Boolean
MpegAudioSource_impl::get_list( PlayList_out playlist )
  throw(
    ::CORBA::SystemException)

{
  ls();
  playlist = new PlayList;
  int len = _playlist->length();
  playlist->length(len);
  for (register int i = 0; i < len; i++) {
    (*playlist)[i] = CORBA::string_dup((*_playlist)[i]);
  }
  return TRUE;
}


CORBA::Boolean
MpegAudioSource_impl::get_item( const char* item, PlayItem_out info )
  throw(
    ::CORBA::SystemException)

{
  CORBA::Boolean retval;

  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

  return retval; 
}


void
MpegAudioSource_impl::select_item( const char* item )
  throw(
    ::CORBA::SystemException,
    ::ItemNotFound)

{
  bool opened = false;
  std::string f = "file:";
  f += _rootdir;
  f += "/";
  pthread_mutex_lock(&_sender_mutex);
  if (_sender) {
    if (strlen(item) == 0) {
      f += (char*)(*_playlist)[0];
      opened = _sender->sndr_reopen(f);
    } else {
      int index = atoi(item);
      if ((index > _playlist->length()) || (index < 1)) {
	f += (char*)(*_playlist)[0];
	opened = _sender->sndr_reopen(f);
      } else {
	f += (char*)(*_playlist)[index - 1];
	opened = _sender->sndr_reopen(f);
      }
    }
  }
  pthread_mutex_unlock(&_sender_mutex);
  std::cerr << "Selecting: " << f << std::endl;
  std::cerr << "File is " << (char*)(opened ? "open." : "not open.");
  std::cerr << std::endl;
}

void 
MpegAudioSource_impl::_destroy()
{
  if (_destroyed) {
    return;
  }

  cerr << "Closing sender." << endl;
  pthread_mutex_lock(&_sender_mutex);
  if (_sender) {
    _sender->sndr_close();
  }
  pthread_mutex_unlock(&_sender_mutex);

  // Wait for the thread to stop
  pthread_join(_thr, NULL);

  delete _sender;

  _destroyed = true;

}

CORBA::Boolean
MpegAudioSource_impl::ls()
{
  DIR* dir;
  struct dirent *dp;

  dir = opendir(_rootdir.c_str());
  if (!dir) {
    return FALSE;
  }

  // Count first to get length for playlist
  int len = 0;
  for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) 
    { 
      if (strstr(dp->d_name, ".mp3") || strstr(dp->d_name, ".MP3"))
	{ len++; } 
    }

  if (len <= 0) {
    return FALSE;
  }

  // Set length
  _playlist->length(len);

  rewinddir(dir);
  dp = readdir(dir);
  for (register int i = 0; i < len && dp != NULL; ) {
    if (strstr(dp->d_name, ".mp3") || strstr(dp->d_name, ".MP3")) {
      (*_playlist)[i] = CORBA::string_dup(dp->d_name);
      i++;
    }
    dp = readdir(dir);
  }

  return TRUE;
}

// Implementation for interface MpegAudioSink

MpegAudioSink_impl::MpegAudioSink_impl()
  : _connected(false), _receiver(0), _service_type(0), _multicasting(FALSE)
{ 
  _framebuf = NULL;
  _receiver = NULL;
}

MpegAudioSink_impl::~MpegAudioSink_impl()
{
  
  // Set receiver thread to stop
  pthread_mutex_lock(&_receiver_mutex);
  _running_receiver = 0;
  pthread_mutex_unlock(&_receiver_mutex);
  // Wait it to stop
  pthread_join(_thr_receiver, NULL);

#if 0
  // Set player thread to stop
  pthread_mutex_lock(&_receiver_mutex);
  _running_player = 0;
  pthread_mutex_unlock(&_receiver_mutex);
  // Wait it to stop
  pthread_join(_thr_player, NULL);
#endif

  delete _framebuf;
  delete _receiver;
}

CORBA::Boolean 
MpegAudioSink_impl::preconnect()
      throw(
        ::CORBA::SystemException)
{
  _running_player = 0;
  _running_receiver = 0;
  pthread_create(&_thr_receiver, NULL, receiver_thread, (void*)_receiver);
  //  pthread_create(&_thr_player, NULL, player_thread, (void*)0);
  
  
  // Allow some time for receiver to wake up (1sec)
  //usleep(1000000);

  return TRUE;
}

CORBA::Boolean 
MpegAudioSink_impl::postconnect()
      throw(
        ::CORBA::SystemException)
{
  // Check _running-flag 50 times
  int running;
  for (register int i = 0; i < 50; i++) {
    pthread_mutex_lock(&_receiver_mutex);
    running = _running_receiver;
    pthread_mutex_unlock(&_receiver_mutex);
    if (running) {
      cerr << "Receiver is running." << endl;
      return TRUE;
      break;
    }
    usleep(100000);
  }

  return FALSE;
}


void
MpegAudioSink_impl::parse_qos(AVStreams::streamQoS& qos)
{
  register int i, j;
  int len = qos.length();
  if (len == 0) {
    return;
  }
  for (i = 0; i < len; i++) {
    string type = (char*)qos[i].QoSType;
    if (type.find("audio_QoS") == 0) {
      int params = qos[i].QoSParams.length();
      if (params > 0) {
	for (j = 0; j < params; j++) {
	  string param = (char*)qos[i].QoSParams[j].property_name;
	  CORBA::Any value = qos[i].QoSParams[j].property_value;
	  if (param.find("ServiceType") == 0) {
	    value >>= _service_type;
	  } else if (param.find("Multicast") == 0) {
	    value >>= CORBA::Any::to_boolean(_multicasting);
	  }
	}
      }
      break;
    }
  }

}

CORBA::Boolean 
MpegAudioSink_impl::connect( AVStreams::StreamEndPoint_ptr responder, 
		    AVStreams::streamQoS& qos_spec,
		    const AVStreams::flowSpec& the_spec )
  throw(
	::CORBA::SystemException,
	::AVStreams::noSuchFlow,
	::AVStreams::QoSRequestFailed,
	::AVStreams::streamOpFailed)
{
  char flowspec[256];
  char hostname[256];

  std::cout << "MpegAudioSink::connect" << std::endl;
  srand(time(NULL));
  _port = 5000 + rand() % 50000; // Get some port

  parse_qos(qos_spec);

#if 0
  if (_connected) {
    std::cout << "disconnect()" << std::endl;
    disconnect(the_spec);
  }
#endif

  if (_multicasting) {
    sprintf(hostname, "239.11.22.33");
    _port = 8888;
  } else {
    if (!get_fqhn(hostname)) {
      return FALSE;
    }
  }

  std::cout << "hostname = " << hostname << std::endl;
  _framebuf = new MpegFrameBuf(100);
#if 0
  if (_service_type == 0) {
    _receiver = new MpegUDPReceiver;
    sprintf(flowspec, "audio\\UDP=%s:%d", hostname, _port);
  } else {
    _receiver = new MpegTCPReceiver;
    sprintf(flowspec, "audio\\TCP=%s:%d", hostname, _port);
  }
#endif

  _receiver = new MpegUDPReceiver;
  sprintf(flowspec, "audio\\UDP=%s:%d", hostname, _port);
  std::cout << "receiver started" << std::endl;

  if (!preconnect()) {
    std::cout << "preconnect() failed" << std::endl;
    return FALSE;
  }

  std::cout << "flowSpec = " << flowspec << std::endl;

  AVStreams::flowSpec* spec = new AVStreams::flowSpec;
  spec->length(1);
  (*spec)[0] = CORBA::string_dup(flowspec);
  AVStreams::streamQoS qos;
  std::cout << "Sending request_connection()" << std::endl << std::flush;
  responder->request_connection(_this(), _multicasting, qos, *spec);

  if (!postconnect()) {
    return FALSE;
  }
  cerr << "connected" << endl;
  _connected = true;

  return TRUE;
}

void 
MpegAudioSink_impl::disconnect( const AVStreams::flowSpec& the_spec )
      throw(
        ::CORBA::SystemException,
        ::AVStreams::noSuchFlow,
        ::AVStreams::streamOpFailed)
{
  std::cout << "disconnect()..." << std::flush;

  // Set receiver thread to stop
  pthread_mutex_lock(&_receiver_mutex);
  _running_receiver = 0;
  pthread_mutex_unlock(&_receiver_mutex);
  // Wait it to stop
  pthread_join(_thr_receiver, NULL);

#if 0
  // Set player thread to stop
  pthread_mutex_lock(&_receiver_mutex);
  _running_player = 0;
  pthread_mutex_unlock(&_receiver_mutex);
  // Wait it to stop
  pthread_join(_thr_player, NULL);
#endif

  delete _receiver;
  delete _framebuf;

  std::cout << "ok." << std::endl << std::flush;

  _connected = false;
}

CORBA::Boolean
MpegAudioSink_impl::get_fqhn(char* hostname)
{
  gethostname(hostname, 256);
  if (hostname == NULL) {
    return FALSE;
  }

  struct hostent* ent = gethostbyname(hostname);
  if (ent == NULL) {
    return TRUE; // Cannot resolve address, return only hostname
  }

  // Copy fully qualified hostname to string
  strcpy(hostname, ent->h_name);

  return TRUE;
}


// Implementation for interface MpegAudioCtrl

CORBA::Boolean
MpegAudioCtrl_impl::bind_mpegaudio( MpegAudioSource_ptr a_party, 
				    MpegAudioSink_ptr b_party, 
				    AVStreams::streamQoS& the_qos, 
				    const AVStreams::flowSpec& the_flows )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::streamOpFailed,
    ::AVStreams::noSuchFlow,
    ::AVStreams::QoSRequestFailed)

{
  PlayList* playlist = NULL;
  _source    = a_party;
  _sink      = b_party;
  _the_flows = the_flows;
  std::cout << "bind_mpegaudio" << std::endl << std::flush;
  return _sink->connect(_source, the_qos, the_flows);
}


void
MpegAudioCtrl_impl::unbind_mpegaudio()
  throw(
    ::CORBA::SystemException,
    ::AVStreams::streamOpFailed)

{
  std::cout << "unbind_mpegaudio" << std::endl << std::flush;
  return _sink->disconnect(_the_flows);
}

void
MpegAudioCtrl_impl::stop( const AVStreams::flowSpec& the_spec )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::noSuchFlow)

{
  _source->stop(the_spec);
}


void
MpegAudioCtrl_impl::start( const AVStreams::flowSpec& the_spec )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::noSuchFlow)

{
  _source->start(the_spec);
}


void
MpegAudioCtrl_impl::destroy( const AVStreams::flowSpec& the_spec )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::noSuchFlow)

{
  _source->destroy(the_spec);
}


void
MpegAudioCtrl_impl::rewind( const AVStreams::flowSpec& the_spec )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::noSuchFlow,
    ::AVStreams::notSupported)

{
  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

}


void
MpegAudioCtrl_impl::resend( const AVStreams::flowSpec& the_spec, CORBA::Long packetnum )
  throw(
    ::CORBA::SystemException,
    ::AVStreams::noSuchFlow,
    ::AVStreams::notSupported)

{
  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

}


void
MpegAudioCtrl_impl::reposition( const AVStreams::flowSpec& the_spec, const Position& pos )
  throw(
    ::CORBA::SystemException,
    ::InvalidPosition,
    ::AVStreams::notSupported)

{
  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

}


CORBA::Boolean
MpegAudioCtrl_impl::get_list( PlayList_out playlist )
  throw(
    ::CORBA::SystemException)

{
  return _source->get_list(playlist);
}


CORBA::Boolean
MpegAudioCtrl_impl::get_item( const char* item, PlayItem_out info )
  throw(
    ::CORBA::SystemException)

{
  CORBA::Boolean retval;

  // add your implementation here
    // REMOVE  
    mico_throw(::CORBA::NO_IMPLEMENT());
    // REMOVE 

  return retval; 
}


void
MpegAudioCtrl_impl::select_item( const char* item )
  throw(
    ::CORBA::SystemException,
    ::ItemNotFound)

{
  _source->select_item(item);
}



