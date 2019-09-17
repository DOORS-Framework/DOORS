//Editor-Info: -*- C++ -*-
//
// 



#ifndef LEMONHAND_H
#define LEMONHAND_H

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <string>
#include <doors/hsitif.h>
#include <doors/ptb.h>
#include <doors/command.h>
#include <doors/dmm.h>
#include <doors/otl_dcl.h>
#include <string>

#if HAVE_IPV6
#include <doors/inetaddr6.h>
#endif

#define FF_show 	(1<<0)
#define FF_structure	(1<<1)
#define FF_set   	(1<<2)
#define FF_print	(1<<3)
#define FF_trace	(1<<4)


class LEMonHandler : public SymbolHandler {
  public:
    enum Type { system, linklist, eventtask, ptask, stm, debugsch, port, timer, iface, 
		message, address, fundtype, usertype, memoryManager, other };
    
    LEMonHandler (Symbol *s, const char *tn, Type t);
    virtual ~LEMonHandler (void);

    void initMenu (Uint8 code, const char *h = 0);

    void setOption (Menu :: TracedSymbol *t) { _option = t; }

    Type type (void) { return _type; }
    Menu :: TracedSymbol *option (void) { return _option; }
    std::string typeName (void) { return _typename; }
    virtual LEMonHandler *currentSet (LEMonHandler *);

    void update (void) { print (); }
    virtual void show (void);
    virtual void structure (void);
    virtual LEMonHandler *set (WordList *words);
    virtual void print (void);
    virtual bool listOptions (void) { return false; }
    virtual Command *process (WordList *words) { return 0; }

    virtual Command *find (const Name *n) { return 0; }

  private:
    Name _typename;
    Menu :: TracedSymbol *_option;
    Type _type;
    Uint8 _funcFlags;
    Symbol *_current;
};


class SystemHandler : public LEMonHandler {
  public:
    SystemHandler (Symbol *s, const char *h);
    ~SystemHandler (void);

  protected:
    Menu :: System *object;
};


class EventTaskHandler : public LEMonHandler {
  public:
    EventTaskHandler (Symbol *s, const char *tn, const char *h);

    void update (void);

  protected:
    const char *formatDate (OTime &currentTime);
    EventTask *object;
};


#if _MT_ENABLED

class MTEventTaskHandler : public EventTaskHandler {
  public:
    MTEventTaskHandler (Symbol *s, const char *tn, const char *h);

    void update (void);
  
  protected:
    bool _beforeRun;
};

typedef MTEventTaskHandler ETaskHandler;
#else
typedef EventTaskHandler ETaskHandler;
#endif

class PTaskHandler : public ETaskHandler {
  public:
    PTaskHandler (Symbol *s, const char *h);

    virtual void print (void);

  protected:
};


class DebugSchHandler : public LEMonHandler {
  public:
    DebugSchHandler (Symbol *s, const char *h);

    Command *process (WordList *words);
    bool listOptions (void);

  protected:
    DebugScheduler *object;
};


class LinkListHandler : public LEMonHandler {
  public:
    LinkListHandler (Symbol *s, const char *h);

    virtual void print (void);

  protected:
    LinkList *object;
};


class PortHandler : public LEMonHandler {
  public:
    PortHandler (Symbol *s, const char *n, const char *h);

    virtual void show (void);
    virtual void structure (void);

    Port *port (void) { return object; }

  protected:
    Port *object;
};


class TimerHandler : public LEMonHandler {
  public:
    TimerHandler (Symbol *s, const char *h);

    virtual void show (void);
    virtual void print (void);
    virtual LEMonHandler *set (WordList *words);
    
    Timer *timer (void) { return object; }

  protected:
    Timer *object;
};


class IfaceHandler : public PortHandler {
  public:
    IfaceHandler (Symbol *s, const char *h);

    bool listOptions (void);
    Command *find (const Name *n);

    virtual void show (void);
    void deleteMessages (void);

    Iface *iface (void) { return object; }
    LEMonHandler *currentSet (LEMonHandler *);

  protected:
    Iface *object;
};


class MessageHandler : public LEMonHandler {
  public:
    MessageHandler (Symbol *s, const char *h);

    void print (void);
    Message *message (void) { return object; }

  protected:
    Message *object;
};


template<class Object> class UserTypeHandler : public LEMonHandler {
  public:
    UserTypeHandler (Symbol *s, const char *type);

  protected:
    Object *object;
};


template<class Object> UserTypeHandler<Object> :: UserTypeHandler (Symbol *s, const char *t)
: LEMonHandler (s, t, usertype), object ((Object *) s->object ()) {
    
    initMenu (FF_show | FF_set | FF_print);
};


// HPUX hack

template<class Type> bool setInt (Type *v, WordList *words);

template<class Object> class FundTypeHandler : public LEMonHandler {
  public:
    virtual void print (void);
    FundTypeHandler (Symbol *s, const char *type);
    
    void show (void);
    LEMonHandler *set (WordList *words){
      _DOORS_ERROR(words,"no arguments");
      LEMonHandler *rc = 0;
      
      vout << Vostream::VAR << "asking " << symbol ()->name () << endv;
      
      if (set (*object, words)) {
	rc = this;
      } else {
	Command *cparent = option ()->parent ();
	if (cparent) {
	  if (cparent->type () == Command :: tracedSymbol) {
	    Menu :: TracedSymbol *tparent = (Menu :: TracedSymbol *) cparent;
	    rc = ((LEMonHandler *) tparent->handler ())->currentSet (this);
	  }
	}
      }
      
      return rc;
    }
    
    static bool set (bool &v, WordList *words) {

      Name *word = words->remove ();
      
      if (word) {
	const char *str = word->c_str ();
	
	if (!strapprx ("true", str)) {
	  v = true;
	  delete word;
	  return false;
	}
	if (!strapprx ("true", str)) {
	  v = true;
	  return false;
	} else {
	  if (!strapprx ("false", str)) {
	    v = false;
	    delete word;
	    return false;
	  }
	}
      }
      vout << Vostream::INFO << "( true false )" << endv;
      
    return true;
    }
  
    static bool set (Sint16 &v, WordList *words) { return setInt (&v, words); }
    static bool set (Sint8 &v, WordList *words) { return setInt (&v, words); }
    static bool set (Sint32 &v, WordList *words) { return setInt (&v, words); }
    static bool set (Uint8 &v, WordList *words) { return setInt (&v, words); }
    static bool set (Uint16 &v, WordList *words) { return setInt (&v, words); }
    static bool set (Uint32 &v, WordList *words) { return setInt (&v, words); }
    static bool set (signed long &v, WordList *words) { return setInt (&v, words); }
   
  static bool set (char *&v, WordList *words){

    Name *word = words->remove ();
    if (word) {
	const char *str = word->c_str ();
	if (str) {
	    strcpy (v, str);
	}
	delete word;
	return false;
    }
    vout << Vostream::INFO << "( <charstring> )" << endv;

    return true;
  }

  static bool set (std::string &v, WordList *words){

    Name *word = words->remove ();
    if (word) {
	const std::string str = *word;
 	if (str.empty() == false) {
	    v= str;
	}
	delete word;
	return false;
    }
    vout << Vostream::INFO << "( <string> )" << endv;

    return true;
  }

  static bool set (Frame &v, WordList *words){

    v.destroy ();

    Name *word = words->remove ();
    if (word) {
	const char *str = word->c_str ();
	if (str) {
	    v.putFirst ((const Byte *)str, strlen (str));
	    delete word;
	    return false;
	}
	delete word;
    }
    vout << Vostream::INFO << "( <frame> )" << endv;
    return true;
  }

  static bool set (UnixAddr &v, WordList *words){    
    Name *word = words->remove ();
    
    // if value is supported process it
    if (word) {
      const char *str = word->c_str ();
      v.set (str);
      delete word;
      return false;
    }
    
    vout << Vostream::INFO << "( <rendezvous point> )" << endv;
    return true;
  }

  static bool set (InetAddr &v, WordList *words) {

    static int state = 0;
    Name *word = 0;
    
    while (word = words->remove ()) {
      static int portNbr;
      static const char *addr;
      
      switch (state) {
      case 0:
	portNbr = atoi (word->c_str ());
	state++;
	break;
	
      case 1:
	addr = word->c_str ();
	v.set (portNbr, addr);
	state = 0;
	delete word;
	return false;
      }
      delete word;
    }

    
    switch (state) {
    case 0:
      vout << "( <port> )" << endv;
      break;
      
    case 1:
      vout << "( <host address> )" << endv;
      break;
    }
    return true;
  }

#if HAVE_IPV6
  static bool set (InetAddr6 &v, WordList *words) {

    static int state = 0;
    Name *word = 0;
    
    while (word = words->remove ()) {
      static int portNbr;
      static const char *addr;
      
      switch (state) {
      case 0:
	portNbr = atoi (word->c_str ());
	state++;
	break;
	
      case 1:
	addr = word->c_str ();
	v.set (portNbr, addr);
	state = 0;
	delete word;
	return false;
      }
      delete word;
    }

    
    switch (state) {
    case 0:
      vout << "( <port> )" << endv;
      break;
      
    case 1:
      vout << "( <host address> )" << endv;
      break;
    }
    return true;
  }
#endif
  
  protected:
   Object *object;
};

Vostream &operator << (Vostream &s, UnixAddr &a);
Vostream &operator << (Vostream &s, InetAddr &a);
#if HAVE_IPV6
Vostream &operator << (Vostream &s, InetAddr6 &a);
#endif

#if _MT_ENABLED
Vostream &operator << (VostreamProxy &s, UnixAddr &a);
Vostream &operator << (VostreamProxy &s, InetAddr &a);
#if HAVE_IPV6
Vostream &operator << (VostreamProxy &s, InetAddr6 &a);
#endif
#endif

/* template<class Object> bool FundTypeHandler<Object>::set(UnixAddr &v, WordList *words) {

    Name *word = words->remove ();
    
    // if value is supported process it
    if (word) {
	const char *str = word->get ();
	v.set (str);
	delete word;
	return false;
    }

    vout << Vostream::INFO << "( <rendezvous point> )" << endv;
    return true;
}


template<class Object> bool FundTypeHandler<Object> :: set (InetAddr &v, WordList *words) {

    static int state = 0;
    Name *word = 0;

    while (word = words->remove ()) {
        static int portNbr;
        static const char *addr;

        switch (state) {
          case 0:
            portNbr = atoi (word->get ());
            state++;
            break;

          case 1:
            addr = word->get ();
	    v.set (portNbr, addr);
            state = 0;
            delete word;
            return false;
        }
        delete word;
    }
            
    switch (state) {
      case 0:
        vout << "( <port> )" << endv;
        break;

      case 1:
        vout << "( <host address> )" << endv;
        break;
    }
    return true;
    }
*/

template<class Object> void FundTypeHandler<Object> :: show (void) {

    vout << Vostream::VAR << symbol ()->name() << "::= " << typeName () << endv;
}


/*
template<class Object> LEMonHandler *FundTypeHandler<Object> :: set (WordList *words) 
{
    _DOORS_ERROR(words,"no arguments");
    LEMonHandler *rc = 0;

    vout << Vostream::VAR << "asking " << symbol ()->name () << endv;

    if (set (*object, words)) {
	rc = this;
    } else {
	Command *cparent = option ()->parent ();
	if (cparent) {
	    if (cparent->type () == Command :: tracedSymbol) {
		Menu :: TracedSymbol *tparent = (Menu :: TracedSymbol *) cparent;
		rc = ((LEMonHandler *) tparent->handler ())->currentSet (this);
	    }
	}
    }

    return rc;
} 
*/
   
template<class Object> void FundTypeHandler<Object> :: print (void) {

    vout << Vostream::VAR << typeName () << " " << symbol ()->name() << " = ";
    if (object) {
	vout << *object << endv;
    } else { 
	vout << "(null)" << endv;
    }
}




class MemStruct {
  public:
    void init (MemoryStatistic *ms);

    uint32 totalAllocAmount;
    uint32 currentAllocAmount;

    uint32 totalAllocSize;
    uint16 meanAllocSize; 
    uint16 largestAllocSize;

    bool statisticError;
};

class BlockStruct : public MemStruct {
  public:
    void init (BlockMemoryStatistic *ms);

    uint32 blockSize;
    uint16 blocks;
    uint32 poolSize;
    uint32 currentTotalAllocSize;
    uint32 largestTotalAllocSize;

    uint32 wantedCurrentTotalAllocSize;
    uint32 wantedLargestTotalAllocSize;
    uint32 wantedTotalAllocSize;
    uint16 wantedMeanAllocSize;
    uint16 wantedLargestAllocSize;
    uint16 fragmentation;
    bool memoryError;
};

typedef TQueue<BlockStruct> BlockStructSeq;

class BlockMemStruct : public BlockStruct {
  public:
    void init (StatBlockMemManager *ms);

    uint16 blockManagerAmount;

    TQueue<BlockStruct> managerList;
};


class MMHandler : public LEMonHandler {
  public:
    MMHandler (Symbol *s, const char *t, const char *h);
    virtual ~MMHandler (void);

    virtual void show (void);
    virtual void print (void) = 0;
    virtual void printMS (MemStruct *mm);

  protected:
    MemoryManager *object;
};

    
class StatPlainMMHandler : public MMHandler {
  public:
    StatPlainMMHandler (Symbol *s, const char *h);

    void print (void);
    
  protected:
};

    
class StatBlockHandler : public MMHandler {
  public:
    StatBlockHandler (Symbol *s, const char *t, const char *h);
    
    virtual void printBS (BlockStruct *mm);

  protected:
};

    
class StatBlockMHandler : public StatBlockHandler {
  public:
    StatBlockMHandler (Symbol *s, const char *h);

    void print (void);
  protected:
};

    
class StatBlockMMHandler : public StatBlockHandler {
  public:
    StatBlockMMHandler (Symbol *s, const char *h);

    void print (void);
    
  protected:
};

// template stuff from lemonhand.C
template<class Object> FundTypeHandler<Object> :: FundTypeHandler (Symbol *s, const char *t)
: LEMonHandler (s, t, fundtype), object ((Object *) s->object ()) {
    
    initMenu (FF_show | FF_set | FF_print);
}


/*
template<class Object> bool FundTypeHandler<Object> :: set (bool &v, WordList *words) {

    Name *word = words->remove ();

    if (word) {
	const char *str = word->get ();

	if (!strapprx ("true", str)) {
	    v = true;
            delete word;
	    return false;
	}
	if (!strapprx ("true", str)) {
	    v = true;
	    return false;
	} else {
	    if (!strapprx ("false", str)) {
		v = false;
		delete word;
		return false;
	    }
	}
    }
    vout << Vostream::INFO << "( true false )" << endv;

    return true;
}
*/

template<class Type> bool setInt (Type *v, WordList *words) {

    Name *word = words->remove ();

    if (word) {
	const char *str = word->c_str ();
	if (str) {
	    *v = atoi (str);
	    delete word;
	    return false;
	}
	delete word;
    }
    vout << Vostream::INFO << "( <value> )" << endv;
    return true;
}

/*
template<class Object> bool FundTypeHandler<Object> :: set (Frame &v, WordList *words) {

    v.destroy ();

    Name *word = words->remove ();
    if (word) {
	const char *str = word->get ();
	if (str) {
	    v.putFirst ((const Byte *)str, strlen (str));
	    delete word;
	    return false;
	}
	delete word;
    }
    vout << Vostream::INFO << "( <frame> )" << endv;
    return true;
}


template<class Object> bool FundTypeHandler<Object> :: set (char *&v, WordList *words) {

    Name *word = words->remove ();
    if (word) {
	const char *str = word->get ();
	if (str) {
	    strcpy (v, str);
	}
	delete word;
	return false;
    }
    vout << Vostream::INFO << "( <string> )" << endv;

    return true;
}
*/

    
#endif  // LEMONHAND_H

