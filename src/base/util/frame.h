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

$Log: frame.h,v $
Revision 1.3  2006/12/12 22:57:58  bilhanan
Added getString ()

Revision 1.2  2002/06/03 09:19:55  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:31  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.5  2001/06/08 08:39:36  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef FRAME_H
#define FRAME_H


#include <doors/cell.h>
#include <string>

/** Type of amount of bytes frame stores */
typedef Sint32 FrameSize;

/**
  A frame is an octet buffer. It is mainly used in protocol
  implementations to store the SDU. The frame consists of 
  one linked list of cells. The cell is an internal data object
  of the frame. Its purpose is to hold actual data. The frame
  dynamically allocates new cells if needed. 
  
  The frame provides two ways to access data in a frame.
  PutFirst, getFirst, putLast, and getLast is an easy interface
  to use the frame. It is good way to use for beginners, but
  it is inefficent. A professional way is to use so called
  address functions, address, space, data, etc. They are
  efficient especially when you are implementing coding functions
  for large PDUs but you must be more careful using them. 
  
  A class for storing dynamic data, octets. Octets are kept in 
  memory cells. The frame maintains a linked list of memmory
  cells keeping the number of allocated memory cells as low
  as possible.
*/
class Frame {
  friend class FrameIterator;
public:
  enum FrameCopyType {copyFrame, moveFrame};
  
  /**
     The constructor initializes the firstCell pointer to zero and
     stores the preferred size of a cell to the suggestedSize variable, 
     which is not currently used.
     
     @param requestedSize The requestedSize parameter is the size of a 
     suggested memory block used in the internal memory block list.
  */
  Frame (CellSize requestedSize = DEFAULT_CELLSIZE);
  
  /** 
      The copy constructor of a frame. Data in the frame is copied into 
      the newly created frame. 
  */
  Frame (const Frame &f);
  
  /**
     The constructor of a frame. Data in the frame is copied into 
     the newly created frame.
     @param f
     @param flag
  */
  Frame (Frame &f, FrameCopyType flag = copyFrame);
  
  /** Recursively deletes all cells in the frame, if any. */
  ~Frame (void);
  
  /**
     <b>Moves</b> the contents of a frame rhs to a new frame. If there is
     data in this frame it is freed. The operator changes the
     suggested cell size, also. Note this has diffrent sematics, than
     int a = b; 
     @param rhs The source frame, that is empty after operation.
  */
  void operator = (Frame &rhs);
  
  /**
     Copies the contents of the frame to a continuous Byte array 
     that is allocated. The size of the array is exactly the same 
     as the size of the frame. 
     @return A pointer to the <b>created</b> memory area is returned. 
     Returns zero if the frame is empty or if memory allocation fails.
     Note: the returned memory have to be deleted by user.
  */
  operator Byte * (void) const;
  
  /** Returns the number of octets in the frame. */
  FrameSize length (void) const;
  
  /** Returns the suggested size of the frame. */
  FrameSize getSize (void) const;
  
  /** 
      Inserts one octet to the head of the frame. A new memory cell
      is allocated if there is no memory cell or if it is full. 
      @param octet
  */
  void putFirst (Byte octet);
  
  /**
     Inserts octets to the head of the frame. Octets are copied into 
     the frame in the same order as they are in the string. Copied
     octets are not necessarily in a continuous memory area in the
     frame. 
     @param octet
     @param lenght
  */
  void putFirst (const Byte *octet, FrameSize length);
  
  /**
     Inserts one byte to the tail of the frame. A new memory cell
     is allocated if there is no cell or if it is full. Note that the 
     current implementation is ineffective when storing data
     massively into the end of the frame.
  */
  void putLast (Byte octet);
  
  /**
     Inserts octets to the tail of the frame. Octets are copied into 
     the frame in the same order as they are in the string. Copied
     octets are not necessarily in a continuous memory area in the
     frame. 
  */
  void putLast (const Byte *octet,FrameSize length);
  
  /**
     Removes one byte from the head of the frame.
     
     @return Returns the octet fetched from the frame. 
     If the frame is empty and 
     the function is called, zero is returned. Note that it is 
     impossible to know if the frame is empty from the return
     value. 
  */
  Byte getFirst (void);
  
  /** */
  Byte *getFirst (FrameSize length, Byte* buff = NULL);
  
  void delFirst( FrameSize length ){
    Byte *tmp = getFirst (length);
    delete tmp;
  };
  
  /**
     Removes one byte from the tail of the frame. The function
     removes also the last memory cell if it becomes empty. 
     @return The octet fetched from the frame. If the frame is empty zero
     is returned.
  */      
  Byte getLast (void);
  
  /** */
  Byte *getLast (FrameSize length);


  /**
     Removes a sequence of bytes specified by the user from the front of 
     the frame and passed in a C++ string reference. If the length is not
     specified, all the bytfroes in the frame would be transferred into string.
     No type checking is performed to ensure only valid characters are
     passed into the string.
     @return The number of bytes removed from the frame are returned. Note
     that this may differ from the resulting size of the string if a NULL value 
     is present as bytes in the frame.
     
  */      
 
  FrameSize getString (std::string &ResultString, FrameSize strlength=0);
    
  
  /**
     Reads a byte at the specified zero based location offset
     in the frame. 
      @param offset
      @return Returns the byte fetched from the frame, or zero if the 
      location is outside of the frame.
  */
  Byte read (FrameSize offset) const;
  
  /**
     Writes an octet to the zero based specified location offset 
     into the frame.
     Writes nothing if the location is outside the frame. It is
     not efficient to write massive amount of data using this 
     function. 
     @param octet
     @param offset
  */
  void write (Byte octet, FrameSize offset);
  
  /**
     Gives the address to the first Cell of the frame. The address
     is the address of the last free byte in the Cell from the 
     beginning of the frame and before the data. Free space of 
     the Cell is placed into a variable of which the address is 
     given as parameter, if it is not zero. This function is used 
     to access octets stored into the frame or to insert octets
     into the head of the frame. A user must not use more octets 
     than there are free or used by the frame and she must herself 
     allocate more cells by calling the space function, if needed.
     
     @param space is a pointer to the variable where the size of 
     free space in the frame will be stored, and could be a zero pointer.
     
     @return The pointer to the internal memory cell of the frame is 
     returned. Zero is returned if the frame is empty.
  */
  Byte *address (FrameSize *space = 0);
  
  /**
     Gives the address to the last Cell of the frame. The address 
     is the address of the first free byte in the Cell after the data. 
     Free space of the Cell is placed to a variable of which address is 
     given as parameter, if it is not zero. The function is used to
     access octets stored into the frame or to append more octets
     into the end of the frame. A user must not use more octets 
     than there are free or used by the frame and she must herself 
     allocate more cells by calling the endSpace function,
     if needed.
     
      @param space is a pointer to variable where the size of free space
      in the frame will be stored, could be zero pointer.
      
      @return The pointer to the internal memory cell of the frame. Zero
      is returned if the frame is empty.
  */
  Byte *endAddress (FrameSize *space = 0);
  
  /**
     Co-routine of the address function. Update function is used to
     tell the frame what have been done with the address that was
     given the address function. Positive movement means that bytes 
     have been inserted. Negative movement means that bytes have
     have been removed.
     
     Use of update function is also needed after space function
     to update e.g. the length of frame.
     @param movement
     @see address
  */
  void update (FrameSize movement);
  
  /**
     Co-routine of the endAddress function. endUpdate function is used to
     tell the frame what have been done with the address which was
     given the endAddress function. Positive movement means that bytes 
     have been inserted. Negative movement means that bytes have
     have been removed.
     
     Use of update function is also needed after endSpace function
     to update e.g. the length of frame.
     
     @param movement
  */
  void endUpdate (FrameSize movement);
  
  /**
     Creates a new Cell to the head of the frame. It is a user's
     responsibility not to create many empty cells into a
     frame and call the update function. 
  */
  void space (void);
  
  /**
     Creates a new Cell to the tail of the frame.
     It is a user's responsibility to call the endUpdate function after
     endSpace.
      */
  void endSpace (void);

  /**
     Calculates the number of bytes stored into the first cell.
     @return The number of octets in the first cell of the frame is returned.
  */
  FrameSize data (void);
  
  /**
     Calculates the number of bytes stored into the last cell.
     @return The number of octets in the last cell of the frame. 
  */
  FrameSize endData (void);
  
  /**
     Deletes all cells, if any
      */
  void destroy (void);

  /**
     Copies the frame. 
     @return Returns a reference to the created frame that is a copy 
      of this frame.
  */
  Frame &copy (void) const;
  
  /**
     Splits the frame to two parts, or frames. The first part is returned.
     If a cell must be splitted the smallest part of the cell is
     copied, only. 
     @param Offset is the number of octets splitted off the frame. 
     @return Returns a new frame that contains the part of this frame. 
  */
  Frame &split (FrameSize offset);

  /**
     Appends the given otherFrame into itself.
     @param The otherFrame is zeroed.
  */
  void combine (Frame &otherFrame);

private:
  Cell *firstCell;
  Cell *lastCell;
  CellSize suggestedSize;
};


/**
   FrameIterator is for iterative manipulations of the frame.
   The iterator can be used for reading and modifying an existing
   frame. It cannot remove or add bytes to the frame. This iterator
   should always be used when reading or writing more than one
   byte to the middle of the frame, for example when calculating
   the CRC. Using the frame iterator is more effective than using
   the read and write functions of the frame.
*/
class FrameIterator {
public:
  /**
     A constructor of an iterator class that is used to access
     or modify the contents of the frame frm. 
  */
  FrameIterator (const Frame *);
  ~FrameIterator (void);
  
  /** Moves the iterator to the offset and then reads the current octet.
      @param offset
      @return Returns the byte at the specified location of the frame.
      Returs zero if the location is outside of the frame.
  */
    Byte read (FrameSize offset);

    /**
      Moves the iterator to the offset and then writes the
      given octet.
      @param octet
      @param offset
      */
    void write (Byte octet, FrameSize offset);
	
    /** 
      Resets the iterator to the beginning of the frame. Used, for
      example, when the contents of the frame have changed. The
      iterator cannot know if the cell to which the iterator points
      has been removed. 
      */
    void reset (void);

    /**
      Peeks the current position of the iterator of a frame. 
      @return Returns the byte at the specified location of the frame.
      Returs zero if the location is outside the frame.
      */
    Byte read (void);

    /**
      Writes the given octet into the frame iterator's current position. 
      @param octet
      */
    void write (Byte octet);

    /** 
      Moves the iterator to the specified zero based location offset.
      @param offset
     */
    void moveCursor (FrameSize offset);

    /** 
      Returns a pointer to data in the frame and the number of 
      octets in the current cell.
      @param space is a pointer to where the number of remaining octets 
      is stored, may be zero
      @return Returns s pointer to an octet inside the frame 
      to which the iterator points. Returns zero if the frame is empty. 
      */
    Byte *address (FrameSize *space = 0);
	
    /** 
      Returns a pointer to data in the frame and the number of 
      octets in the next cell. Works as the address function but
      moves the iterator to the next non-empty cell.
      @param space is a pointer to where the number of remaining octets 
      is stored, may be zero
      @see Frame::address
      */
    Byte *nextCell (FrameSize *space = 0);

private:
    const Frame *frame; 
    Cell *currentCell;
    FrameSize cursor;
    FrameSize cellCursor;
};


#define FRAME_DATA(b,i)   *((Byte*)(b) + (i))

inline int DATA_SIZE(Cell *c) {
  return c->dataSize();
}

inline Byte FrameIterator :: read (void)
{
    Byte b = 0;
    if (currentCell) {
	b = FRAME_DATA (currentCell, currentCell->i + 1 + cursor - cellCursor);
    }
    return b;
}
    
inline void FrameIterator :: write (Byte octet)
{
    if (currentCell) {
	FRAME_DATA(currentCell, currentCell->i + 1 + cursor - cellCursor) 
	    = octet;
    }
}

inline Byte FrameIterator :: read (FrameSize offset)
{
    moveCursor (offset);
    return read ();
}
    
    
inline void FrameIterator :: write (Byte octet, FrameSize offset)
{
    moveCursor (offset);
    write (octet);
}

#endif  //FRAME_H






