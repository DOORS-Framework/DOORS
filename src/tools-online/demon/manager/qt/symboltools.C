#include <stdio.h>
#include <qstring.h>
#include <qlistview.h>
#include "symboltools.h"

std::string
SymbolTools::typeName(const Demon::Symbol* symbol) const
{
  std::string tname;

  switch(symbol->value._d()) {
  case Demon::stEventTaskBefore :
    tname = "EventTask";
    break;
  case Demon::stEventTaskAfter :
    tname = "EventTask";
    break;
  case Demon::stEventTask :
    tname = "EventTask";
    break;
  case Demon::stPTask :
    tname = "PTask";
    break;
  case Demon::stMessage :
    tname = "Message";
    break;
  case Demon::stInetAddr4 :
    tname = "InetAddr4";
    break;
  case Demon::stInetAddr6 :
    tname = "InetAddr6";
    break;
  case Demon::stLinkList :
    tname = "LinkList";
    break;
  case Demon::stState :
    tname = "State";
    break;
  case Demon::stPort :
    tname = "Port";
    break;
  case Demon::stIface :
    tname = "Iface";
    break;
  case Demon::stTimer :
    tname = "Timer";
    break;
  case Demon::stSint8 :
    tname = "Sint8";
    break;
  case Demon::stSint16 :
    tname = "Sint16";
    break;
  case Demon::stSint32 :
    tname = "Sint32";
    break;
  case Demon::stUint8 :
    tname = "Uint8";
    break;
  case Demon::stUint16 :
    tname = "Uint16";
    break;
  case Demon::stUint32 :
    tname = "Uint32";
    break;
  case Demon::stFrame :
    tname = "Frame";
    break;
  case Demon::stSystem :
    tname = "System";
    break;
  case Demon::stInt :
    tname = "Integer";
    break;
  default :
    tname = "Unknown";
    break;
  }
  return tname;
}

void
SymbolTools::show_attributes(const Demon::Symbol* symbol, bool showvalues)
{
  static char buf[256];
  Demon::InetAddr4 addr4;
  Demon::InetAddr6 addr6;
  Demon::Timer timer;
  int index = 0;
  int len = 0;

  // Name first
  QString n("name = ");
  n += QString(symbol->name);
  showAttribute(n);

  // Index
  QString i("index = ");
  i += QString("%1").arg(symbol->index);
  showAttribute(i);

  // Then attributes
  QString value;
  switch(symbol->value._d()) {
  case Demon::stPTask :
  case Demon::stEventTask :
  case Demon::stSystem :
  case Demon::stPort :
    // No values
    break;
  case Demon::stIface :
    len = symbol->value.iface().messages.length();
    if (len > 0) {
      value = "messages = ";
      for (index = 0; index < len; index++) {
	value += QString(symbol->value.iface().messages[index]);
	value += " ";
      }
      showAttribute(value);
    }
    break;
  case Demon::stEventTaskBefore :
    if (showvalues) {
      value = "state = Before run";
      showAttribute(value);
    }
    break;
  case Demon::stEventTaskAfter :
    if (showvalues) {
      value = "state = After run";
      showAttribute(value);
    }
    break;
  case Demon::stMessage :
    value = "source name = ";
    value += QString((const char*)symbol->value.message().source.parentname);
    showAttribute(value);
    value = "source port = ";
    value += QString((const char*)symbol->value.message().source.otherport);
    showAttribute(value);
    value = "destination name = ";
    value += QString((const char*)symbol->value.message().target.parentname);
    showAttribute(value);
    value = "destination port = ";
    value += QString((const char*)symbol->value.message().target.otherport);
    showAttribute(value);
    break;
  case Demon::stInetAddr4 :
    if (showvalues) {
      addr4 = symbol->value.inetaddr4();
      sprintf(buf, "IP-address = %d.%d.%d.%d", (addr4 & 0xff), 
	      ((addr4>>8) & 0xff),
	      ((addr4>>16) & 0xff), ((addr4>>24) & 0xff));
      value = (const char*)buf;
      showAttribute(value);
    }
    break;
  case Demon::stInetAddr6 :
    if (showvalues) {
      value = "IPv6-address = ";
      value += QString(fromInetAddr6(symbol->value.inetaddr6()).c_str());
      showAttribute(value);
    }
    break;
  case Demon::stLinkList :
    if (showvalues) {
      sprintf(buf, "length = %d", symbol->value.linklist());
      value = (const char*)buf;
      showAttribute(value);
    }
    break;
  case Demon::stState :
    if (showvalues) {
      sprintf(buf, "state# = %d", symbol->value.state());
      value = (const char*)buf;
      showAttribute(value);
    }
    break;
  case Demon::stTimer :
    if (showvalues) {
      timer = symbol->value.timer();
      if (timer.is_ticking) {
	sprintf(buf, "time = %f / %f",
		(double)timer.delta.seconds + 
		(double)timer.delta.useconds / 1000000.0,
		(double)timer.initial.seconds +
		(double)timer.initial.useconds / 1000000.0);
      } else {
	sprintf(buf, "time = %f",
		(double)timer.initial.seconds +
		(double)timer.initial.useconds / 1000000.0);
      }
      value = (const char*)buf;
      showAttribute(value);
    }
    break;
  case Demon::stSint8 :
    if (showvalues) {
      sprintf(buf, "value = %d", (int)symbol->value.sint8());
      value = (const char*)buf;
      showAttribute(value);
    }
    break;
  case Demon::stSint16 :
    if (showvalues) {
      sprintf(buf, "value = %d", (int)symbol->value.sint16());
      value = (const char*)buf;
      showAttribute(value);
    }
    break;
  case Demon::stSint32 :
    if (showvalues) {
      sprintf(buf, "value = %d", (int)symbol->value.sint32());
      value = (const char*)buf;
      showAttribute(value);
    }
    break;
  case Demon::stUint8 :
    if (showvalues) {
      sprintf(buf, "value = %d", (int)symbol->value.sint8());
      value = (const char*)buf;
      showAttribute(value);
    }
    break;
  case Demon::stUint16 :
    if (showvalues) {
      sprintf(buf, "value = %d", (int)symbol->value.sint16());
      value = (const char*)buf;
      showAttribute(value);
    }
    break;
  case Demon::stUint32 :
    if (showvalues) {
      sprintf(buf, "value = %d", (unsigned long)symbol->value.sint32());
      value = (const char*)buf;
      showAttribute(value);
    }
    break;
  case Demon::stFrame :
#if 0
    for (i = 0; i < symbol->value.frame().length(); i++) {
      std::cout << hex << (int)symbol->value.frame()[i] << dec << " ";
      if (i % 9 == 8) {
	//	std::cout << std::endl << "        ";
	std::cout << "... (" << symbol->value.frame().length();
	std::cout << " values)" << std::endl;
	break;
      }
    }
#endif
    break;
  case Demon::stInt :
    if (showvalues) {
      sprintf(buf, "value = %d", symbol->value.intval());
      value = (const char*)buf;
      showAttribute(value);
    }
    break;
  default :
    if (showvalues) {
      sprintf(buf, "value = %d", symbol->value._d());
      value = (const char*)buf;
      showAttribute(value);
    }
    break;
  }

}


std::string
SymbolTools::fromInetAddr6(const Demon::InetAddr6& addr) const
{
  static char buf[42];
  unsigned short s1,s2,s3,s4,s5,s6,s7,s8;

  s1 = (addr[0]&0xff)<<8|addr[1]&0xff;
  s2 = (addr[2]&0xff)<<8|addr[3]&0xff;
  s3 = (addr[4]&0xff)<<8|addr[5]&0xff;
  s4 = (addr[6]&0xff)<<8|addr[7]&0xff;
  s5 = (addr[8]&0xff)<<8|addr[9]&0xff;
  s6 = (addr[10]&0xff)<<8|addr[11]&0xff;
  s7 = (addr[12]&0xff)<<8|addr[13]&0xff;
  s8 = (addr[14]&0xff)<<8|addr[15]&0xff;

  // Convert the IPv6 standard hex string
  sprintf(buf, "%x:%x:%x:%x:%x:%x:%x:%x", 
	  s1, s2, s3, s4, s5, s6, s7, s8);

  return std::string(buf);
}
