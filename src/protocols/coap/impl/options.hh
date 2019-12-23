#ifndef OPTIONS_H
#define OPTIONS_H

#include <doors/frame.h>
#include <map>


class Options {

  public:
   Options();
   ~Options();

   // Used for reading through the options and storing them in a map
   void readOptions(Frame& options, Uint16 optionCount);

   // Returns the value for provided option Number
  //   std::string getOption(Uint32 optionNumber);
  Frame& getOption(Uint32 optionNumber);
   // Used for checking if the option is critical or Not
   bool checkCritical();


  private:

   // map to store the options and there corresponding values
  //   std::map<Uint32, std::string> options_map;
  std::map<Uint32, Frame> options_map;
};

#endif // OPTIONS_H
