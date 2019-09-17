//Editor-Info: -*- C++ -*-
//
// 


 
#include <doors/inputask.h>
#include <doors/vstream.h>
#include <doors/ocoremac.h>

//
//Constructor: InputTask
//
//Parameters: 
//       Scheduler *, IoHandler *, const char *name
//
//Description: 
//      The constructor initializes variables 
//      and calls base class constructors.
//      FileDevice opens the file automatically, but if it fails
//      the task deletes itself. current points always to an allocated 
//      message for sending. 
//

InputTask :: InputTask (Scheduler *s, IoHandler *io, const char *n, int md) 
: EventTask (n, s), device (0), output (this),
 devIf(this)
{

  if (strcmp(n, "/dev/tty")) 
    device = new FileDevice (io, this, n, 256, md);
  else
    device = new TtyDevice (io, this, TtyDevice :: READ, 256);

  current = new InputMessage;
  device->readBytes(StreamDevice :: immediate);
  devIf.connect (&device->iFace);
  device->open();
}


//
//Destructor: InputTask
//
//Description: 
//      The destructor frees current message, if present (not sent).
//
InputTask :: ~InputTask (void)
{
  if (device)
    delete device;
  if (current)
    delete current;
}


//
//Function: run
//
//Member-Of: InputTask
//
//Description: 
//      run-function is called when the FileDevice has been able 
//      to read something. Input from user is cutted to lines and
//      forwarded as messages. At error the task is destroyed.
//
bool InputTask :: execute (EventType type, Message *msg) 
{
  if (msg->getTarget () == &devIf) {
  
   switch (msg->getType()) {
      
    case (DevIf :: open) : {
      delete msg;
      break;
    }
    
    case (DevIf :: close) : {
      device->close();
      getIoHandler()->forget(device);
      delete msg;
      break;
    }
    case (DevIf :: read) : {
      buffer = ((DevIf::Read *) msg)->data;
      device->readBytes(StreamDevice :: immediate);
      if (buffer.length() > 0) {
	// we got something from luser
    
	for (FrameSize len = buffer.length(); len > 0; len--) {
      
	  Byte b = buffer.getFirst();
	  current->data.putLast (b);
	  if (b == '\n') {
	    //	printf("%s", (Byte *) current->data);
	    Message *bMsg = current;       // CC problems
	    output.putMessage (bMsg);
	    current = new InputMessage;
	  }
	}
      }
      delete msg;
      break;
    }
   }   
  }

  return true;
}

bool InputTask :: connect (Port *)
{

  if (device->getStatus() & Device::ERROR) {
    scheduler->forget (this);
    return false;
  }

  return true;
}


bool InputTask :: disconnect (Port *)
{

  return true;
}







