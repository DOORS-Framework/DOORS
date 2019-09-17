#ifndef MPEGUDPRECEIVER_H
#define MPEGUDPRECEIVER_H

#include <doors/mpegaudio.h>
#include <doors/udpreceiver.h>

class MpegUDPReceiver : public MPEGAudio,
			public UDPReceiver
{
public:
  MpegUDPReceiver() { }

  virtual int parse_rtp_header(unsigned char* buffer, int size);

  virtual int mpegaudio_get_buffer(unsigned char*& buffer, int size,
				   int offset)
    { return rcvr_get_buffer(buffer, size, offset); }

 private:
  unsigned short _sequence_number;
  unsigned long _timestamp;
  unsigned long _ssrc;

  unsigned long get_uint32(unsigned char* buffer, int offset);
  unsigned short get_uint16(unsigned char* buffer, int offset);
};

#endif
  
