// -*- C++ -*-

#ifndef _otl_dcl_h
#define _otl_dcl_h 1

#include <doors/obl_dcl.h>

#define _xcat3(a,b,c)     a##b##c
#define _cat3(a,b,c)      _xcat3(a,b,c)
#define _xcat5(a,b,c,d,e) a##b##c##d##e
#define _cat5(a,b,c,d,e)  _xcat5(a,b,c,d,e)
#define _xtostr(s)        #s
#define _tostr(s)         _xtostr(s)

class otl_Sobject;
class otl_SobjectImpl;
class otl_ToolKit;
class DebugScheduler;
class Alias;
// class AliasList;
class Command;
// class CommandList;
class LEMonHandler;
class UiTask;
class GptMsgTool;
class GptTaskObs;
class GptUser;
class GptApplication;
class MscModel;
class MscView;
class GptHandler;
class XbApplication;
class XbDisplay;
class XbComponent;
class Xb;
class XbShell;
class XbObserver;
class XbSubject;
class XbDisplayShell;


#include <vector>
#include <list>
#include <set>

template<class Object> class TQueue;
template<class Object> class TQueueListIterator;
template<class T> class Handler;


class Handler<Iface>;
class Handler<Message>;

typedef std::vector<char*> StrSet;
typedef std::vector<Handler<Iface>* > IfaceHandSet;
typedef std::vector<Handler<Message>* > MessageHandSet;
typedef std::list<GptHandler*> GptHandlerList; 
typedef std::list<GptUser*> GptUserPtrSeq;
typedef std::set<otl_Sobject*,std::less<otl_Sobject*> > otl_SobjectPtrSet;

typedef TQueue<Frame> HistoryList;
typedef TQueueListIterator<Frame> HistoryListIterator;

//+ WordList
//. WordList and WordListIterator are synonyms for
//. TQueue:d Name objects (obu_String objects indeed).
//. These are removed as the Name is.
//:syntax:
//typedef TQueue<obl_String> WordList;
//typedef TQueueListIterator<obl_String> WordListIterator;
//-

#define GPT_XCLASS      "Gpt"
#define GPT_NAME        "Graphical Protocol Tracer"
#define GPT_VERSION_CAB 00 // cabability
#define GPT_VERSION_IFC 01 // interface
#define GPT_VERSION_IMP 00 // implementation
#define GPT_VERSION_TAG \
 _tostr(_cat5(GPT_VERSION_CAB,.,GPT_VERSION_IFC,.,GPT_VERSION_IMP))
#define GPT_VERSION_NUM \
 _cat3(GPT_VERSION_CAB,GPT_VERSION_IFC,GPT_VERSION_IMP)

#undef _xcat3
#undef _cat3
#undef _xcat5
#undef _cat5
#undef _xtostr
#undef _tostr

#endif // _otl_dcl_h
