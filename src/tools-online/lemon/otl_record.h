// -*- C++ -*-
// Contents
// 1. Record Managers
// - otl_Record
// - otl_RecordProxy
// - otl_RecordMaster
// 2. Record Event Types
// - otl_RecordEvent
// - otl_CreateEvent
// - otl_DestroyEvent
// - otl_ExecuteEvent

#ifndef _OTL_RECORD_H
#define _OTL_RECORD_H 1

#include <doors/obl_dcl.h>
#include <doors/otl_dcl.h>
#include <doors/otl_base.h>
#include <doors/etask.h>
#include <doors/port.h>
#include <doors/message.h>

#include <list>


class otl_Record;
class otl_RecordEvent;
class otl_CreateEvent;
class otl_DestroyEvent;
class otl_ExecuteEvent;


typedef std::list<otl_RecordEvent*> otl_RecordEventPtrSeq;
typedef std::list<otl_Record*>      otl_RecordPtrSeq;

//+ otl_Record
class otl_Record {
public:
    //+ push_back
    //:return:
    //'syntax:
    virtual bool push_back(otl_RecordEvent *) = 0;
    //-

    //+ filter
    //:return:
    //. true if event is accpected.
    //.
    //. false is event is rejected, and thus not added into
    //. event sequence.
    //'syntax:
    virtual bool filter(otl_RecordEvent *event) = 0;
    //-

    //+ nextSerialNumber
    //:return:
    //'syntax:
    virtual int nextSerialNumber() = 0;
    //-

    virtual const otl_RecordEventPtrSeq &events() const = 0;
};
//-

//+ otl_RecordMaster
class otl_RecordMaster : public otl_Record, 
			 public EventTask, 
			 public otl_SobjectImpl
{
public:
    // types
    enum Messages {
	msgUpdate
    };
    class MsgUpdate : public Message {
	MessageType getType() const {return msgUpdate;}
	Message *clone(void) const {return new MsgUpdate();}
    };
    // implements from otl_Record
    bool push_back(otl_RecordEvent *);
    bool filter(otl_RecordEvent *event);
    int nextSerialNumber();
    // implemets
    static otl_RecordMaster *instance();
    void start(Scheduler *);
    void stop();
    void info();
    const otl_RecordEventPtrSeq &events() const {return _events;}
private:
    otl_RecordMaster();
    // implements from EventTask
    bool execute(EventType type, Message *msg);
    // implements none from otl_SobjectImpl
    // attrbutes
    static otl_RecordMaster *_instance; // xxx
    otl_RecordEventPtrSeq _events;      // xxx
    int _nextSerialNumber;              // xxx
    Port _relink;                       // xxx
};
//-

//+ otl_RecordEvent
class otl_RecordEvent {
public:
    // types
    enum Type {
	create,
	destroy,
	execute,
	other
    };
    // impls
    otl_RecordEvent(otl_Record *,Symbol *);
    const int serialNumber() const {return _serialNumber;}
    Symbol *symbol() const {return _symbol;}
    virtual Type type() const = 0;
protected:
    const int _serialNumber;
    Symbol *_symbol;
};
//-

//+ otl_CreateEvent
class otl_CreateEvent : public otl_RecordEvent {
public:
    otl_CreateEvent(otl_Record *record,Symbol *symbol);
    otl_RecordEvent::Type type() const {return otl_RecordEvent::create;}
    const obl_String &name() const {return _name;}
    otl_DestroyEvent *destroyEvent() const {return _destroyEvent;}
    void destroyedBy(otl_DestroyEvent *);
    bool isDestroyed() const {return _destroyEvent == 0 ? false : true;}
private:
    obl_String _name;
    otl_DestroyEvent *_destroyEvent;
};
//-

//+ otl_DestroyEvent
class otl_DestroyEvent : public otl_RecordEvent {
public:
    otl_DestroyEvent(otl_Record *record,Symbol *symbol) ;
    otl_RecordEvent::Type type() const {return otl_RecordEvent::destroy;}
    otl_CreateEvent *createEvent() const {return _createEvent;}
private:
    otl_CreateEvent *_createEvent;
};
//-

//+ otl_ExecuteEvent
class otl_ExecuteEvent : public otl_RecordEvent {
public:
    otl_ExecuteEvent(otl_Record *record,Symbol *symbol);
    otl_RecordEvent::Type type() const {return otl_RecordEvent::execute;}
    otl_CreateEvent *fromCreateEvent() const {return _fromEvent;}
    otl_CreateEvent *toCreateEvent() const {return _toEvent;}
    const obl_String &name() const {return _name;}
private:
    obl_String _name;
    otl_CreateEvent *_fromEvent;
    otl_CreateEvent *_toEvent;
};
//-

#endif // _OTL_RECORD_H
