// 
//  Copyright 1999 
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
 
 
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

$Log: buffer.C,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:46:18  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:10  bilhanan
Full licence clause added into doors 0.1


*/

 

#include <new>
#include <doors/cell.h>
#include <doors/buffer.h>

//
//Constructor: Buffer
//
//Parameters:
//      Sint32 tsize
//
//Description:
//  Initializes the base class and own variables.  
//
//  The parameter tsize is the requested size of buffer. If buffer size
//  is bigger than maximum cell size, the cell size is determined to the
//  maximum. If buffer size is negative, default cell size is used for cell.  

Buffer :: Buffer (Sint32 tsize) 
  : buffer (0), space (tsize), size (tsize)
{
  if (tsize >0) {    
    Frame *frm = 0;
    if (tsize < MAX_CELLSIZE) {
      frm = new Frame (tsize);
    }
    else {
      frm = new Frame (MAX_CELLSIZE);
      space = MAX_CELLSIZE;
    }
    frame = frm;
  }
  else {
    Frame *frm = new Frame;
    frame = frm;	
    space = DEFAULT_CELLSIZE;
  }
}

//
//Destructor: Buffer
//
//Description:
//  Delete existing frame.
//

Buffer :: ~Buffer (void)
{
  if (frame) 
    delete frame;
}
//
//Function: setSize
//
//Member-Of: Buffer
//
//Parameters: 
//      Sint32 tsize
//
//Description:
//  Function sets the size of buffer. If buffer size is bigger than maximum
//  cell size, the cell size is determined to the maximum. If buffer size
//  is negative, default cell size is used for cell.  


void Buffer :: setSize (Sint32 tsize)
{
  size = tsize;

  if (tsize > 0 ) {
    Frame *frm = 0;

    if (tsize < MAX_CELLSIZE)
      frm = new Frame (tsize);
    else
      frm = new Frame (MAX_CELLSIZE);
    if (frame)
      {
	frm->combine (*frame);
        delete frame;
	frame = frm;
	space = tsize - frame->length();
      }
    else
      {
	frame = frm;
      }
  }
  else 
    {
      if (frame)
	{
	  Frame *frm = new Frame (*frame, Frame :: moveFrame);
	  delete frame;
	  frame = frm;
	  space = frame->getSize() - frame->length();
	}
      else 
	{
	  frame = new Frame;
	  space = DEFAULT_CELLSIZE - frame->length();
	}
    }  
}


//
//Function: getSize
//
//Member-Of: Buffer
//
//Returns: Sint32
//
//Description:
//  Returns the size of buffer.  

Sint32 Buffer :: getSize (void)
{
  return size;
}



