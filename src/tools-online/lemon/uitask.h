///Editor-Info: -*- C++ -*-
//

#ifndef UITASK_H
#define UITASK_H

#include <doors/core.h>

#include <doors/otl_dcl.h>

typedef TQueueListIterator<Port> PortIterator;

class MemoryManager;

//
//Class: MultiPort
//
//Inherited: Port
//
//Description: 
//      MultiPort is a specialized port which can be connected to many
//      ports simultaneously. It can be used to receive messages, but
//      currently there is no way to select wanted port in sending.
//
class MultiPort : public Port {
    friend class UiTask;

  public:
    MultiPort (EventTask *);
    virtual ~MultiPort (void);

    bool connect (Port *to);
    bool connected (void);
    bool disconnectFrom (Port *from);

  private:
    inline virtual bool connectRequest (Port *to);
    inline virtual bool disconnectRequest (Port *from);

    TQueue<Port> others;
};


//
//Class: UiTask
//
//Inherited: EventTask
//
//Description: 
//      UiTask is the main user interface object. It plays two different 
//      roles. Firstly, it takes input from user, parses it and delivers 
//      separated words to command hierarchy for execution. Secondly
//      it can be used as 'ouput' like in CVOPS, e.g. users may bind 
//      their tasks upper interface to UiTask::output to have a virtual
//      upper layer.
//      CVOPS adapter uses blocking input, so there is support for that
//      in UiTask.
//      Macro recording is also done in UiTask. Currently it is done
//      in a very ugly way. (Hopefully this get's fixed some day)
//      
class UiTask : public EventTask {

  public:
    static UiTask* instance();
    UiTask (Scheduler *s, IoHandler *oih);
    ~UiTask (void);
    bool execute (EventType, Message *);
    bool save (Message *msg, EventType type);
    EventType event (Message *&msg);
    Load getLoad (void) const;
    bool disconnect (Port *);
    void terminate (void);
    void macroStart (const char *name);
    void macroEnd (void);
    void addInput (const char *name);
    static void readLine (char *buffer, int length);
    static int readChar (void);

    bool notify (MemoryManager *mm);
    void history (bool b);
    bool isTerminated (void) { return terminated; };

    MultiPort input;

  private:
    static UiTask *globalUi;
    static TtyLogger *tty;

    static bool cmp (const Link *, const void *);

    Command *commandSet;	// the default object to execute list
    Command *current;   	// the current object to send list.
    WordList *inputList; 	// parsed list of input tokens

    IoHandler *ioHandler;

    bool recording;          // recording a macro
    bool terminated;         // exiting by ui
    EditTask *defaultInput;
    FILE *macroFP; // sigh
    void performRecord (Frame *data);
};


//
//Constructor: MultiPort
//
//Parameters: 
//      EventTask *
//
//Description: 
//      Passes EventTask pointer to base class constructor.
//
inline MultiPort :: MultiPort (EventTask *t)
:Port (t)
{
}


//
//Destructor: MultiPort
//
//Parameters: 
//
//Description: 
//      Passes EventTask pointer to base class constructor.
//
inline MultiPort :: ~MultiPort (void) {
    PortIterator ports (&others);
    Port *port;
    
    while (port = ports.next ()) {
	port->disconnectRequest (this);
    }
}


//
//Function: disconnectRequest
//
//Member-Of: MultiPort
//
//Parameters: 
//      Port *
//
//Return: 
//      int
//
//Description: 
//      Called when other port is disconnected (deleted).
//      Informs task about disconnection and removes given port from 
//      the port list.
//
inline bool MultiPort :: disconnectRequest (Port *from)
{
    bool rc = true;

    if (others.remove (from)) {
	if (task) {
	    task->disconnect (from);
	}
    } else {
	rc = false;
    }

    return rc;
}


//
//Function: disconnectFrom
//
//Member-Of: MultiPort
//
//Parameters: 
//      Port *from
//
//Return: 
//      int
//
//Description: 
//      Informs task about disconnection and removes given port from 
//      the port list and calls disconnectRequest method of that port.
//
inline bool MultiPort :: disconnectFrom (Port *from)
{
    bool rc;

    if (task) {
	task->disconnect (this);
    }

    rc = from->disconnectRequest (this);

    return rc;
}


//
//Function: connect
//
//Member-Of: MultiPort
//
//Parameters: 
//      Port *to
//
//Return: 
//      int
//
//Description: 
//      Inserts other port to list of ports and connects other side.
//      Return value different from zero indicates error.
//
inline bool MultiPort :: connect (Port *to)
{
    bool rc = false;

    if (to && to->connectRequest (this)) {
        if (task == 0 || task->connect (this) == true) {
	    others.insert (to, false);
	    rc = true;
	} else {
	    to->disconnectRequest (this);
        }
    }
    return rc;
}


//
//Function: connectRequest
//
//Member-Of: MultiPort
//
//Parameters: 
//      Port *
//
//Return: 
//      int
//
//Description: 
//      Just inserts connected port to list of ports.
//      Return value different from zero indicates error.
//
inline bool MultiPort :: connectRequest (Port *to)
{
    bool rc = false;

    if (to && (task == 0 || task->connect (this))) {
	others.insert (to, false);
        rc = true;
    }
    return rc;
}


//
//Function: connected
//
//Member-Of: MultiPort
//
//Return: 
//      bool
//
//Description: 
//      Returns the non zero value if the port is connected.
//      In this case return true if there are any ports in the list of ports.
//
inline bool MultiPort :: connected (void)
{
    PortIterator iterator (&others);
    return (iterator.head ()) ? true : false;
}


//
//Function: isTerminated
//
//Member-Of: UiTask
//
//Return: 
//      bool
//
//Description: 
//      Returns true if terminate function is called. Otherwise the
//      return value is false.
//

#endif




