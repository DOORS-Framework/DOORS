#include <sys/types.h>
#include <unistd.h>
#include <slp.h>
#include <sys/time.h>
#include <deque>
#include <list>
#include <string>
#include <fstream>
#include <algorithm>
#include "slpevent.h"
#include "mgrslpclnt.h"

#include "mgrslpclnt.moc.h"

// Static functions
static SLPBoolean SLPSrvCallback(SLPHandle handle, const char* srvtypes, 
				 unsigned short lifetime, SLPError error, 
				 void* cookie);
static SLPBoolean SLPAttCallback(SLPHandle handle, const char* attrlist, 
				 SLPError error, void* cookie);

// Static variables
static std::list<std::string> ior_;
static std::list<std::string> agent_;
static SLPHandle slp_ = 0;
static std::string url_;

// #define IOR_TO_FILE

MgrSlpClnt::MgrSlpClnt(QObject *sigrec, QWidget *parent, const char *name)
  : QPushButton(parent, name), sigrec_(sigrec)
{


  // Make sure the lists are empty und set slp handle to zero
  ior_.clear();
  agent_.clear();
  slp_ = 0;

  // connect the clicked() signal inherited from QPushButton to our
  // slotClicked() method
  connect(this, SIGNAL(clicked()), SLOT(slotClicked()));
  
  // call the start() method inherited from QThread... this starts
  // execution of the thread immediately
  //  QThread::start();

}

MgrSlpClnt::~MgrSlpClnt()
{
  // Wait in case there's a thread still running
  // TODO: check this
  //  wait();

  // Close SLP
  if (slp_) {
    SLPClose(slp_);
    slp_ = 0;
  }

  // Empty the lists
  ior_.clear();
  agent_.clear();
}

void 
MgrSlpClnt::slotClicked()
{
  // Disable update button until SLP is checked
  setEnabled(false);

#ifndef IOR_TO_FILE
  // Start a new thread to poll SLP
  QThread::start();
#else
  ifstream file("/tmp/agent.ref");
  if (file) {
    file >> url_;
    std::string ior;
    file >> ior;
    std::list<std::string>::iterator i = std::find(ior_.begin(),
						   ior_.end(),
						   ior);
    if (i == ior_.end()) {
      ior_.push_back(ior);
      SLPEvent* event = new SLPEvent(SLPEvent::NewAgent);
      event->setIOR(ior);
      event->setAgent(url_);
      QThread::postEvent(sigrec_, event);
      std::cout << "SLP Event posted." << std::endl;
    }
  }
  file.close();
  setEnabled(true);
#endif
}


void
MgrSlpClnt::run()
{
  SLPError slperr;

  slperr = SLPOpen("default", SLP_FALSE/*=sync*/, &slp_);
  
  if (slperr == SLP_OK) {
    std::cerr << "Trying SLP..." << std::endl;
    agent_.clear();
    slperr = SLPFindSrvs(slp_, "service:demon", "default", "", 
			 &SLPSrvCallback, (void*)this);
    if (slperr == SLP_OK) {
      std::cerr << "Success with SLP." << std::endl;
    } else {
      std::cerr << "No success with SLP." << std::endl;
    }

    // Loop through the agent addresses and get attributes for them
    for (std::list<std::string>::iterator i = agent_.begin();
	 i != agent_.end(); i++) {
      url_ = *i;
      std::cout << " url_ = " << url_ << std::endl;
      slperr = SLPFindAttrs(slp_, url_.c_str(), "default", "",
			    &SLPAttCallback, sigrec_);
    }

  }

  // Enable Update SLP button again; we are ready to run again
  setEnabled(true);

}

SLPBoolean 
SLPSrvCallback(SLPHandle handle, const char* srvtypes, 
	       unsigned short lifetime, SLPError error, 
	       void* cookie)
{
  std::cerr << "Entering SLPSrvCallback" << std::endl;
  if (error == SLP_OK) {
    std::cout << "srvtype = " << srvtypes << ", lifetime = ";
    std::cout << lifetime << std::endl;

    // Save service name
    agent_.push_back(srvtypes);

    return SLP_TRUE;
  } else {
    if (error == SLP_LAST_CALL) {
      std::cerr << "Last call to server callback function." << std::endl;
    } else {
      std::cerr << "SLP server callback failed. error = " << error << std::endl;
    }
  }

  return SLP_FALSE;
}

SLPBoolean 
SLPAttCallback(SLPHandle handle, const char* attrlist, 
	       SLPError error, void* cookie)
{
  if (error == SLP_OK) {
    std::cout << "ior = " << attrlist << std::endl;

    // Search the list of the old IOR's if this is already there
    std::list<std::string>::iterator i = std::find(ior_.begin(),
						   ior_.end(),
						   attrlist);
    if (i == ior_.end()) {
      ior_.push_back(attrlist);

      // Post an event of finding a new agent to main window
      QObject* sigrec = (QObject*)cookie;
      if (sigrec) {
	SLPEvent* event = new SLPEvent(SLPEvent::NewAgent);
	event->setIOR(attrlist);
	event->setAgent(url_);
	QThread::postEvent(sigrec, event);
	std::cout << "SLP Event posted." << std::endl;
      }

    }

  } else {
    std::cerr << "SLP attribute callback failed." << std::endl;
  }

  return SLP_FALSE; // No use calling this callback again
}


