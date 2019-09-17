//Editor-Info: -*- C++ -*-
//
// 



#ifndef INPUTTASK_H
#define INPUTTASK_H

#include <doors/core.h>


//
//Class: InputMessage
//
//Inherited: SystemMessage
//
//Description: 
//      Message for delivering lines from InputTask to outside, 
//      for example to UiTask. Consists of one frame for storing data.
//
class InputMessage : public SystemMessage {
  public:
    InputMessage (void) : SystemMessage (M_INPUT) {};
    InputMessage (Frame &frm) : SystemMessage (M_INPUT), data (frm) {};
    Frame data;
};


//
//Class: InputTask
//
//Inherited: Otask, FileDevice
//
//Description: 
//      A special task for reading lines from a file. Uses FileDevice to
//      perform actual reading. After a whole line, ending to a newline
//      is read, a message with that line is constructed and passed to
//      output port.
//

class InputTask : public EventTask {
public:
  InputTask (Scheduler *s, IoHandler *io, 
	     const char *name = "/dev/tty", int mode = Device::READ);
  ~InputTask (void);
  
  bool execute (EventType type, Message *msg);
  Port *getPort(void);

  bool connect (Port *at);
  bool disconnect (Port *at);
  
  DevIf :: User devIf;

 protected:

  FileDevice *device;
  Port output;
  Frame buffer;
  InputMessage *current;

};


//
//Function: getPort
//
//Member-Of: InputTask
//
//Return: 
//      Port *
//
//Description: 
//      Returns a pointer input port. Function is used in connecting
//      InputTask to some other task like UiTask.
//
inline Port *InputTask :: getPort(void)
{

  return &output;
}

#endif


