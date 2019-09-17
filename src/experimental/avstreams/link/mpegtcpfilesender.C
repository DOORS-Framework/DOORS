#include <doors/mpegtcpfilesender.h>

int
MpegTCPFileSender::mpegaudio_get_buffer(unsigned char*& buffer, int size,
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

