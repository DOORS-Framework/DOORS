//Editor-Info: -*- C++ -*-
//  Copyright 2000 
//  	Telecoms Lab, Tampere University of Technology.  All rights reserved.

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
//
//  Subject: DEMON handler header file
//  Author:  Ilkka Karvinen, TTKK/DMI

/*
$Log: demonhand.h,v $
Revision 1.13  2004/01/19 10:45:24  bilhanan
#include fixed.

Revision 1.12  2003/01/03 17:10:10  ik
Simpler approach to indexing symbols. Temporary hack.

Revision 1.11  2002/12/30 14:42:27  ik
Support for index in symbols.

Revision 1.10  2002/12/30 13:12:56  ik
Time changed to absolute.

Revision 1.9  2002/12/04 16:16:29  ik
Timer starts from zero.

Revision 1.8  2002/11/25 16:52:48  ik
Added overloaded isEnabled() for MessageHandler.

Revision 1.7  2002/11/22 17:17:41  ik
Filter changes. setEnabled(), setEnabledRecursively(), setFilter()
functions added.

Revision 1.6  2002/11/11 16:27:56  ik
Bug fixes. Added lines for sending whole system symbol.

Revision 1.5  2002/08/14 09:33:38  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.4  2002/08/09 11:46:33  ik
Handler for InetAddr6.

Revision 1.3  2002/08/09 09:32:40  ik
Added c_str()'s to std::string type variables because Mico CORBA
doesn't have operator= from std::string to CORBA::String.

Revision 1.2  2002/07/23 14:46:56  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/05/13 14:54:40  bilhanan
Restructured

Revision 1.3  2002/05/10 10:49:17  ik
Text UI.

Revision 1.2  2002/05/03 13:42:48  ik
Memory handlers made conditional according to configuration.

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.7  2002/01/22 11:45:56  ik
update() moved to base class because it is always the same.

Revision 1.6  2002/01/21 17:19:00  ik
Changes to remove CORBA::Any from demon.idl.

Revision 1.5  2001/12/12 13:49:56  ik
Demon modified to use Scheduler::Instance().

Revision 1.4  2001/12/12 13:19:41  ik
Log tag added.

*/

#ifndef DEMONHAND_H
#define DEMONHAND_H

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <doors/demon.h>
#include <doors/hsitif.h>
#include <sys/time.h>

class DemonHand : public SymbolHandler
{
public:
  DemonHand (Symbol *s, const char *tn, Demon::SymbolType t);
  virtual ~DemonHand (void);
  const char* getTypename() { return typename_; }
  Demon::SymbolType getType() { return type_; }
  virtual Demon::Symbol getSymbol (void) = 0;
  virtual void update (void);
  void setEnabled(bool enabled = true) { enabled_ = enabled; }
  void setEnabledRecursive(bool enabled = true);
  virtual bool isEnabled() const { return enabled_; }
  bool setFilter (Demon::Filter& filter);
  unsigned long getIndex() { return index_; }

protected:
  // Helper functions
  Demon::Symbol createStateSymbol(std::string name, CORBA::ULong number);

private:
  static unsigned long indexpool_;
  unsigned long index_;
  bool enabled_;
  const char* typename_;
  Demon::SymbolType type_;
};


class DemonSystemHandler : public DemonHand
{
public:
  DemonSystemHandler (Symbol* s, const char *n);

  virtual void update (void);
  virtual Demon::Symbol getSymbol (void);
};


class DemonLinkListHandler : public DemonHand 
{
public:
  DemonLinkListHandler (Symbol *s, const char *n);

  virtual Demon::Symbol getSymbol (void);

private:
  LinkList *object_;
};


class DemonEventTaskHandler : public DemonHand 
{
public:
  DemonEventTaskHandler (Symbol *s, const char *n, 
			 Demon::SymbolType t = Demon::stEventTask);
  
  virtual Demon::Symbol getSymbol (void);
  
private:
  EventTask* object_;

};


class DemonPTaskHandler : public DemonEventTaskHandler 
{
public:
  DemonPTaskHandler (Symbol *s, const char *h);
  
  virtual Demon::Symbol getSymbol (void);

private:
  PTask* object_;
};


class DemonPortHandler : public DemonHand 
{
public:
  DemonPortHandler (Symbol *s, const char *n, 
		    Demon::SymbolType t = Demon::stPort);
  
  virtual Demon::Symbol getSymbol (void);

private:
  Port *object_;
};


class DemonTimerHandler : public DemonHand 
{
public:
  DemonTimerHandler (Symbol *s, const char *h);
  
  virtual Demon::Symbol getSymbol (void);

private:
  Timer *object_;
};


class DemonIfaceHandler : public DemonPortHandler 
{
public:
  DemonIfaceHandler (Symbol *s, const char *h);

  virtual Demon::Symbol getSymbol (void);

private:
  Iface *object_;
};


class DemonMessageHandler : public DemonHand 
{
public:
  DemonMessageHandler (Symbol *s, const char *h);

  virtual Demon::Symbol getSymbol (void);

  // Reimplemented here to check also state of the message end points
  virtual bool isEnabled() const;

private:
  Message* object_;
};

class DemonDebugSchHandler : public DemonHand 
{
public:
  DemonDebugSchHandler (Symbol *s, const char *h);

  virtual Demon::Symbol getSymbol (void);

private:
  Message* object_;
};

class DemonUserTypeHandler : public DemonHand 
{
public:
  DemonUserTypeHandler (Symbol *s, const char *type, 
			Demon::SymbolType t);
  
private:
  const char* type_;
};

class DemonSint8TypeHandler : public DemonUserTypeHandler
{
public:
  DemonSint8TypeHandler (Symbol *s)
    : DemonUserTypeHandler(s, "Sint8", Demon::stSint8), 
      object_ ((Sint8 *) s->object ()) { }

  virtual Demon::Symbol getSymbol (void);

private:
  Sint8* object_;
};

class DemonUint8TypeHandler : public DemonUserTypeHandler
{
public:
  DemonUint8TypeHandler (Symbol *s)
    : DemonUserTypeHandler(s, "Uint8", Demon::stUint8), 
      object_ ((Uint8 *) s->object ()) { }

  virtual Demon::Symbol getSymbol (void);

private:
  Uint8* object_;
};

class DemonSint16TypeHandler : public DemonUserTypeHandler
{
public:
  DemonSint16TypeHandler (Symbol *s)
    : DemonUserTypeHandler(s, "Sint16", Demon::stSint16), 
      object_ ((Sint16 *) s->object ()) { }

  virtual Demon::Symbol getSymbol (void);

private:
  Sint16* object_;
};

class DemonUint16TypeHandler : public DemonUserTypeHandler
{
public:
  DemonUint16TypeHandler (Symbol *s)
    : DemonUserTypeHandler(s, "Uint16", Demon::stUint16), 
      object_ ((Uint16 *) s->object ()) { }

  virtual Demon::Symbol getSymbol (void);

private:
  Uint16* object_;
};

class DemonSint32TypeHandler : public DemonUserTypeHandler
{
public:
  DemonSint32TypeHandler (Symbol *s)
    : DemonUserTypeHandler(s, "Sint32", Demon::stSint32), 
      object_ ((Sint32 *) s->object ()) { }

  virtual Demon::Symbol getSymbol (void);

private:
  Sint32* object_;
};

class DemonUint32TypeHandler : public DemonUserTypeHandler
{
public:
  DemonUint32TypeHandler (Symbol *s)
    : DemonUserTypeHandler(s, "Uint32", Demon::stUint32),
      object_ ((Uint32 *) s->object ()) { }

  virtual Demon::Symbol getSymbol (void);

private:
  Uint32* object_;
};

class DemonInetAddrTypeHandler : public DemonUserTypeHandler
{
public:
  DemonInetAddrTypeHandler (Symbol *s)
    : DemonUserTypeHandler(s, "InetAddr", Demon::stInetAddr),
      object_ ((InetAddr *) s->object ()) { }

  virtual Demon::Symbol getSymbol (void);

private:
  InetAddr* object_;
};

class DemonInetAddr6TypeHandler : public DemonUserTypeHandler
{
public:
  DemonInetAddr6TypeHandler (Symbol *s)
    : DemonUserTypeHandler(s, "InetAddr6", Demon::stInetAddr6),
      object_ ((InetAddr6 *) s->object ()) { }

  virtual Demon::Symbol getSymbol (void);

private:
  InetAddr6* object_;
};

class DemonUnixAddrTypeHandler : public DemonUserTypeHandler
{
public:
  DemonUnixAddrTypeHandler (Symbol *s)
    : DemonUserTypeHandler(s, "UnixAddr", Demon::stUnixAddr),
      object_ ((UnixAddr *) s->object ()) { }

  virtual Demon::Symbol getSymbol (void);

private:
  UnixAddr* object_;
};

class DemonboolTypeHandler : public DemonUserTypeHandler
{
public:
  DemonboolTypeHandler (Symbol *s)
    : DemonUserTypeHandler(s, "bool", Demon::stBoolean),
      object_ ((bool *) s->object ()) { }

  virtual Demon::Symbol getSymbol (void);

private:
  bool* object_;
};

class DemonIntTypeHandler : public DemonUserTypeHandler
{
public:
  DemonIntTypeHandler (Symbol *s)
    : DemonUserTypeHandler(s, "Int", Demon::stBoolean),
      object_ ((int *) s->object ()) { }

  virtual Demon::Symbol getSymbol (void);

private:
  int* object_;
};

class DemonStringTypeHandler : public DemonUserTypeHandler
{
public:
  DemonStringTypeHandler (Symbol *s)
    : DemonUserTypeHandler(s, "String", Demon::stChar),
      object_ ((char **) s->object ()) { }

  virtual Demon::Symbol getSymbol (void);

private:
  char** object_;
};

class DemonFrameTypeHandler : public DemonUserTypeHandler
{
public:
  DemonFrameTypeHandler (Symbol *s)
    : DemonUserTypeHandler(s, "Frame", Demon::stFrame),
      object_ ((Frame *) s->object ()) { }

  virtual Demon::Symbol getSymbol (void);

private:
  Frame* object_;
};

#if _CF_MEM
class DemonMMHandler : public DemonHand 
{
public:
  DemonMMHandler (Symbol *s, const char *n, const char* t);
  
  virtual Demon::Symbol getSymbol (void);

private:
  MemoryManager* object_;
  const char* type_;

};

class DemonStatPlainMMHandler : public DemonMMHandler 
{
public:
  DemonStatPlainMMHandler (Symbol *s, const char *n);

};
    
class DemonStatBlockHandler : public DemonMMHandler 
{
public:
  DemonStatBlockHandler (Symbol *s, const char *n, const char *t);

};
  
class DemonStatBlockMHandler : public DemonStatBlockHandler 
{
public:
  DemonStatBlockMHandler (Symbol *s, const char *h);

};
    
class DemonStatBlockMMHandler : public DemonStatBlockHandler 
{
public:
  DemonStatBlockMMHandler (Symbol *s, const char *h);

};
#endif

#endif


