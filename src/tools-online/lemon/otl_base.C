// -*- C++ -*-
//

#include <doors/otl_base.h>
#include <doors/debug.h>
#include <doors/uitask.h>

// otl_Sobject //////////////////////////////////////////////////////

otl_Sobject::~otl_Sobject() {} // instantiate vtbl here

// otl_SobjectImpl //////////////////////////////////////////////////

otl_SobjectImpl::~otl_SobjectImpl()
{
    while(!_observers.empty()) {
	otl_SobjectPtrSet::iterator i = _observers.begin();
	(*i)->disconnect(this);
	_observers.erase(i);
    }
}

void otl_SobjectImpl::attach(otl_Sobject *sob)
{
    std::pair<otl_SobjectPtrSet::iterator,bool> p = _observers.insert(sob);
    _DOORS_DEBUG(p.second == true,"sob already attached");
}

void otl_SobjectImpl::detach(otl_Sobject *sob)
{
    int num = _observers.erase(sob);
    if(num > 0) {
	sob->disconnect(this);
    }
    _DOORS_DEBUG(num == 1,"strange number of observers as " << num);
}

void otl_SobjectImpl::notify()
{
    otl_SobjectPtrSet::iterator i;
    for(i = _observers.begin();i != _observers.end();++i) {
	(*i)->update(this);
    }
}

void otl_SobjectImpl::disconnect(otl_Sobject *from)
{
    _DOORS_DEBUG(0,"non-implmented disconned called from " << from);
}

void otl_SobjectImpl::update(otl_Sobject *from)
{
    _DOORS_DEBUG(0,"non-implmented update called from " << from);
}

// otl_ToolKit //////////////////////////////////////////////////////

otl_ToolKit *otl_ToolKit::_instance = 0;

otl_ToolKit *otl_ToolKit::instance()
{
    if(!_instance) {
	_instance = new otl_ToolKit();
    }
    return _instance;
}

otl_ToolKit::otl_ToolKit()
    : _uiTask(0)
{
    _DOORS_DEBUG(0,"otl_ToolKit");
}

otl_ToolKit::~otl_ToolKit()
{
    _DOORS_DEBUG(0,"~otl_ToolKit");
    delete _uiTask;
}

UiTask *otl_ToolKit::uiTask()
{
    _DOORS_ERROR(_uiTask,"otl_ToolKit: uiTask is not initialized");
    return _uiTask;
}

UiTask *otl_ToolKit::uiTask(Scheduler *s,IoHandler *i)
{
    if(!_uiTask) {
	_uiTask = new UiTask(s,i);
    }
    return _uiTask;
}

