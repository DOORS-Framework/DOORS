// -*- C++ -*-
//



// Contents
// - OptValue
// - OptBool
// - OptString
// - OptDB
// - OptX11Load


#include <doors/otl_opt.h>
//#include <doors/doorsconf.h>
#include <doors/debug.h>

//#define NeedFunctionPrototypes 1
//#include <X11/Xlib.h>
//#include <X11/Xresource.h>
#include <string.h>

#define otl_RESNAME "Otl"

// OptValue //////////////////////////////////////////////////////////

otl_OptValue::otl_OptValue(const char *name)
    : _name(name) 
{}

otl_OptValue::otl_OptValue(const otl_OptValue &value)
    : _name(value.name())
{}

otl_OptValue::~otl_OptValue() {} 

// OptBool ///////////////////////////////////////////////////////////

otl_OptBool::otl_OptBool(const char *name,bool b) 
    : otl_OptValue(name),
      _value(b) 
{}

otl_OptBool::otl_OptBool(const otl_OptBool &b) 
    : otl_OptValue(b),
      _value(b._value) 
{}

bool otl_OptBool::cstringSet(const char *s)
{
    bool rv = false;
    if(!strcasecmp(s,"TRUE") ||
       !strcasecmp(s,"ON")) {
	_value = true;
	rv = true;
    }
    if(!strcasecmp(s,"FALSE") ||
       !strcasecmp(s,"OFF")) {
	_value = false;
	rv = true;
    }
    notify();
    return rv;
}

char *otl_OptBool::cstring(char *s,int l)
{
    if(_value == true) {
	strncpy(s,"true",l);
    } else {
	strncpy(s,"false",l);
    }
    return s;
}

// OptString /////////////////////////////////////////////////////////

otl_OptString::otl_OptString(const char *name,obl_String &value)
    : otl_OptValue(name),
      _value(value)
{}

otl_OptString::otl_OptString(const char *name,const char *value)
    : otl_OptValue(name),
      _value(value)
{}

otl_OptString::otl_OptString(const otl_OptString &s)
    : otl_OptValue(s),
      _value(s._value)
{}

bool otl_OptString::cstringSet(const char *value)
{
    _value = value;
    notify();
    return true;
}

char *otl_OptString::cstring(char *str,int size)
{
    strncpy(str,_value.c_str(),size);
    return str;
}

// OptDB /////////////////////////////////////////////////////////////

otl_OptDB::otl_OptDB(const char *name)
    : otl_OptValue(name)
{
    _DOORS_DEBUG(0,"otl_OptDB");
}

otl_OptDB::~otl_OptDB()
{
    _DOORS_DEBUG(0,"~otl_OptDB deleting " << _db.size() << " values");
    while(!_db.empty()) {
	otl_OptValuePtrSet::iterator i = _db.begin();
	_db.erase(i);
	// (*i).first is automatic, so deleted un esase
	delete (*i).second;
    }
}

void otl_OptDB::store(otl_OptValue *v)
{
    otl_OptValuePair p(v->name(),v);
    _db.insert(p);
}

otl_OptValue *otl_OptDB::fetch(const obl_String &name)
{
    otl_OptValuePtrSet::iterator i = _db.find(name);
    if(i != _db.end()) {
	return (*i).second;
    } else {
	_DOORS_DEBUG(0,"missed fetch for " << name);
	return 0;
    }
}

bool otl_OptDB::cstringSet(const char *)
{
    _DOORS_FAULT(0,"otl_OptDB::cstringSet NOT implemented");
    return false;
}

char *otl_OptDB::cstring(char *str,int size)
{
    strcpy(str,"database");
    return str;
}

// OptX11Load ////////////////////////////////////////////////////////
/*
void otl_OptX11Load(otl_OptDB *db,int *argc,char **argv)
{
    XrmInitialize();
    // create XrmOptionDesc table
    XrmOptionDescRec *table = new XrmOptionDescRec[db->size()];
    otl_OptDB::iterator i = db->begin();
    int ti = 0;
    while(i != db->end()) {
	otl_OptValue *v = otl_OptDB::value(i);
	// option
	obl_String os("-");
	os += v->name();
	table[ti].option = strdup(os.c_str());
	// specifier
	obl_String ss(".");
	ss += v->name();
	table[ti].specifier = strdup(ss.c_str());
	// style and value of option
	if(v->isToggle()) {
	    table[ti].argKind = XrmoptionNoArg;
	    table[ti].value = (XPointer)"on";
	} else {
	    table[ti].argKind = XrmoptionSepArg;
	    table[ti].value = (XPointer)0;
	}
	++i;
	++ti;
    }
    // load
    XrmDatabase xrmdb = 0;
    XrmParseCommand(&xrmdb,table,db->size(),otl_RESNAME,argc,argv);
    _DOORS_DEBUG(xrmdb,"no arguments");
    _DOORS_DEBUG(0,"argumenst left: " << *argc);
    // fill OptDB
    for(i = db->begin();i != db->end();++i) {
	otl_OptValue *v = otl_OptDB::value(i);
	XrmValue xrmv;
	char *str_class = (char *)otl_RESNAME,*str_type_return = 0;
	obl_String name(otl_RESNAME ".");
	name += v->name();
	if(True == XrmGetResource(xrmdb,
				  name.c_str(),
				  0,
				  &str_type_return,
				  &xrmv)) {
	    v->cstringSet((char*)xrmv.addr);
	}
    }
    // free
    XrmDestroyDatabase(xrmdb);
    for(ti = 0;ti < db->size();++ti) {
	delete [] table[ti].option;
	delete [] table[ti].specifier;
    }
    delete [] table;
}
*/
