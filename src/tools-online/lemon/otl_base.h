// -*- C++ -*-
//

#ifndef _otl_base_h
#define _otl_base_h 1

#include <doors/obl_dcl.h>
#include <doors/otl_dcl.h>

#include <set>

// Observer pattern.

class otl_Sobject {
public:
    virtual ~otl_Sobject();

    // connecting 

    virtual void attach(otl_Sobject *sob) = 0;
    virtual void detach(otl_Sobject *sob) = 0;


    // notify

    virtual void notify() = 0;


    // invoke interface
    // disconnect() is invoked when sob is detached from
    // other or it is deleted.
    // 
    // update() is invoked when there appear state change,
    // manged by notify().

    virtual void disconnect(otl_Sobject *from) = 0;
    virtual void update(otl_Sobject *from) = 0;

};

// otl_SobjectImpl

// from otl_Sobject: attach, detach, disconnet, notify.
// update() and disconnect() is left open.
// has 4 functions to implement.
class otl_SobjectImpl : virtual public otl_Sobject {
public:
    ~otl_SobjectImpl();
    // implements from Sobject
    void attach(otl_Sobject *sob);
    void detach(otl_Sobject *sob);
    void notify();
    void disconnect(otl_Sobject *from);
    void update(otl_Sobject *from);
private:
    otl_SobjectPtrSet _observers;
};



class otl_ToolKit {
public:
    ~otl_ToolKit();

    // create

    static otl_ToolKit *instance();


    // uiTask

    UiTask *uiTask();
    UiTask *uiTask(Scheduler *,IoHandler *);

private:
    otl_ToolKit();
    static otl_ToolKit *_instance;

    UiTask *_uiTask;
};


#endif // _otl_base_h
