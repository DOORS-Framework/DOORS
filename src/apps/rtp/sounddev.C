//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
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

$Log: sounddev.C,v $
Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.2  2001/06/08 16:46:52  bilhanan
Full Licence clause into DOORS 0.2


*/

#include <doors/sounddev.h>

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <iostream.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

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
#include <sys/audioio.h>
#include <stropts.h>
#define AUDIO_DEVICE "/dev/audio"
#endif

#if defined (HAVE_LINUX_SOUNDCARD_H) || defined (HAVE_MACHINE_SOUNDCARD_H) || defined(HAVE_SOUNDCARD_H)

static int _fd;

SoundDev::SoundDev()
  : _burstsize(0)
{
  _fd = -1;
}

SoundDev::~SoundDev()
{
  if (_fd >= 0) {
    close(_fd);
  }
}

bool
SoundDev::open_audio()
{
  if (_fd >= 0) { // Already open?
    return true;
  }
  _fd = open(AUDIO_DEVICE, O_WRONLY, 0);
  if (_fd < 0) {
    cerr << "Error: Cannot open audio device." << endl;
    return false;
  }

  // Get maximum sound block size
  ioctl(_fd, SNDCTL_DSP_GETBLKSIZE, &_burstsize);
  if(_burstsize > MAXBURSTSIZE) {
    _burstsize = MAXBURSTSIZE;
  }

  return true;
}

bool
SoundDev::close_audio()
{
  if (_fd < 0) { // Already closed?
    return true;
  }
  if (close(_fd)) {
    return false;
  }
  _fd = -1;
  return true;
}

bool
SoundDev::sync_audio()
{
  if (_fd < 0) { // No audio device?
    return false;
  }
  if (ioctl(_fd, SNDCTL_DSP_SYNC, NULL) < 0) {
    return false;
  }
  return true;
}

bool
SoundDev::reset_audio()
{
  if (_fd < 0) { // No audio device?
    return false;
  }
  if (ioctl(_fd, SNDCTL_DSP_RESET, NULL) < 0) {
    return false;
  }
  return true;
}

bool
SoundDev::set_audio_parameters(int stereo, int speed, int bits)
{
  int format, stereo0, speed0;

  if (_fd < 0) { // Not opened?
    return false;
  }

  reset_audio(); 
  // These syncs were removed because mpg123 uses RESET
  // Let us see which way is better...
  //sync_audio();

  // Set format 
  if (bits == 16) {
    format = AFMT_S16_LE;
    if (ioctl(_fd, SNDCTL_DSP_SETFMT, &format) == -1) {
      cerr << "Setting sample size failed." << endl;
      return false;
    }
    if (format != AFMT_S16_LE) {
      cerr << "Error: Soundcard doesn't support 16 bits sample size.";
      cerr << endl;
      return false;
    }
  } else if (bits == 8) {
    format = AFMT_U8;
    if (ioctl(_fd, SNDCTL_DSP_SETFMT, &format) == -1) {
      cerr << "Setting sample size failed." << endl;
      return false;
    }
    if (format != AFMT_U8) {
      cerr << "Error: Soundcard doesn't support 8 bits sample size.";
      cerr << endl;
      return false;
    }
  } else {
    cerr << "Unsupported bitsize." << endl;
    return false;
  }

  // Set stereo mode 
  stereo0 = stereo;
  if (ioctl(_fd, SNDCTL_DSP_STEREO, &stereo0) == -1) {
    cerr << "Setting stereo mode failed." << endl;
    return false;
  }

  if (stereo0 != stereo) {
    if (stereo) {
      cerr << "Error: Soundcard doesn't support stereo." << endl;
    } else {
      cerr << "Error: Mono requested but stereo result. Weird!" << endl;
    }
    return false;
  }

  // Set sampling speed
  speed0 = speed;
  if (ioctl(_fd, SNDCTL_DSP_SPEED, &speed0) == -1) {
    cerr << "Setting sampling speed failed." << endl;
    return false;
  }

  if (speed0 != speed) {
    cerr << "Warning: Soundcard doesn't support requested speed." << endl;
    cerr << "  Requested: " << speed << " Hz" << endl;
    cerr << "     Result: " << speed0 << " Hz" << endl;
  }

  //sync_audio();

  cerr << "Sound parameters: " << speed0 << " Hz, ";
  cerr << (char*)(stereo0 ? "stereo, " : "mono, ");
  cerr << format << " bits." << endl;

  return true;
}

bool
SoundDev::write_audio(short left, short right) 
{
  if ((write(_fd, &left, 2) == 2) && (write(_fd, &right, 2) == 2)) {
    return true;
  }
  return false;
}


bool
SoundDev::write_audio(char* buffer, int size)
{
  if (_fd < 0) {
    return false;
  }
  if (write(_fd, buffer, size) == size) {
    return true;
  }
  return false;
}

#elif defined (HAVE_SYS_AUDIOIO_H)

static int _fd;

SoundDev::SoundDev()
  : _burstsize(0)
{
  _fd = -1;
}

SoundDev::~SoundDev()
{
  if (_fd >= 0) {
    close(_fd);
  }
}

bool SoundDev::open_audio() 
{
  if ((_fd = open(AUDIO_DEVICE, O_RDWR)) == -1) {
    return false;
  }

  return true;
}

bool SoundDev::close_audio() 
{
  if (_fd != -1) {
    close(_fd);
    _fd = -1;
  }
  return true;
}

bool SoundDev::sync_audio() 
{ 
  if (ioctl(_fd, AUDIO_DRAIN, NULL) < 0) {
    return false; 
  }

  return true;
}

bool SoundDev::reset_audio() 
{ 
  if (ioctl(_fd, I_FLUSH, FLUSHW) < 0) {
    return false;
  }

  return true;
}

bool SoundDev::set_audio_parameters(int stereo, int speed, int bits) 
{ 
  audio_info_t auinfo;

#ifdef AUDIO_INITINFO
  AUDIO_INITINFO(&auinfo);
#else
  if (ioctl(_fd, AUDIO_GETINFO, &auinfo) < 0) {
    return false;
  }
#endif

  auinfo.play.precision = bits;
  auinfo.play.encoding = 3/*linear bits*/;
  auinfo.play.channels = stereo ? 2 : 1;
  auinfo.play.sample_rate = speed;

  if (ioctl(_fd, AUDIO_SETINFO, &auinfo) < 0) {
    return false;
  }

  return true;
}

bool
SoundDev::write_audio(short left, short right) 
{
  if ((write(_fd, &left, 2) == 2) && (write(_fd, &right, 2) == 2)) {
    return true;
  }
  return false;
}


bool
SoundDev::write_audio(char* buffer, int size)
{
  if (_fd < 0) {
    return false;
  }
  if (write(_fd, buffer, size) == size) {
    return true;
  }
  return false;
}

#else // No sound driver; implement dummy functions

SoundDev::SoundDev() { }

SoundDev::~SoundDev() { }

bool SoundDev::open_audio() { return false; }

bool SoundDev::close_audio() { return false; }

bool SoundDev::sync_audio() { return false; }

bool SoundDev::reset_audio() { return false; }

bool SoundDev::set_audio_parameters(int stereo, int speed, int bits) { return false; }

bool SoundDev::write_audio(char* buffer, int size) { return false; }

bool SoundDev::write_audio(short left, short right) { return false; }  


#endif

