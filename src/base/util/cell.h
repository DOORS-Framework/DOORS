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

$Log: cell.h,v $
Revision 1.2  2002/06/03 09:19:55  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:25  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.5  2001/06/08 08:39:35  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef CELL_H
#define CELL_H

#include <doors/llist.h>

/** Type of size of bytes in Cell */
typedef unsigned int CellSize;

/** Size of cell used by default */
#define DEFAULT_CELLSIZE 1024

/** Maximum size of cell */
#define MAX_CELLSIZE 60000

class Cell;

/**
  A class for storing data in frames. A cell class is used
  as a header of a memory cell. Actual data is located just after
  the header. The operator new of Cell allocates needed 
  space for Cell itself and data. Only the frame uses this
  class. The frame links these cells together as a linked list.
  @see Frame
  @see FrameIterator
  */
class Cell : public Link {
    friend class Frame;
    friend class FrameIterator;
public:
    /**
      Initializes the i, j, and the memory cell with given parameters. 
      The position from which the cell is started to fill can be 
      controlled by modifying the startPosition variable. 

      @param nextCell The nextCell parameter is a pointer to the next cell, 
      default value is zero. 
      @param size
      @param start Start is an initial value for i, default zero.
      @param end End is an initial value for j, default zero.
      */
    Cell (Link *nextCell = 0, CellSize size = 0, CellSize start = 0, 
	  CellSize end = 0);

    /** Deletes all cells linked together, if any. */
    ~Cell (void);

    /**
      Clones all cells that are linked together with the next 
      pointer in the memory cell class. The function is typically 
      used by the copy constructor of the frame. 
      @return Returns a pointer to the linked list of copied cells.
      */
    Cell *copy (void);

    /** Return size of cell. */
    CellSize getLength (void) { return length; }

    /**
      Allocates memory area for memory cell.
      @param ownSize
      @param dataSize
      @return memory
      */
    void *operator new (size_t ownSize, CellSize dataSize);

    // This function will not be used anywhere and exists to overcome
    // compiler error in HP UX 9.X C++ A.3.65
    void *operator new (size_t ownSize);

    /** Return size of data contained in frame */
    int dataSize(void) {return j - i;};  
private:
    CellSize i;
    CellSize j;
    CellSize length;
};



#endif  // CELL_H

