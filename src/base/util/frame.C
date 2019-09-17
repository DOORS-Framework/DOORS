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

$Log: frame.C,v $
Revision 1.7  2009/04/20 13:44:26  bilhanan
bugfix to allow c-style string manipulation

Revision 1.6  2007/02/26 10:13:30  bilhanan
cleaned up and debugged getString()

Revision 1.5  2006/12/12 22:57:57  bilhanan
Added getString ()

Revision 1.4  2002/10/02 10:28:09  ik
Pointer cast from Cell* to int which failed with SunCC. Type int
changed to long.

Revision 1.3  2002/08/14 09:31:42  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/07/23 14:25:23  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:36  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.5  2001/06/08 08:39:36  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/frame.h>
#include <doors/debug.h>
#include <doors/obl_dcl.h>
#include <string>
#include <cstring>

#define CELL_HEADER       sizeof(Cell)

#define FAST_COMBINE      0 

#ifndef MIN
#define MIN(a, b)         ((a) < (b) ? (a) : (b))
#endif

Frame :: Frame (CellSize requestedSize)
    : firstCell(0),lastCell(0)
{
     firstCell = lastCell = 0;
     suggestedSize = requestedSize;
     _DOORS_FAULT(suggestedSize,"bad cell size");
}

Frame :: Frame (const Frame &frame)
    : firstCell(0),lastCell(0)
{
    Cell *cell = 0;
    suggestedSize = frame.suggestedSize;
    if (frame.firstCell) {
	firstCell = cell = frame.firstCell->copy ();
        while (cell) {
            lastCell = cell;
            cell = (Cell*) cell->_next;
        }
    } else {
        firstCell = lastCell = 0;
    }
    _DOORS_FAULT((long)firstCell % 4 == 0,"bad address");
    _DOORS_FAULT(suggestedSize,"bad cell size");
}

Frame :: Frame (Frame &frame, FrameCopyType flag)
    : firstCell(0),lastCell(0)
{
    suggestedSize = frame.suggestedSize;
    if (flag == copyFrame) {
	Cell *cell = 0;
	if (frame.firstCell) {
	    firstCell = cell = frame.firstCell->copy ();
	    while (cell) {
		lastCell = cell;
		cell = (Cell*) cell->_next;
	    }
	} else {
	    firstCell = lastCell = 0;
	}
    } else {
	firstCell = frame.firstCell;
	lastCell = frame.lastCell;
	frame.firstCell = frame.lastCell = 0;
    }
    _DOORS_FAULT((long)firstCell % 4 == 0,"bad address");
    _DOORS_FAULT(suggestedSize,"bad cell size");
}

Frame :: ~Frame (void)
{
    if (firstCell) {
        delete firstCell;
    }
}

void Frame :: operator = (Frame &rhs)
{
    if (firstCell) 
    {
        delete firstCell;
    }
    firstCell = rhs.firstCell;
    lastCell = rhs.lastCell;
    suggestedSize = rhs.suggestedSize;
    rhs.firstCell = rhs.lastCell = 0;
    _DOORS_FAULT((long)firstCell % 4 == 0,"bad address");
    _DOORS_FAULT(suggestedSize,"bad cell size");
}

Frame :: operator Byte * (void) const
{    
    Byte *buf = 0;
    if (firstCell) 
    {
	if ((buf = new Byte[length() + 1])) 
        {
	    FrameSize i = 0;
	    for (Cell *cell = firstCell; cell; cell = (Cell *) cell->_next) 
            {
		CellSize len = DATA_SIZE (cell);
		memcpy (buf + i, (Byte*)cell + cell->i + 1, (size_t)len);
		i += len;
	    }
	    *(buf + i) = 0;
        }
    }
    return buf;
}

FrameSize Frame :: length (void) const
{
    register Cell *cell = firstCell;
    register FrameSize frameLength = 0;

    while (cell) 
    {
        frameLength += DATA_SIZE(cell);
        cell = (Cell *) cell->_next;
    }

    return frameLength;
}

FrameSize Frame :: getSize (void) const 
{
    return suggestedSize;
}

void Frame :: putFirst (Byte octet)
{
    Cell *cell = firstCell;
    if (cell)  {                      //the frame is not empty
        if (cell->i < CELL_HEADER)  { // the first Cell is full
            firstCell = cell = 
		new(suggestedSize)Cell(cell, suggestedSize + CELL_HEADER, 0, 0);
	}
    } else  {                         // the frame is empty
	firstCell = lastCell = cell = 
	    new (suggestedSize) Cell (0, suggestedSize + CELL_HEADER, 0, 0);
    }
    _DOORS_FAULT(cell,"cannot get memory");
    FRAME_DATA(cell, cell->i) = octet;
    --(cell->i);
    _DOORS_FAULT((long)firstCell % 4 == 0,"bad address");
}

void Frame :: putFirst (const Byte *octets, FrameSize size)
{
    register Cell *cell = firstCell;

    while (size > 0) 
    {
	// allocate space
	if (cell) 
        {
	    if (cell->i < CELL_HEADER) 
		firstCell = cell = new (suggestedSize) Cell (cell, suggestedSize + CELL_HEADER, 0, 0);
	} 
        else 
	    firstCell = lastCell = cell = new (suggestedSize) Cell (0, suggestedSize + CELL_HEADER, 0, 0);

	// copy memory
	FrameSize len = cell->i - CELL_HEADER + 1;
	if (len > size) {
	    len = size;
	}
	size -= len;
	cell->i -= (CellSize) len;
	memcpy ((Byte *) cell + cell->i + 1, octets + size, (int) len);
    }
    _DOORS_FAULT((long)firstCell % 4 == 0,"bad address");
}

void Frame :: putLast (Byte octet)
{
    register Cell *cell = lastCell;

    if (cell == 0)  // there is no Cell, create one
    {
        firstCell = lastCell = cell = new (suggestedSize) Cell(0, suggestedSize + CELL_HEADER, 
        CELL_HEADER-1, CELL_HEADER-1);
    } 
    else 
    {
        if (cell->j >= (cell->length-1)) {  // check if the last Cell is full
            Cell *newCell = lastCell = new (suggestedSize) Cell (0, suggestedSize + CELL_HEADER, 
            CELL_HEADER-1, CELL_HEADER-1);
            cell->_next = newCell;
            cell = newCell;
        }
    }
    // store the octet
    ++(cell->j);
    FRAME_DATA(cell, cell->j) = octet;
    _DOORS_FAULT((long)firstCell % 4 == 0,"bad address");
}

void Frame :: putLast (const Byte *octets, FrameSize size)
{
    register Cell *cell = lastCell;

    while (size > 0) 
    {
	// allocate space
	if (cell) 
        {
	    if (cell->j >= (cell->length - 1)) 
            {
		lastCell->_next = cell = new (suggestedSize) Cell (0, suggestedSize + 
			   CELL_HEADER, CELL_HEADER - 1, CELL_HEADER - 1);
	        lastCell = (Cell *) lastCell->_next;
	    }
	} 
        else 
        {
	    firstCell = lastCell = cell = new (suggestedSize) Cell (0, suggestedSize + CELL_HEADER, 
            CELL_HEADER - 1, CELL_HEADER - 1);
	}

	// copy memory
	FrameSize len = (cell->length - 1) - cell->j;
	if (len > size) 
	    len = size;
	size -= len;
	memcpy ((Byte *) cell + cell->j + 1, octets, (int) len);
	cell->j += (CellSize) len;
	octets += len;
    }
    _DOORS_FAULT((long)firstCell % 4 == 0,"bad address");
}

Byte Frame :: getFirst (void)
{
    register Cell *cell = firstCell;
    register Byte octet = 0;
    
    if (cell && cell->i < cell->j) 
    {
	++(cell->i);
	octet = FRAME_DATA(cell, cell->i);
	if (cell->i == cell->j)  // release the empty Cell
	{
	    if (!(firstCell = (Cell*) cell->_next)) // no cell left
                lastCell = firstCell;
	    cell->_next = 0;
 	    delete cell;
	}
    }
    _DOORS_FAULT((long)firstCell % 4 == 0,"bad address");
    return octet;
}

Byte *Frame :: getFirst (FrameSize size, Byte* buff)
{
    register Cell *cell = firstCell;
    register Byte *octets = 0;
    
    if (cell && cell->i < cell->j) 
      { 
	if(buff){
	  octets = buff;
	} else{
	  octets = new Byte [size];
	}
	FrameSize length = size;
	
        while (size > 0 && cell)
	{
            FrameSize len = cell->j - cell->i;
            if (len > size)
	        len = size;
            size -= len;
	    memcpy ( octets + (length - size - len), (Byte *) cell + cell->i + 1, (int) len);
	    cell->i += (CellSize) len;

	    if (cell->i == cell->j)  // release the empty Cell
	    {
	        if (!(firstCell = (Cell*) cell->_next)) // no cell left
                     lastCell = firstCell;
	        cell->_next = 0;
 	        delete cell;
                cell = firstCell;
	    }
            
        }
    }
    _DOORS_FAULT((long)firstCell % 4 == 0,"bad address");
    return octets;
}

Byte Frame :: getLast (void)
{
    register Cell *previous = 0;
    register Cell *cell = lastCell;
    register Byte octet = 0; 

    if (cell && cell->i < cell->j) // make sure that there is data
    {
	octet = FRAME_DATA(cell, cell->j);
	--(cell->j);
	if (cell->i == cell->j)  // Cell becomes empty
	{
            cell = firstCell;
            while (cell && cell->_next)  // find the one before last Cell
	    {
        	previous = cell;
                cell = (Cell*) cell->_next;
            }

            delete lastCell;  // delete empty cell  

	    if (previous) 
            {
		previous->_next = 0;
                lastCell = previous;
	    } 
            else 
            {
		firstCell = lastCell = 0;
	    }
	}
    }
    _DOORS_FAULT((long)firstCell % 4 == 0,"bad address");
    return octet;
}

Byte *Frame :: getLast (FrameSize size)
{
    register Cell *cell = lastCell;
    register Cell *previous = 0;
    register Byte *octets = 0;
    
    if (cell && cell->i < cell->j) 
    {
        octets = new Byte [size];
        FrameSize length = size;

        while (size > 0 && cell)
	{
            FrameSize len = cell->j - cell->i;
            if (len > size)
	        len = size;
            size -= len;
	    memcpy ( octets + size, (Byte *) cell + cell->j + (1 - len), (int) len);
	    cell->j -= (CellSize) len;

	    if (cell->i == cell->j)  // release the empty Cell
	    {
               cell = firstCell;
               while (cell && cell->_next)  // find the one before last Cell
	       {
                   previous = cell;
                   cell = (Cell*) cell->_next;
               }

               delete lastCell;  // delete empty cell  

	       if (previous) 
               {
		   previous->_next = 0;
                   lastCell = cell = previous;
		   previous = 0;
	       } 
               else 
               {
		   firstCell = lastCell = cell = 0;
	       }
	    }
            
        }

        if (size > 0) // there was not enough bytes in the frame
        {
            Byte *newOctets = new Byte [length];  // allocate new wanted size string
	    memcpy (newOctets, octets + size, length -size);  // copy bytes to the head
            delete [] octets;                                 // of newOctets string
            octets = newOctets;
        }
    }
    _DOORS_FAULT((long)firstCell % 4 == 0,"bad address");
    return octets;
}

FrameSize Frame :: getString (std::string &ResultString, FrameSize strlength) {
 
  Byte *tmp = 0;
  FrameSize currentlength = length();
  FrameSize removedoctets = 0;
  if (currentlength) {
    
    if ((!strlength) || (strlength >= currentlength)) {
      removedoctets = currentlength;
    }
    else {
      removedoctets = strlength;
    }
    
    tmp = getFirst (removedoctets);
    // To avoid possible errors, let's copy to a temp string first
    std::string tmpResultString ((char*) tmp);
    ResultString = tmpResultString.substr(0,removedoctets);
    delete []tmp;
  }
  
  return removedoctets;
}

Byte Frame :: read (FrameSize offset) const
{
    register Cell *cell = firstCell;
    
    while (cell) {
        // find the cell where the offset is located
        if ((offset - DATA_SIZE(cell)) < 0) {
            return FRAME_DATA(cell, cell->i + offset + 1);
        }
        offset -= DATA_SIZE(cell);
        cell = (Cell*) cell->_next;
    }
    return 0;
}

void Frame :: write (Byte octet, FrameSize offset)
{
    register Cell *cell = firstCell;

    while (cell) {
        // find the cell where the offset is located
        if ((offset - DATA_SIZE(cell)) < 0) {
            FRAME_DATA(cell, cell->i + offset + 1) = octet;
            return;
        }
        offset -= DATA_SIZE(cell);
        cell = (Cell*) cell->_next;
    }
    return;
}

Byte *Frame :: address (FrameSize *room)
{
    register Cell *cell = firstCell;
    if (cell) {
	if (room) {
            *room = cell->i + 1 - CELL_HEADER;
	}
        return (Byte*) cell + cell->i;
    }
    if (room)
	*room = 0;
    return 0; 
}

Byte *Frame :: endAddress (FrameSize *room)
{
    register Cell *cell = lastCell;

    // calculate the free space
    if (cell) {
	if (room) {
            *room = cell->length - cell->j - 1;
	}
        return (Byte*) cell + cell->j + 1;
    }
    if (room)
	*room = 0;

    return 0;
}

void Frame :: update (FrameSize movement)
{
    if (firstCell) 
    {
        firstCell->i -= (CellSize) movement;
        // Note: i could be negative if removed too much data, error
        if (firstCell->i == firstCell->j) 
        {          
            // cell is empty now and we should destroy it
            register Cell *cell = (Cell*) firstCell->_next;
            firstCell->_next = 0;
            delete firstCell;

            if (!(firstCell = cell))
                lastCell = firstCell;
        }
    }
    _DOORS_FAULT((long)firstCell % 4 == 0,"bad address");
    return;
}

void Frame :: endUpdate (FrameSize movement)
{
    register Cell *cell = lastCell;

    if (cell) 
    {
        cell->j += (CellSize) movement;
        // No check if the user has crossed the cell boundary
	if (cell->i == cell->j) {          
	    // the cell is now empty and we should destroy it
            register Cell *fCell = firstCell;
            register Cell *prev = 0;

            while (fCell && fCell->_next) // find the cell before last one 
            {
	        prev = fCell;
                fCell = (Cell*) fCell->_next;
            }

            delete fCell;

            if (prev) 
            {
		prev->_next = 0;
                lastCell = prev;
	    } 
            else 
            {
		firstCell = lastCell = 0;
	    }
	}
    }
    return;
}

void Frame :: space (void)
{
    firstCell = new (suggestedSize) Cell(firstCell, suggestedSize + CELL_HEADER, 0 ,0);
    if (!lastCell)
        lastCell = firstCell;
    _DOORS_FAULT((long)firstCell % 4 == 0,"bad address");
    return;
}

void Frame :: endSpace (void)
{
    register Cell *cell = lastCell;

    if (cell) 
       cell->_next = lastCell = new (suggestedSize) Cell(0, suggestedSize + CELL_HEADER, 
       CELL_HEADER-1, CELL_HEADER-1);
    else 
       firstCell = lastCell = new (suggestedSize) Cell(0, suggestedSize + CELL_HEADER, 
       CELL_HEADER-1, CELL_HEADER-1);
    _DOORS_FAULT((long)firstCell % 4 == 0,"bad address");
    return;
}

FrameSize Frame :: data (void)
{
    return DATA_SIZE(firstCell);
}

FrameSize Frame :: endData (void)
{
    register Cell *cell = lastCell;

    if (cell) 
        return DATA_SIZE(cell);
 
    return 0;
}

Frame &Frame :: copy (void) const
{
    Frame *frame = new Frame (*this);
    return *frame;
}

Frame &Frame :: split (FrameSize offset)
{
    register Cell *cell = firstCell;
    register Cell *previousCell = 0;
    Frame *part = new Frame (suggestedSize);

    if (offset < 1) {
	return *part;
    }
    part->firstCell = cell;

    while (cell) {
	//check if current Cell must be splitted
        if ((offset - DATA_SIZE(cell)) < 0) 
        {  
            register Cell *cellPart = new (cell->length - CELL_HEADER) Cell (0, cell->length, 0, 0);

	    CellSize leftPart = (CellSize) offset;
	    CellSize rightPart = DATA_SIZE(cell) - (CellSize) offset;

	    // copy the smallest part of Cell, split the list of Cells
	    // to two lists (to two frames)
            if ( leftPart < rightPart )   // first part of Cell is smaller
	    {
                cellPart->i = CELL_HEADER - 1;
		cellPart->j = CELL_HEADER - 1 + leftPart;
                memcpy ((Byte*)cellPart + CELL_HEADER,
                        (Byte*)cell + cell->i + 1, leftPart);
                //cellPart->_next = 0; // done allready
		firstCell = cell;
                cell->i += leftPart;

                if (previousCell)
		    previousCell->_next = part->lastCell = cellPart;
                  else 
		    part->firstCell = part->lastCell = cellPart;
		
            } 
            else   // second part of Cell is smaller, let's copy it
	    {
		cellPart->j = cell->length - 1;
                cellPart->i = cell->length - 1 - rightPart;
                memcpy ((Byte*) cellPart + cellPart->i + 1,
                        (Byte*) cell + cell->i + 1 + offset, rightPart);

                if (!(cellPart->_next = cell->_next))
                    lastCell = cellPart;

		firstCell = cellPart;
                cell->j -= rightPart;
		cell->_next = 0;
                part->lastCell = cell;
            }
            return *part;

	    // special case - no cell splitting needed.
        } 
        else if ((offset - DATA_SIZE(cell)) == 0) 
        { 
	    if (!(firstCell = (Cell*) cell->_next))
                  lastCell = firstCell;
	    cell->_next = 0;
            part->lastCell = cell;
            return *part;
	} 

        offset -= DATA_SIZE(cell);
        previousCell = cell;
        cell = (Cell*) cell->_next;
    }

    // Special case, offset is greater than the length of the frame.
    // The whole this frame is returned as a return value.
    firstCell = lastCell = 0;
    return *part;
}

#if FAST_COMBINE

void Frame :: combine (Frame &otherFrame)
{
    register Cell *cell = lastCell;

    // other frame is combined
    if (cell) 
    {
        cell->_next = otherFrame.firstCell;
        lastCell = otherFrame.lastCell;        
    } 
    else 
    {
        firstCell = otherFrame.firstCell;
        lastCell = otherFrame.lastCell; 
    }

    // and zeroed
    otherFrame.firstCell = otherFrame.lastCell = 0;
    _DOORS_FAULT((long)firstCell % 4 == 0,"bad address");
    return;
}

#else

void Frame :: combine (Frame &otherFrame)
{
    register Cell *cell = firstCell;
    register Cell *previousCell = 0;

    while (cell && cell->_next) 
    {
	previousCell = cell;
        cell = (Cell*) cell->_next;
    }

    if (cell) 
    {
	// if other frame is empty, there is no need for modifications
	if (otherFrame.firstCell) 
        {	    
	    // check if data can be copied.
	    FrameSize firstLength = DATA_SIZE(cell);
	    FrameSize secondLength = DATA_SIZE(otherFrame.firstCell);
	    bool fitsInFirst = (cell->j + secondLength < cell->length) ? 
		true : false;
	    bool fitsInSecond = (CELL_HEADER + firstLength <= 
				    otherFrame.firstCell->i) ? true : false;

	    // copy first if 
	    //  -there is room in the second block and
	    //  -first block is smaller or there is no room in first block

	    if (fitsInSecond && (firstLength < secondLength ||!fitsInFirst)) 
            {
		otherFrame.firstCell->i -= (CellSize) firstLength;
		memcpy ((Byte *) otherFrame.firstCell + otherFrame.firstCell->i + 1,
			(Byte*) cell + cell->i + 1,
			(size_t) firstLength);
		
		if (previousCell)
                {
		    previousCell->_next = otherFrame.firstCell;
		} 
                else 
                {
		    firstCell = otherFrame.firstCell;
		}
                lastCell = otherFrame.lastCell;
		delete cell;

	    } 
            else if (fitsInFirst) 
            {
		// copy second
		memcpy ((Byte *) cell + cell->j + 1,
			(Byte *) otherFrame.firstCell + otherFrame.firstCell->i + 1,
			(size_t) secondLength);
		
		cell->j += (CellSize) secondLength;
		if (cell->_next = otherFrame.firstCell->_next)
                     lastCell = otherFrame.lastCell;
		otherFrame.firstCell->_next = 0;
		delete otherFrame.firstCell;

	    } 
            else 
            {
		// give up
		cell->_next = otherFrame.firstCell;
                lastCell = otherFrame.lastCell;
	    }

	}

    } 
    else 
    {
        firstCell = otherFrame.firstCell;
        lastCell = otherFrame.lastCell;
    }

    otherFrame.firstCell = otherFrame.lastCell = 0;
    return;
}
#endif

void Frame :: destroy (void)
{
    if (firstCell) {
        delete firstCell;
	firstCell = lastCell = 0;
    }
}

FrameIterator :: FrameIterator (const Frame *frm) 
: frame (frm), currentCell (0), cursor (0), cellCursor (0)
{
    _DOORS_ERROR(frm,"FreameIterator: bad parameter");
}

FrameIterator :: ~FrameIterator (void)
{
}

void FrameIterator :: reset (void)
{
    currentCell = 0;
    cellCursor = cursor = 0;
    return;
}

Byte *FrameIterator :: address (FrameSize *space)
{
    if (!currentCell) {
	return 0;
    }
    if (space) {
	*space = DATA_SIZE(currentCell) + cellCursor - cursor;
    }
    return (Byte *)currentCell + cursor - cellCursor + currentCell->i + 1;
}

Byte *FrameIterator :: nextCell (FrameSize *space)
{
    if (currentCell) {
        // find the next cell that is not empty
	do {
	    currentCell = (Cell *) currentCell->_next;
	} while (currentCell && !DATA_SIZE (currentCell));
    } else {
	// start from beginning if currentCell is zero
	currentCell = frame->firstCell;
	cellCursor = cursor = 0;
    }
    return address (space);
}

void FrameIterator :: moveCursor (FrameSize offset)
{
    register Cell *cell = currentCell;
    
    // we want to access data in the previous cell
    if (!cell || offset < cellCursor) {	
	// moving backward - start from beginning
	cell = frame->firstCell;
	cellCursor = 0;
    }

    // find the cell where the offset is
    while (cell && offset >= cellCursor + DATA_SIZE (cell)) {
	cellCursor += DATA_SIZE (cell);
	cell = (Cell *) cell->_next;
    }

    if (!cell || !DATA_SIZE(cell)) {
	currentCell = 0;
	cellCursor = cursor = 0;
    } else {
	cursor = offset;
	currentCell = cell;
    }
    return;
}

