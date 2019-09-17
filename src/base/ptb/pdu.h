//
//  Copyright 1999
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
//
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

$Log: pdu.h,v $
Revision 1.2  2002/06/03 09:19:49  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:50  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:30  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef PDU_H
#define PDU_H


#include <doors/core.h>


//
//Class: ICI
//
//Description:
//   Some cases bare PDU doesn't contain all information
//   required to process it in task's automaton but the ICI
//   part of lower layer indication is also needed. For
//   those situations user should define an ICI data
//   structure inherited from this class to hold that
//   information and attach it to the PDU. The ICI structure
//   may also be useful when PDU is encoded into primitive
//   of lower layer.
//
//   ICI class is an abstract base class for all ICI data
//   structures user has to define. This class has not any
//   functionality, its purpose is only define a common base
//   class for ICI:s.

class ICI : public Message {
  public:
};


//
//Class: PDU
//
//Description:
//   PDU class is an abstract base class for all PDU:s the
//   protocol specifies. It defines pure virtual functions 
//   for encoding and decoding. PDU class is inherited from 
//   Message class because it is treated like any message 
//   from DOORS system's perspective.
//   
//   When user defines PDU:s of protocol he must inherit
//   them from this class. The inherited class should
//   contain all fields of the PDU as a data members of the
//   class. Furthermore, in the derived class the encode and
//   decode functions of the PDU must be implemented.


class PDU : public Message {
  public:
    PDU (void);
    virtual ~PDU (void);

    ICI *getIci (void);
    void setIci (ICI *str);

    virtual Uint16 decode (Message *msg) = 0;
    virtual Message *encode (void) = 0;

  protected:

  private:
    ICI *ici;
};


//
//Function: decode
//
//Member-Of: PDU
//
//Parameters:
//   Message *msg
//
//Return:
//   Uint16
//
//Description:
//   This functions is a pure base function for concrete
//   decode function of the PDU user defines. This function
//   doesn't have implementation but in inherited PDUs it
//   should:
//   
//   1. Decode a frame representing a SDU from given
//   primitive of lower layer. During decoding the data
//   fields should be set into the corresponding data
//   members of the PDU class.
//   
//   2. If the primitive contains some interface control
//   information (ICI) needed in PDU execution the ICI
//   structure must be created, constructed and attached
//   also to the PDU class before quitting.
//   
//   3. Return a number of bytes decoded from
//   primitive's SDU.
//   
//   Parameter msg is a primitive received from lower layer
//   and which will be decoded in this function.
//   
//   This function should return a number of decoded bytes.
//   


//
//Function: encode
//
//Member-Of: PDU
//
//Return:
//   Message *
//
//Description:
//   This functions is a pure base function for concrete
//   encode function of the PDUs user defines in his
//   protocol. This function doesn't have implementation but
//   in inherited PDUs it should:
//   
//   1. Create a new lower layer primitive need to send this
//   PDU.
//   
//   2. Construct that primitive by using data members of
//   this PDU class.
//   
//   3. Return created primitive.
//   
//   This function should return a primitive of lower layer
//   needed to send this PDU class.
//   


#endif  // PDU_H
