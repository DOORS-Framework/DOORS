// -*- C++ -*-
//

#ifndef _otl_opt_h
#define _otl_opt_h 1

#include <doors/otl_base.h>
#include <doors/obl_string.h>

#include <map>

class otl_OptValue : public otl_SobjectImpl {
public:


    otl_OptValue(const char *name);
    otl_OptValue(const otl_OptValue &value);


    virtual ~otl_OptValue();


    virtual bool cstringSet(const char *) = 0;
    virtual char *cstring(char *str,int str_size) = 0;


    virtual bool isToggle() const = 0;
    virtual bool isDatabase() const = 0;
    virtual const obl_String &name() const {return _name;}

private: 
    otl_OptValue() {}
    const obl_String _name;
};


class otl_OptBool : public otl_OptValue {
public:


    otl_OptBool(const char *,bool b);
    otl_OptBool(const otl_OptBool &b);


    const bool value() const {return _value;}


    bool cstringSet(const char *);
    char *cstring(char *,int);
    bool isToggle() const {return true;}
    bool isDatabase() const {return false;}
private:
    bool _value;
};



class otl_OptString : public otl_OptValue {
public:

    otl_OptString(const char *name,obl_String &value);
    otl_OptString(const char *name,const char *value);
    otl_OptString(const otl_OptString &s);


    const obl_String &value() const {return _value;}


    bool cstringSet(const char *);
    char *cstring(char *,int);
    bool isToggle() const {return false;}
    bool isDatabase() const {return false;}
private:
    obl_String _value;
};


//typedef std::map<const obl_String, otl_OptValue*, std::less<const obl_String> >otl_OptValuePtrSet;  
typedef std::map<obl_String, otl_OptValue* > otl_OptValuePtrSet;

typedef std::pair<const obl_String,otl_OptValue*> otl_OptValuePair;

// otl_OptDB

// once OptValue object pointer is given to OptDB, the OptDB
// manages the object. When OptDB is deleted, it subsequently
// deletes the stored OptValue's as well. User, who has created
// OptValue or not, may not delete any store OptValue.

class otl_OptDB : public otl_OptValue {
public:
  //  typedef otl_OptValuePtrSet::iterator iterator;

    otl_OptDB(const char *name);


    ~otl_OptDB();
    


    void store(otl_OptValue *);
    otl_OptValue *fetch(const obl_String &s);


    otl_OptValuePtrSet::iterator begin() {return _db.begin();}
    otl_OptValuePtrSet::iterator end() {return _db.end();}
    static otl_OptValue *value(otl_OptValuePtrSet::iterator &i) {return (*i).second;}


    int size() {return _db.size();}



    bool cstringSet(const char *);
    char *cstring(char *,int);
    bool isToggle() const {return false;}
    bool isDatabase() const {return true;}
private:
    otl_OptValuePtrSet _db;
};
//-


//+ otl_OptX11Load
//. Load X11-style command line arguments into database.

//extern void otl_OptX11Load(otl_OptDB *db,int *argc,char **argv);



#endif
