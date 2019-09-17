#ifndef SYMBOLTOOLS_H
#define SYMBOLTOOLS_H

#include "demon.h"
#include <string>
#include <qstring.h>

class SymbolTools
{
public:
  SymbolTools() { }

protected:
  // Called by show_attributes and to be implemented in a descendant
  virtual void showAttribute(QString& str) = 0;

  std::string typeName(const Demon::Symbol* symbol) const;
  void show_attributes(const Demon::Symbol* symbol, bool showvalues = true);
  std::string fromInetAddr6(const Demon::InetAddr6& addr) const;
};

#endif

