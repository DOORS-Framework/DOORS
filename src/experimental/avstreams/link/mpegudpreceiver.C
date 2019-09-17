#include <doors/mpegudpreceiver.h>

unsigned long
MpegUDPReceiver::get_uint32(unsigned char* buffer, int offset)
{
  unsigned long tmp32 = ((unsigned long)(buffer[offset] << 24));
  tmp32 |=       ((unsigned long)(buffer[offset + 1] << 16));
  tmp32 |=       ((unsigned long)(buffer[offset + 2] << 8));
  tmp32 |=       ((unsigned long)(buffer[offset + 3]));
  return tmp32;
}

unsigned short 
MpegUDPReceiver::get_uint16(unsigned char* buffer, int offset)
{
  unsigned short tmp16 = ((unsigned short)(buffer[offset] << 8));
  tmp16 |=       ((unsigned short)buffer[offset + 1]);
  return tmp16;
}

int
MpegUDPReceiver::parse_rtp_header(unsigned char* buffer, int size)
{
    register int i;
    unsigned char padding_length;
    int version;
    int padding;
    int extension;
    int csrc_count;
    int marker;
    int payload_type;

    // Get header and check its validity
    // Get first octet
    unsigned char tmp = buffer[0];
    // Check version
    if ((version = tmp>>6) != 2) {
      //   cerr << "Error: Unsupported RTP version (" << (int)version << ").";
      //   cerr << endl;
      return -1;
    }
    // Is there a padding after the payload?
    padding = tmp >> 5 & 1/*bit*/;
    // Is there an extension?
    extension = tmp >> 4 & 1/*bit*/;
    if (extension) {
      //cout << "Error: Extension bit cannot be set in this implementation.";
      //cout << endl;
      return -1;
    }
    // Number of CSRC identifiers
    csrc_count = tmp & 0xF/*4 bits*/;
    if (size < (12 + 4 * csrc_count)) {
      //cout << "Error: Invalid frame length." << endl;
      return -1;
    }
    // Get second octet
    tmp = buffer[1];
    // Frame marker bit
    marker = tmp >> 7 & 1/*bit*/;
    // Payload type
    payload_type = tmp & 0x7F/*7 bits*/;
    if (payload_type != 14) {
      // cerr << "Error: Unknown payload type." << endl;
      return -1;
    }

    // Get third and fourth octet (16 bits)
    // Sequence number
    _sequence_number = get_uint16(buffer, 2);

    // Get 5th to 8th octet (32 bits)
    // Timestamp
    _timestamp = get_uint32(buffer, 4);

    // Get 9th to 12th octet (32 bits)
    // SSRC
    _ssrc = get_uint32(buffer, 8);

    // If padding bit is set, resolve its length (last byte of frame)
    padding_length = 0;
    if (padding) {
      padding_length = buffer[size-1];
      if (padding_length > (size - 12)) {
	//cout << "Error: Invalid padding length." << endl;
	return -1;
      }
    }

    // Return beginning offset of MPEG frame
    return 16 + 4 * csrc_count;
}
  
