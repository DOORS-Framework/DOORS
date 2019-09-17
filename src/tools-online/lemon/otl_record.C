#include <doors/vstream.h>
#include <doors/symbol.h>
#include <doors/otl_record.h>

// otl_Record ////////////////////////////////////////////////////////

// otl_RecordProxy ///////////////////////////////////////////////////

// otl_RecordMaster /////////////////////////////////////////////////

otl_RecordMaster *otl_RecordMaster::_instance = 0;


bool otl_RecordMaster::push_back(otl_RecordEvent *event)
{
    if(!filter(event)) return false;
    _events.push_back(event);
    _relink.putMessage(new otl_RecordMaster::MsgUpdate());
    if(scheduler) {
	scheduler->request (this, priority); // da hack
    }
    return true;
}

bool otl_RecordMaster::filter(otl_RecordEvent *) {return true;}

int otl_RecordMaster::nextSerialNumber()
{
    return ++_nextSerialNumber;
}

otl_RecordMaster *otl_RecordMaster::instance()
{
    if(!_instance) {
	_instance = new otl_RecordMaster();
    }
    return _instance;
}

void otl_RecordMaster::start(Scheduler *s)
{
    _DOORS_FAULT(s,"need scheduler");
    scheduler = s;
    scheduler->inform(this);
    _relink.putMessage(new otl_RecordMaster::MsgUpdate());
}

void otl_RecordMaster::stop()
{
    if(scheduler) {
	scheduler->forget(this);
	scheduler = 0;
    }
    _relink.putMessage(new otl_RecordMaster::MsgUpdate());
}

void otl_RecordMaster::info()
{
    vout << "[msc: info]" << endv;
    otl_RecordEventPtrSeq::iterator ei;
    int createEvents = 0,destroyEvents = 0,executeEvents = 0;
    for(ei = _events.begin();ei != _events.end();++ei) {
	switch((*ei)->type()) {
	case otl_RecordEvent::create:
	    ++createEvents;
	    break;
	case otl_RecordEvent::destroy:
	    ++destroyEvents;
	    break;
	case otl_RecordEvent::execute:
	    ++executeEvents;
	    break;
	}
    }
    vout << "Recording: " << (scheduler ? "yes" : "no") << endv;
    vout << "Load: " << getLoad() << endv;
    vout << "Events[" << _events.size() << "]:" << endv
	 << "  create: " << createEvents << endv
	 << "  destroy: " << destroyEvents << endv
	 << "  execute: " << executeEvents << endv
	 << endv;
}

otl_RecordMaster::otl_RecordMaster()
    : EventTask("Record Master"),
      _nextSerialNumber(0),
      _relink(this)
{
    _relink.connect(&_relink);
    _DOORS_DEBUG(0,"Record Master instantiated");
}

bool otl_RecordMaster::execute(EventType, Message *msg)
{
    switch(msg->getType()) {
    case otl_RecordMaster::msgUpdate:
	notify();
	break;
    default:
	_DOORS_DEBUG(0,"unused message got");
	break;
    }
    delete msg;
    return true;
}

// otl_RecordEvent ///////////////////////////////////////////////////

otl_RecordEvent::otl_RecordEvent(otl_Record *record,Symbol *symbol)
    : _serialNumber(record->nextSerialNumber()),
      _symbol(symbol)
{
}

// otl_CreateEvent ///////////////////////////////////////////////////

otl_CreateEvent::otl_CreateEvent(otl_Record *record,Symbol *s) 
    : otl_RecordEvent(record,s),
      _destroyEvent(0)
{
    _name = symbol()->name().c_str();
    _DOORS_DEBUG(0,"record: createEvent: " << _name.c_str());
}

void otl_CreateEvent::destroyedBy(otl_DestroyEvent *event)
{
    _destroyEvent = event;
}

// otl_DestroyEvent //////////////////////////////////////////////////

otl_DestroyEvent::otl_DestroyEvent(otl_Record *record,Symbol *s) 
    : otl_RecordEvent(record,s) 
{
    otl_RecordEventPtrSeq::const_iterator ievent;
    for(ievent = record->events().begin();
	ievent != record->events().end();
	++ievent) {
	if((*ievent)->symbol() == s) {
	    _createEvent = (otl_CreateEvent*)*ievent; // cast
	}
    }
    _DOORS_FAULT(_createEvent,"destroyEvent got, but no createEvent");
}

// otl_ExecuteEvent //////////////////////////////////////////////////

otl_ExecuteEvent::otl_ExecuteEvent(otl_Record *record,Symbol *s) 
    : otl_RecordEvent(record,s),
      _fromEvent(0),
      _toEvent(0)
{
    _name = symbol()->name().c_str();
    // get sending task (can not to exist) and receiving task for message 
    Message *msg = (Message*)symbol()->object(); // cast
    Port *fromPort = msg->getSender(); // can be nil
    Port *toPort = msg->getTarget();   // have to set
    _DOORS_FAULT(toPort,"got a message without target port, cannot exist !");
    EventTask *fromTask = fromPort != 0 ? fromPort->getTask() : 0;
    EventTask *toTask = toPort->getTask();
    // map EventTask to otl_CreateEvent
    otl_RecordEventPtrSeq::const_iterator ievent;
    for(ievent = record->events().begin();
	ievent != record->events().end();
	++ievent) {
	if(_fromEvent && _toEvent) break;
	if((*ievent)->symbol()->object() == fromTask) 
	    _fromEvent = (otl_CreateEvent*)*ievent; // cast
	if((*ievent)->symbol()->object() == toTask) 
	    _toEvent = (otl_CreateEvent*)*ievent; // cast
    }
    _DOORS_FAULT(_toEvent,"receiving createEvent for executeEvent is missing");
}
