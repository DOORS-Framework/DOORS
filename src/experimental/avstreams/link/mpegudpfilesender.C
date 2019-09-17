#include <string.h>
#include <stdio.h>
#include <doors/mpegudpfilesender.h>

MpegUDPFileSender::MpegUDPFileSender(std::string filename) 
  : FileSender(filename)
{
  _timestamp = rand();
  _ssrc = rand();
  _sequence_number = (unsigned short)(rand() % 65536);
}

MpegUDPFileSender::MpegUDPFileSender(int fd) 
  : FileSender(fd)
{
  _timestamp = rand();
  _ssrc = rand();
  _sequence_number = (unsigned short)(rand() % 65536);
}

MpegUDPFileSender::MpegUDPFileSender() 
  : FileSender()
{
  _timestamp = rand();
  _ssrc = rand();
  _sequence_number = (unsigned short)(rand() % 65536);
}

void 
MpegUDPFileSender::put_uint32(unsigned char* buffer, unsigned long value)
{
  buffer[0] = (unsigned char)(value>>24&0xFF);
  buffer[1] = (unsigned char)(value>>16&0xFF);
  buffer[2] = (unsigned char)(value>> 8&0xFF);
  buffer[3] = (unsigned char)(value    &0xFF);
}

void 
MpegUDPFileSender::put_uint16(unsigned char* buffer, unsigned short value)
{
  buffer[0] = (unsigned char)(value>>8&0xFF);
  buffer[1] = (unsigned char)(value   &0xFF);
}

int
MpegUDPFileSender::write_rtp_header(unsigned char* buffer)
{
    unsigned long tmp32;
    register int i;
    const int marker = 0;
    const int payload_type = 14;
    unsigned char tmp;

    // Add version. Padding, extension and CSRC count are zero
    tmp = (unsigned char)(2<<6);
    // Write to the frame
    buffer[0] = tmp;

    // Add marker bit
    tmp = (unsigned char)((marker&1)<<7);
    // Add payload type
    tmp |= payload_type&0x7F;
    // Write to the frame
    buffer[1] = tmp;

    // Write sequence#
    put_uint16(buffer + 2, _sequence_number++);

    // Write timestamp
    put_uint32(buffer + 4, _timestamp);
    mpeg_header header = mpegaudio_get_header();
   _timestamp += header.sampling_freq;

    // Write SSRC
    put_uint32(buffer + 8, _ssrc);

    // Write MPEG specific header, zeros only for now
    //    put_uint32(buffer + 12, 0);
    put_uint32(buffer + 12, 0x444F5253);
    // Return RTP header size
    return 16;
}

int
MpegUDPFileSender::mpegaudio_get_buffer(unsigned char*& buffer, int size,
				     int offset)
{
  if (!inputstream_.is_open()) {
    return 0;
  }

  if ((inputstream_.eof()) || (!inputstream_.good())) {
    inputstream_.close();
    return 0;
  }
  char* p = (char*)&buffer[offset];
  inputstream_.read(p, size);
  return inputstream_.gcount();
}

