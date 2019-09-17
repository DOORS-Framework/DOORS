//Editor-Info: -*- C++ -*-
//

#ifndef COMMAND_H
#define COMMAND_H

#include <doors/util.h>
#include <doors/alias.h>
#include <doors/otl_dcl.h>

//
//Class: CommandList
//
//Description: 
//

class CommandList {
  public:
    CommandList (void);
    virtual ~CommandList (void);
    
    bool add (Command *c);
    void remove (Command *c);

    TQueue<Command> *list (void) { return &_commands; }
    Command *find (const Name *name);

  private:
    TQueue<Command> _commands;
};


//
//Class: Command
//
//Description: 
//      Command is the base class of all commands and traced objects.
//      Command contains its name, help and a table of possible 'methods'.
//      'methods' are simple subcommands, which have a name and action
//      function. Command is used by calling its execute function with
//      list of words as parameter. The function then searches through 
//      it's list of 'methods' and calls corresponding action function.
//

class Command {
  public:
    enum Type { command, tracedSymbol, other };

    Command (const char *n, Command *p, const char *h);
    virtual ~Command (void);

    virtual Type type (void) const { return command; }

    virtual void listOptions (void);
    Command *execute (WordList *words);
    virtual Command *process (WordList *words);
    virtual Command *find (const Name *n);
    void printHelp (void);
    void printSubHelp (void);

    CommandList *subCommands (void) { return &_subCommands; }
    Command *parent (void) { return _parent; }
    virtual bool isEqual (const Name *n);
    virtual bool isApprx (const Name *n);
    const Name *name (void) const { return &_name; }
    void setParent (Command *p);

    static Command *top;

  protected:
    Name _name;
    Command *_parent;
    CommandList _subCommands;
    const char *_help;

    static AliasList aliases;
};


class Menu {
  public:
    
    class Tool : public Command {
      public:
	Tool (Command *p, const char *h);
	virtual ~Tool (void);

	class Log : public Command {
	  public:
	    Log (Command *p, const char *h);
	    virtual ~Log (void);

	    class Open : public Command {
	      public:
		Open (UiTask *t, Command *p, const char *h);
		virtual ~Open (void);

		virtual Command *process (WordList *words);
		void listOptions (void) { return; }

	      private:
		UiTask *ui;
	    };

	    class Cont : public Command {
	      public:
		Cont (Command *p, const char *h);
		virtual ~Cont (void);
		
		virtual Command *process (WordList *words);
	    };

	    class Hold : public Command {
	      public:
		Hold (Command *p, const char *h);
		virtual ~Hold (void);
		
		virtual Command *process (WordList *words);
	    };

	    class Close : public Command {
	      public:
		Close (Command *p, const char *h);
		virtual ~Close (void);
		
		virtual Command *process (WordList *words);
	    };
	    
	    class File : public Command {
	      public:
		File (Scheduler *s, const char *n, Command *p, const char *h);
		virtual ~File (void);

		void hold (void);
		void cont (void);
		
	      private:
		FileLogger *logger;
		bool stopped;
	    };
	};

	class Macro : public Command {
	  public:
	    Macro (UiTask *t, Command *p, const char *h);
	    virtual ~Macro (void);

	    void record (const char *n);
	    void unrecord (void);
	    bool isRecording (void);
	    void run (const char *n);

	    class Make : public Command {
	      public:
		Make (Command *p, const char *h);
		virtual ~Make (void);
		
		virtual Command *process (WordList *words);
		void listOptions (void) { return; }
	    };

	    class End : public Command {
	      public:
		End (Command *p, const char *h);
		virtual ~End (void);
		
		virtual Command *process (WordList *words);
	    };
	    
	    class Run : public Command {
	      public:
		Run (Command *p, const char *h);
		virtual ~Run (void);
		
		virtual Command *process (WordList *words);
		void listOptions (void) { return; }
	    };

	  private:
	    bool recording;
	    UiTask *ui;
	};

    };

#define TF_before	(1<<0)
#define TF_message	(1<<1)
#define TF_after	(1<<2)
#define TF_between	(1<<3)
#define TF_4	(1<<4)
#define TF_5	(1<<5)
#define TF_6	(1<<6)
#define TF_7	(1<<7)
#define TF_8	(1<<8)
#define TF_9	(1<<9)
#define TF_10	(1<<10)
#define TF_11	(1<<11)
#define TF_12	(1<<12)

    class TracedSymbol : public Command {
      public:
	TracedSymbol (const char *n, Command *p, const char *h, LEMonHandler *t);
	virtual ~TracedSymbol (void);
	
	virtual Type type (void) const { return tracedSymbol; }
	LEMonHandler *handler (void) { return _handler; }
	
	void updateChildren (TracedSymbol *c);
	
	void setTrace (Uint16 c);
	void addTrace (Uint16 c);
	bool isTrace (Uint16 c);
	
	Command *find (const Name *name);
	virtual void show (void);
	virtual Command *set (WordList *words);
	virtual void print (void);
	void structure (void);
	virtual Command *process (WordList *words);
	
	void listOptions (void);
	void printTraceFlags (void);
	
      private:
	Uint32 traceFlags;
	LEMonHandler *_handler;
    };
    

    class System : public TracedSymbol {
      public:
	System (Command *p, const char *h, LEMonHandler *t);
	virtual ~System (void);
	
	
	class Object : public TracedSymbol {
	  public:
	    Object (Command *p, const char *h, LEMonHandler *t);
	    
	  private:
	};

	class Struct : public Command {
	  public:
	    Struct (Command *p, const char *h);
	    virtual ~Struct (void);
	    
	    virtual Command *process (WordList *words);
	};

	class Show : public Command {
	  public:
	    Show (Command *p, const char *h);
	    virtual ~Show (void);
	    
	    virtual Command *process (WordList *words);
	};

	class Set : public Command {
	  public:
	    Set (Command *p, const char *h);
	    virtual ~Set (void);
	    
	    virtual Command *process (WordList *words);
	    void listOptions (void) { return; }
	};

	class Print : public Command {
	  public:
	    Print (Command *p, const char *h);
	    virtual ~Print (void);
	    
	    virtual Command *process (WordList *words);
	};

	class Trace : public Command {
	  public:
	    Trace (Command *p, const char *h);
	    virtual ~Trace (void);
	    
	    virtual Command *process (WordList *words);
	    void listOptions (void) { return; }
	    
	  private:
	    void printHelp (void);
	};

	static TracedSymbol *tmpObjects;
    };
    
    class Exit : public Command {
      public:
	Exit (UiTask *t, Command *p, const char *h);
	virtual ~Exit (void);

	virtual Command *process (WordList *words);

      private:
	UiTask *ui;
    };

    class Alias : public Command {
      public:
	Alias (Command *p, const char *h);

	virtual Command *process (WordList *words);
    };

    class Unalias : public Command {
      public:
	Unalias (Command *p, const char *h);
	virtual ~Unalias (void);

	virtual Command *process (WordList *words);
    };

    class Shell : public Command {
      public:
	Shell (Command *p, const char *h);
	virtual ~Shell (void);

	virtual Command *process (WordList *words);
    };

    class Help : public Command {
      public:
	Help (Command *p, const char *h = 0);
	virtual ~Help (void);

	virtual Command *process (WordList *words);
    };

    class Dot : public Command {
      public:
	Dot (Command *p, const char *h = 0);
	virtual ~Dot (void);

	virtual Command *process (WordList *words);
    };
};


extern int strapprx (const char *s1, const char *s2);


inline bool Command :: isEqual (const Name *n) {

    bool rc = false;

    if (n != 0) {
      rc = (*name () != *n) ? false : true;
    }
    return rc;
}


inline bool Command :: isApprx (const Name *n) {

    bool rc = false;

    if (n != 0) {
      rc = (strapprx (name ()->c_str(), n->c_str())) ? false : true;
    }
    return rc;
}

#endif  // COMMAND_H

