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

$Log: cell.C,v $
Revision 1.3  2002/08/14 09:31:42  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/06/03 09:19:55  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:35  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.5  2001/06/08 08:39:35  bilhanan
Full licence clause added into doors 0.1


*/

#include <string.h>
#include <doors/obl_dcl.h>
#include <doors/debug.h>
#include <doors/cell.h>

// A macro that calculates the length of the given cell.
#define CELL_HEADER sizeof(Cell)


const CellSize startPosition = 0;

Cell :: Cell (Link *nextCell, CellSize size, CellSize start, CellSize end) 
    : Link (nextCell), length (size)
{
    _DOORS_FAULT(length,"bad size");
    if (start) {
        i = start;
    } else {
        i = length - 1 - startPosition;
    }
    if (end) {
        j = end;
    } else {
        j = length - 1 - startPosition;
    }
}

Cell :: ~Cell (void)
{
    if (_next) {
        delete (Cell*) _next;
    }
}

void *Cell :: operator new (size_t ownSize, CellSize dataSize)
{
    void *tmp = (void *) new Byte [(size_t)(ownSize + dataSize)];
    _DOORS_FAULT(tmp,"cannot allocate memory");
    return tmp;
}

void *Cell :: operator new (size_t ownSize)
{
    void *tmp = (void *) new Byte [(ownSize)];
    _DOORS_FAULT(tmp,"cannot allocate memory");
    return tmp;
}

Cell *Cell :: copy (void)
{
    Cell *cell = 0;
    if (_next) {
        cell = ((Cell*) _next)->copy ();
    }
    _DOORS_FAULT(length,"bad size");
    Cell *clone = new (length - CELL_HEADER) Cell(cell, length, i, j); 
    _DOORS_FAULT(clone,"cannot allocate memory");
    memcpy ((Byte*)clone + i+1, (Byte*)this + i+1, j - i);
    return clone;
}








