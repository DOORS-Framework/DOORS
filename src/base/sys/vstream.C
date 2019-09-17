// 
//  Copyright 1999 
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
 
 
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
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

$Log: vstream.C,v $
Revision 1.8  2003/05/09 10:14:52  bilhanan
comment clarification: string -> charstring.

Revision 1.7  2002/08/14 09:31:42  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.6  2002/07/25 22:41:09  ik
Fixes for std::string changes.

Revision 1.5  2002/07/25 15:38:44  ik
Use std::string.

Revision 1.4  2002/07/25 10:08:40  ik
Use std::string.

Revision 1.3  2002/07/23 14:25:22  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:54  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:31  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:29  bilhanan
Full licence clause added into doors 0.1


*/


#include <string>
#include <string.h>
#include <stdio.h>


#include <doors/vstream.h>


//
//Constructor: VstreamMessage
//
//Commentary:
//       Data is copied.
//
VstreamMessage::VstreamMessage (const VstreamMessage &m) 
  : data (m.data), scope (m.scope), type (m.type), level (m.level)
{
}


//
//Destructor: VstreamMessage
//
VstreamMessage::~VstreamMessage (void)
{
}

//Macro: OCTETS
//Description: The number of octets of the frame printed on one line.
#define OCTETS 10


#if _MT_ENABLED

VostreamProxy::VostreamProxy (Vostream::Type t, Scheduler *s)
    : EventTask (0, s), type (t)
{
}

VostreamProxy :: ~VostreamProxy (void)
{

}


//
//Function: putOutput
//
//Member-Of: VostreamProxy
//
//Description:
//      Attach the given port into the stream.
//
void VostreamProxy::putOutput (Port *port)
{
  Port *p = new Port (this);
  port->connect (p);
  oports.append (p);
  return;
}


typedef bool (*CmpPortProc)(const Port*, const void *);

//
//Function: getOutput
//
//Member-Of: VostreamProxy
//
//Description:
//      Deattach the given port from the stream.
//
void VostreamProxy::getOutput (Port *port)
{
  delete oports.remove ((const void *)port, (CmpPortProc)Port::compareOther);
  return;
}

TQueue<Port> *VostreamProxy::getPorts (void) 
{
  return &oports;
}


//
//Function: propagate
//
//Member-Of: VostreamProxy
//
//Description:
//      Sends the message to every connected port.
//
void VostreamProxy::propagate (VstreamMessage *vmsg)
{
  TQueueListIterator<Port> iter (&oports);
  Port *port;
  int used = 0;
  VstreamMessage *nmsg = vmsg;
  VstreamMessage *tmpMsg = 0;  

  if (iter.head () != iter.last()) {
    tmpMsg = new VstreamMessage (*vmsg);
  }

  while (port = iter.next ()) {
    if (used) {
      nmsg = new VstreamMessage (*tmpMsg);
    } else if (iter.head () == 0) {
      nmsg = tmpMsg;
    }
    
    Message *msg = nmsg;       // CC problems with references
    port->putMessage (msg);
    used = 1;
  }
  if (!used) {
    delete vmsg;
  }
  
  return;
}


//
//Function: operator<<
//
//Member-Of: VostreamProxy
//
//Parameters:
//      VostreamProxy::Type arg
//
//Description:
//      Sets the stream's type with the given type.
//
Vostream &VostreamProxy::operator<< (Vostream::Type arg)
{
  Vostream &stream = getStream ();
  
  stream << arg;

  return stream;
}


//
//Function: operator<<
//
//Member-Of: Vostream
//
//Paramters:
//      VstreamCtrl arg
//
//Description:
//      Sends the given control token to the stream.
//
Vostream &VostreamProxy::operator<< (VstreamCtrl arg)
{
  Vostream &stream = getStream ();
  
  stream << endv;

  return stream;
}


//
//Function: operator<<
//
//Member-Of: VostreamProxy
//
//Paramters:
//      const char c
//
//Description:
//      Sends the given character to the stream.
//
Vostream &VostreamProxy::operator<< (const char c)
{
  Vostream &stream = getStream ();
  
  stream << c;

  return stream;
}

//
//Function: operator<<
//
//Member-Of: VostreamProxy
//
//Paramters:
//      bool
//
//Description:
//      Sends the given boolean value to the stream.
//      Outputs 'true' or 'false'
//
Vostream &VostreamProxy::operator<< (const bool b)
{
  Vostream &stream = getStream ();
  
  stream << b;

  return stream;
}

//
//Function: operator<<
//
//Member-Of: VostreamProxy
//
//Paramters:
//      const unsigned char c
//
//Description:
//      Sends the given character to the stream.
//
Vostream &VostreamProxy::operator<< (const unsigned char c)
{
  Vostream &stream = getStream ();
  
  stream << c;

  return stream;
}

//Operator: <<
//Member-Of: VostreamProxy
//Paramters:
//      const int i
//Description:
//      Sends the given int to the stream.
//
Vostream &VostreamProxy::operator<< (const int i)
{
  Vostream &stream = getStream ();
  
  stream << i;

  return stream;
}

//
//Function: operator<<
//
//Member-Of: VostreamProxy
//
//Paramters:
//      const unsigned int i
//
//Description:
//      Sends the given integer to the stream.
//
Vostream &VostreamProxy::operator<< (const unsigned int i)
{
  Vostream &stream = getStream ();
  
  stream << i;

  return stream;
}

//
//Function: operator<<
//
//Member-Of: VostreamProxy
//
//Paramters:
//      const long i
//
//Description:
//      Sends the given long to the stream.
//
Vostream &VostreamProxy::operator<< (const long i)
{

  Vostream &stream = getStream ();
  
  stream << i;

  return stream;
}

//
//Function: operator<<
//
//Member-Of: VostreamProxy
//
//Paramters:
//      const unsigned long i
//
//Description:
//      Sends the given long integer to the stream.
//
Vostream &VostreamProxy::operator<< (const unsigned long i)
{
  Vostream &stream = getStream ();
  
  stream << i;

  return stream;
}


//
//Function: operator<<
//
//Member-Of: Vostream
//
//Paramters:
//      const double f
//
//Description:
//      Sends the given double to the stream.
//
Vostream &VostreamProxy::operator<< (const double f)
{
  Vostream &stream = getStream ();
  
  stream << f;

  return stream;
}


//
//Function: operator<<
//
//Member-Of: VostreamProxy
//
//Paramters:
//      const double f
//
//Description:
//      Sends the given charstring to the stream.
//
Vostream &VostreamProxy::operator<< (const char *s)
{
  Vostream &stream = getStream ();
  
  stream << s;

  return stream;
}


//
//Function: operator<<
//
//Member-Of: VostreamProxy
//
//Paramters:
//      std::string s
//
//Description:
//      Sends the given string to the stream.
//
Vostream &VostreamProxy::operator<< (std::string s)
{
  Vostream &stream = getStream ();
  
  stream << s;

  return stream;
}


//
//Function: operator<<
//
//Member-Of: VostreamProxy
//
//Paramters:
//      const double f
//
//Description:
//      Adds the given number of spaces into the stream.
// 
Vostream &VostreamProxy::spaces (int s)
{
  Vostream &stream = getStream ();
  
  stream << s;

  return stream;
}


//
//Function: operator<<
//
//Member-Of: VostreamProxy
//
//Paramters:
//      const float f
//
//Description:
//      Sends the given float to the stream.
//
Vostream &VostreamProxy::operator<< (const float f)
{
  Vostream &stream = getStream ();
  
  stream << f;

  return stream;
}

//
//Function: levelIncrement
//
//Member-Of: VostreamProxy
//
//Description:
//      Increments the messaging level.
//
Vostream &VostreamProxy::levelIncrement (void)
{
  Vostream &stream = getStream ();
  
  stream.levelIncrement();

  return stream;
}


//
//Function: levelIncrement
//
//Member-Of: VostreamProxy
//
//Description:
//      Decrements the messaging level.
//
Vostream &VostreamProxy::levelDecrement (void)
{
  Vostream &stream = getStream ();
  
  stream.levelDecrement();

  return stream;
}


//
//Function: levelIncrement
//
//Member-Of: VostreamProxy
//
//Parameters:
//      int l
//
//Description:
//      Sets the messaging level. The default level is 1.
//
Vostream &VostreamProxy::levelSet (int l = 1)
{
  Vostream &stream = getStream ();
  
  stream.levelSet(l);

  return stream;
}


//
//Function: operator<<
//
//Member-Of: VostreamProxy
//
//Parameters:
//      Frame &f
//
//Description:
//      Prints the given frame to the stream.
//
Vostream &VostreamProxy::operator<< (Frame &f)
{
  Vostream &stream = getStream ();
  
  stream << f;

  return stream;
}


//
//Function: operator<<
//
//Member-Of: VostreamProxy
//
//Parameters:
//      Frame &f
//
//Description:
//      Prints the given time to the stream.
//
Vostream &VostreamProxy::operator<< (OTime &t)
{
  Vostream &stream = getStream ();
  
  stream << t;

  return stream;
}

//
//Function: getStream
//
//Member-Of: VostreamProxy
//
//Return: Vostream & 
//
//Description:
//      Returns the thread specific Vostream instance
//

Vostream &VostreamProxy::getStream (void)
{
  _MT_SYNCMONITOR;

  TQueueListIterator<VostreamItem> iterator(&streamList);
  VostreamItem *item = 0;
  while ((item = iterator.next ()) && (item->threadId != OThread :: self()));
  
  if (!item) {
    item = new VostreamItem;
    item->threadId = OThread :: self ();
    item->stream = new Vostream (type, this);
    streamList.append(item);
  }

  return *item->stream;
}

//
//Function: removeStream
//
//Member-Of: VostreamProxy
//
//Parameters: Vostream * 
//
//Description:
//      Delete the specified instance from queue.
//

void VostreamProxy::removeStream (Vostream *stream)
{
  _MT_SYNCMONITOR;

  TQueueListIterator<VostreamItem> iterator(&streamList);
  VostreamItem *item = 0;
  while ((item = iterator.next ()) && (item->stream != stream));
  
  if (item) {
    streamList.remove (item);
    delete item->stream;
    delete item;
  }
}

#endif

//
//Constructor: Vostream
//
//Description:
//

#if _MT_ENABLED

Vostream::Vostream (Vostream::Type t, VostreamProxy *proxy)
    : EventTask (0, 0), type (t), def (t), level (1),
      oports (*proxy->getPorts())
{
  *buf = 0; 
}

#else

Vostream::Vostream (Vostream::Type t, Scheduler *s)
    : EventTask ("", s), type (t), def (t), level (1)
{
  *buf = 0; 
}

#endif




Vostream :: ~Vostream (void)
{
}




//
//Function: putOutput
//
//Member-Of: Vostream
//
//Description:
//      Attach the given port into the stream.
//
void Vostream::putOutput (Port *port)
{
  Port *p = new Port (this);
  port->connect (p);
  oports.append (p);
  return;
}

typedef bool (*CmpPortProc)(const Port*, const void *);

//
//Function: getOutput
//
//Member-Of: Vostream
//
//Description:
//      Deattach the given port from the stream.
//
void Vostream::getOutput (Port *port)
{
  delete oports.remove ((const void *)port, (CmpPortProc)Port::compareOther);
  return;
}


//
//Function: propagate
//
//Member-Of: Vostream
//
//Description:
//      Sends the message to every connected port.
//
void Vostream::propagate (VstreamMessage *vmsg)
{
  TQueueListIterator<Port> iter (&oports);
  Port *port;
  int used = 0;
  VstreamMessage *nmsg = vmsg;
  VstreamMessage *tmpMsg = 0;  

  if (iter.head () != iter.last()) {
    tmpMsg = new VstreamMessage (*vmsg);
  }

  while (port = iter.next ()) {
    if (used) {
      nmsg = new VstreamMessage (*tmpMsg);
    } else if (iter.head () == 0) {
      nmsg = tmpMsg;
    }
    
    Message *msg = nmsg;       // CC problems with references
    port->putMessage (msg);
    used = 1;
  }
  if (!used) {
    delete vmsg;
  }
  
  return;
}


//
//Function: operator<<
//
//Member-Of: Vostream
//
//Parameters:
//      Vostream::Type arg
//
//Description:
//      Sets the stream's type with the given type.
//
Vostream &Vostream::operator<< (Vostream::Type arg)
{
  type = arg;
  return *this;
}


//
//Function: operator<<
//
//Member-Of: Vostream
//
//Paramters:
//      VstreamCtrl arg
//
//Description:
//      Sends the given control token to the stream.
//
Vostream &Vostream::operator<< (VstreamCtrl arg)
{
  VstreamMessage *msg;
  switch (arg) {
    case endv:
    case vend:
    case vflush:
      if (type == DIRE) {
	printf("%s\n",buf);
      }
      msg = new VstreamMessage (type, level, std::string(buf), std::string(scope));
      *buf = 0;
      propagate (msg);
      type = def;
      break;
    default:
      // unknown control
    break;
  }
  
  return *this;
}


//
//Function: operator<<
//
//Member-Of: Vostream
//
//Paramters:
//      const char c
//
//Description:
//      Sends the given character to the stream.
//
Vostream &Vostream::operator<< (const char c)
{


  char lbuf[LBUFSIZE+1];
  sprintf (lbuf, "%c", c);
  strcat (buf, lbuf);
  return *this;
}

//
//Function: operator<<
//
//Member-Of: Vostream
//
//Paramters:
//      bool
//
//Description:
//      Sends the given boolean value to the stream.
//      Outputs 'true' or 'false'
//
Vostream &Vostream::operator<< (const bool b)
{

    
  if (b == true) {
    strcat (buf, "true");
  } else {
    strcat (buf, "false");
  }
  return *this;
}

//
//Function: operator<<
//
//Member-Of: Vostream
//
//Paramters:
//      const unsigned char c
//
//Description:
//      Sends the given character to the stream.
//
Vostream &Vostream::operator<< (const unsigned char c)
{


  char lbuf[LBUFSIZE+1];
  sprintf (lbuf, "%c", c);
  strcat (buf, lbuf);
  return *this;
}

//Operator: <<
//Member-Of: Vostream
//Paramters:
//      const int i
//Description:
//      Sends the given int to the stream.
//
Vostream &Vostream::operator<< (const int i)
{


  char lbuf[LBUFSIZE+1];
  sprintf (lbuf, "%d", i);
  strcat (buf, lbuf);
  return *this;
}

//
//Function: operator<<
//
//Member-Of: Vostream
//
//Paramters:
//      const unsigned int i
//
//Description:
//      Sends the given integer to the stream.
//
Vostream &Vostream::operator<< (const unsigned int i)
{


  char lbuf[LBUFSIZE+1];
  sprintf (lbuf, "%d", i);
  strcat (buf, lbuf);
  return *this;
}

//
//Function: operator<<
//
//Member-Of: Vostream
//
//Paramters:
//      const long i
//
//Description:
//      Sends the given long to the stream.
//
Vostream &Vostream::operator<< (const long i)
{


  char lbuf[LBUFSIZE+1];
  sprintf (lbuf, "%ld", i);
  strcat (buf, lbuf);
  return *this;
}

//
//Function: operator<<
//
//Member-Of: Vostream
//
//Paramters:
//      const unsigned long i
//
//Description:
//      Sends the given long integer to the stream.
//
Vostream &Vostream::operator<< (const unsigned long i)
{


  char lbuf[LBUFSIZE+1];
  sprintf (lbuf, "%ld", i);
  strcat (buf, lbuf);
  return *this;
}


//
//Function: operator<<
//
//Member-Of: Vostream
//
//Paramters:
//      const double f
//
//Description:
//      Sends the given double to the stream.
//
Vostream &Vostream::operator<< (const double f)
{

  
  char lbuf[LBUFSIZE+1];
  sprintf (lbuf, "%f", f);
  strcat (buf, lbuf);
  return *this;
}


//
//Function: operator<<
//
//Member-Of: Vostream
//
//Paramters:
//      const double f
//
//Description:
//      Sends the given string to the stream.
//
Vostream &Vostream::operator<< (const char *s)
{

  
  if (s != 0)
    strcat (buf, s);
  else
    strcat (buf, "(null)");
  
  return *this;
}


//
//Function: operator<<
//
//Member-Of: Vostream
//
//Paramters:
//      std::string
//
//Description:
//      Sends the given string to the stream.
//
Vostream &Vostream::operator<< (std::string s)
{
  if (!s.empty())
    strcat (buf, s.c_str());
  else
    strcat (buf, "(null)");
  
  return *this;
}


//
//Function: operator<<
//
//Member-Of: Vostream
//
//Paramters:
//      const double f
//
//Description:
//      Adds the given number of spaces into the stream.
// 
Vostream &Vostream::spaces (int s)
{


  for (int i=0; i < s; i++) {
    strcat (buf, " ");
  }
  
  return *this;
}


//
//Function: operator<<
//
//Member-Of: Vostream
//
//Paramters:
//      const float f
//
//Description:
//      Sends the given float to the stream.
//
Vostream &Vostream::operator<< (const float f)
{


  char lbuf[LBUFSIZE+1];
  sprintf (lbuf, "%f", f);
  strcat (buf, lbuf);
  return *this;
}

//
//Function: levelIncrement
//
//Member-Of: Vostream
//
//Description:
//      Increments the messaging level.
//
Vostream &Vostream::levelIncrement (void)
{
  level++;
  return *this;
}


//
//Function: levelIncrement
//
//Member-Of: Vostream
//
//Description:
//      Decrements the messaging level.
//
Vostream &Vostream::levelDecrement (void)
{
  level--;
  return *this;
}


//
//Function: levelIncrement
//
//Member-Of: Vostream
//
//Parameters:
//      int l
//
//Description:
//      Sets the messaging level. The default level is 1.
//
Vostream &Vostream::levelSet (int l = 1)
{
  level = l;
  return *this;
}


//
//Function: operator<<
//
//Member-Of: Vostream
//
//Parameters:
//      Frame &f
//
//Description:
//      Prints the given frame to the stream.
//
Vostream &Vostream::operator<< (Frame &f)
{
  FrameIterator iter (&f);
  char lbuf[GBUFSIZE+1];
  char abuf[LBUFSIZE+1];
  FrameSize p=0;
  FrameSize i;
  
  while (p < f.length ()) {
    for (i=0;i<OCTETS && p<f.length (); i++, p++) {
      Byte c = iter.read (p);
      sprintf (lbuf, "%0.2X ", c);
      strcat (buf, lbuf);
      if (c < 32 || c > 127) c = '.';
      abuf[i] = c;
    }
    abuf[i] = 0;
    for (;i<OCTETS;i++) {
      sprintf (lbuf, "   ");
      strcat (buf, lbuf);
    }
    strcat (buf, abuf);
    VstreamMessage *msg = new VstreamMessage (type, level, std::string(buf), scope);
    propagate (msg);
    *buf = 0;
  }
  return *this;
}


//
//Function: operator<<
//
//Member-Of: Vostream
//
//Parameters:
//      Frame &f
//
//Description:
//      Prints the given time to the stream.
//
Vostream &Vostream::operator<< (OTime &t)
{


  char lbuf[LBUFSIZE+1];
  sprintf (lbuf, "%ld.%06lu", t.getSeconds (), t.getMicros ());
  strcat (buf, lbuf);
  return *this;
}


#if _MT_ENABLED

VostreamProxy vout(Vostream::USER);
VostreamProxy verr(Vostream::USER);

#else

Vostream vout (Vostream::USER);
Vostream verr (Vostream::ERROR);

#endif







