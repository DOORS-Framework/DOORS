#ifndef SLPEVENT_H
#define SLPEVENT_H

#include <qevent.h>
#include <string>

#define QEVENT_SLP 366434

class SLPEvent : public QCustomEvent 
{
 public:
  typedef enum { NewAgent } SlpEventType;

  SLPEvent(SlpEventType type) : QCustomEvent (QEVENT_SLP), type_(type) { }

  SlpEventType eventtype() const { return type_; };

  void setIOR(std::string i) { ior_ = i; }
  void setAgent(std::string a) { agent_ = a; }

  std::string IOR() const { return ior_; }
  std::string agent() const { return agent_; }

 private:
  SlpEventType type_;
  std::string ior_;
  std::string agent_;
};

#endif

