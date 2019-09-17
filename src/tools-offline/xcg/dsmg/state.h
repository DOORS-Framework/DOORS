//Editor-Info: -*- C++ -*-

//  Copyright 2000
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.

//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
//  THIS SOFTWARE IS PROVIDED BY TAMPERE UNIVERSITY OF TECHNOLOGY AND
//  CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
//  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//  FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY
//  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/*

$Log: state.h,v $
Revision 1.1  2002/06/17 13:29:56  bilhanan
*** empty log message ***

Revision 1.2  2002/06/07 12:19:10  hartmanj
Full licence clause added


*/


// Defines class that represents a state in statemachine

#ifndef STATE_HH
#define STATE_HH

#include <iostream>
#include <string>
#include <vector>

class State {
  
public:
  
  State();
  // Constructor takes the name of the State.
  State( std::string &name );
  ~State();
  
  // Name of the State
  std::string getName() const;
  
  // Set functions to call on default action and
  // system action
  
  void setDefaultAction( const std::string &action );
  void setSystemAction( const std::string &action );
  
  // returns the states system and default actions
  std::string getDefaultAction();
  std::string getSystemAction();
  
  
private:
  
  // Default action in state
  std::string defAction_;
  // system action in state
  std::string sysAction_;
  
  // name of the state
  std::string name_;

};

#endif
