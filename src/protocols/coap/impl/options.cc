#include <iostream>
#include <sstream>
#include <string>

#include "coapconstants.hh"
#include "options.hh"

using namespace std;

// empty constructor
Options::Options()
{
}
// empty destructor
Options::~Options()  
{
}

void Options::readOptions(Frame& options, Uint16 optionCount)
{

   options_map.clear();

   //   std::string optionValue;
   Frame optionValue;
   Uint32 optionNumber = 0;
   FrameSize optionLength, optvalbytesread;

   Byte optfirstbyte = 0;
   Byte optdeltabits = 0; 
   Byte temp;

    for (int i = 0; i < optionCount; i++) {
      optfirstbyte = options.getFirst();
      optdeltabits = (optfirstbyte & 240) >> 4;
      optionNumber += optdeltabits;

      temp = optfirstbyte & 15;
      if (temp < 15) {
	optionLength = (FrameSize) temp;
      } else {	
	temp = options.getFirst();
	if (temp < 255) {
	  optionLength = ((FrameSize)temp + 15);
	} else {
	  temp = options.getFirst();
	  optionLength = ((FrameSize)temp + 270);
	}
      }
      //      optvalbytesread = options.getString(optionValue, optionLength);
      optionValue = options.split (optionLength);

      options_map.insert(std::make_pair(optionNumber, optionValue));
    }

}

bool Options::checkCritical()
{
   return true;
}

Frame& Options::getOption(Uint32 optionNumber)
{
   return options_map[optionNumber];
}

