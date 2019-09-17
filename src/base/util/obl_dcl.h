// 
//  Copyright 1999
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
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

$Log: obl_dcl.h,v $
Revision 1.6  2004/02/10 09:54:22  bilhanan
DOORS integer types all converted to C99 types.

Revision 1.5  2003/04/17 12:59:48  bilhanan
int64 types added

Revision 1.4  2002/07/23 16:34:07  hartmanj

Development

Revision 1.3  2002/07/23 14:25:23  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/05/12 20:51:39  ik
Removed bool typedefs.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.4  2002/02/25 17:06:05  ik
sockaddr.* split to inetaddr.* and unixaddr.* and the depending
files changed accordingly.

Revision 1.3  2002/02/18 09:27:32  bilhanan

CORBA datatypes sucked in from <doors/datatypes.h>.

Revision 1.2  2001/06/08 16:45:47  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.6  2001/06/08 08:39:38  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef _obl_dcl_h
#define _obl_dcl_h 1

#include <doors/doorsconf.h>
#include <string>
#include <vector>
#include <inttypes.h>

// rules
// - add here global declarations and cpp macros
// - fill the compiler symbol table here for obl-library by declarations
// - do NOT include here any definitions
// - do NOT add here translation-unit-only/local declarations and macros
//   put them into c-file

// class forward declarations
class Address;
class AsnType;
class Buffer;
class ConnectionList;
class Device;
class EditTask;
class EventTask;
class FileDevice;
class FileLogger;
class Frame;
class InputTask;
class IoHandler;
class Logger;
class Message;
class Otask;
class OTime;
class Port;
class PTask;
class RRScheduler;
class Scheduler;
class SystemMessage;
class SystemPort;
class Symbol;
class SymbolHandler;
class Timer;
class TimerMessage;
class TimerTask;
class TtyLogger;
class LinkList;
class Iface;
class Peer;
class InetAddr;
class MemoryManager;
class StatPlainMemManager;
class StatBlockManager;
class StatBlockMemManager;
class VMessage;
class Vdebug;
class Vistream;
class Vostream;
class Vstream;
class VstreamMessage;
class UnixAddr;
class obl_String;

// bool
// PORT-HERE
#if defined(__GNUG__)
#define OD_CXXBOOL 1
#endif

#if defined(bool)
#define OD_CPPBOOL 1
#endif

// This seems to be obsolete by now. Anyway, it brakes the compilation
// in HP-UX/aCC and still works everywhere else so excluded now. 
#if 0
#if ! defined(bool) && ! defined(__GNUG__) && (__SUNPRO_CC < 0x500) 
#define bool int
#define true 1
#define false 0
#define OD_CPPBOOL 1
#endif
#endif

#define OdBoolean bool
#define OdFalse false
#define OdTrue true

// typedefs
typedef uint8_t  Byte;
typedef uint16_t Word;
typedef uint32_t  Dword;
typedef uint8_t  Uint8;
typedef uint8_t  uint8;
typedef uint16_t Uint16;
typedef uint16_t uint16;
typedef uint32_t  Uint32;
typedef uint32_t  uint32;
typedef int8_t           Sint8;
typedef int8_t           sint8;
typedef int16_t          Sint16;
typedef int16_t          sint16;
typedef int32_t           Sint32;
typedef int32_t           sint32;
typedef Sint32         MessageType;
typedef signed long      Load;
typedef int16_t         LinkSize;
typedef int64_t         int64;
typedef int64_t         Int64;

#if 0
namespace CORBA {

  typedef unsigned char Boolean; 
  typedef unsigned char Char; 
  typedef wchar_t WChar;  
  typedef unsigned char Octet; 
  typedef short Short; 
  typedef unsigned short UShort; 
  typedef long Long; 
  typedef unsigned long ULong; 
  typedef float Float; 
  typedef double Double; 
  typedef long double LongDouble; 
  typedef Boolean& Boolean_out; 
  typedef Char& Char_out; 
  typedef WChar& WChar_out; 
  typedef Octet& Octet_out; 
  typedef Short& Short_out; 
  typedef UShort& UShort_out; 
  typedef Long& Long_out; 
  
  typedef ULong& ULong_out; 
  
  typedef Float& Float_out; 
  typedef Double& Double_out; 
  
  typedef std::string String;

  // some necessary typedefs

  typedef void Status;     // <-- Status will become obsolete in future specs
  typedef ULong Flags;

  // why are these flag defined as values? coz they're bitmasks.

  const Flags ARG_IN = 1;
  const Flags ARG_OUT = 2;
  const Flags ARG_INOUT = 3;

  const Flags OUT_LIST_MEMORY = 8;
  const Flags IN_COPY_VALUE = 16;

  const Boolean TRUE = 1;
  const Boolean FALSE = 0;

  typedef std::string Context;
  typedef Context* Context_ptr;

  enum TCKind {tk_null, tk_void, tk_short, tk_long, tk_ushort, tk_ulong, tk_float, tk_double, tk_boolean, tk_char, tk_octet, tk_any, tk_TypeCode, tk_Principal, tk_objref, tk_struct, tk_union, tk_enum, tk_string, tk_sequence, tk_array, tk_alias, tk_except, tk_longlong, tk_ulonglong, tk_longdouble, tk_wchar, tk_wstring, tk_fixed};

  typedef std::string ObjectId;
  typedef std::string RepositoryId;
  
  typedef unsigned long PolicyType;

  const CORBA::PolicyType THREAD_POLICY_ID = 16;
  const CORBA::PolicyType LIFESPAN_POLICY_ID = 17;
  const CORBA::PolicyType ID_UNIQUENESS_POLICY_ID = 18;
  const CORBA::PolicyType ID_ASSIGNMENT_POLICY_ID = 19;
  const CORBA::PolicyType IMPLICIT_ACTIVATION_POLICY_ID = 20;
  const CORBA::PolicyType SERVANT_RETENTION_POLICY_ID = 21;
  const CORBA::PolicyType REQUEST_PROCESSING_POLICY_ID = 22;

  class Policy {
  public:
    PolicyType policy_type;
  };
 
};
#endif

#endif // _obl_dcl_h
