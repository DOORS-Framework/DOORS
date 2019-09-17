#ifndef MPEGTCPRECEIVER_H
#define MPEGTCPRECEIVER_H

#include <doors/mpegaudio.h>
#include <doors/tcpreceiver.h>

class MpegTCPReceiver : public MPEGAudio,
			public TCPReceiver
{
public:
  MpegTCPReceiver() { }

  virtual int mpegaudio_get_buffer(unsigned char*& buffer, int size,
				   int offset)
    { rcvr_get_buffer(buffer, size, offset); }

};

#endif
  
