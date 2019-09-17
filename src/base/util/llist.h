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

$Log: llist.h,v $
Revision 1.2  2002/07/23 14:25:23  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:40  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:37  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef LLIST_H
#define LLIST_H

#include <stdlib.h> 
#include <doors/obl_dcl.h>

/**
  A class for storing data. Required space for data is allocated
  by the constructor.
  */
class Link {
    friend class LinkList;
    friend class LinkListIterator;
public:
    /**
      Initializes pointer to next memory cell, default 0.
      Initializes memory cell length, DEFAULT_CELLSIZE. 
      Note that given length of Link (len) doesn't include
      the length of a header of Link.
      @param nextCell
      */
    Link (Link *nextCell = 0);

    virtual ~Link (void);

    /** Return next link. */
    Link *next (void) { return _next;}

    /** Set next ponter */
    void next (Link *n){ _next = n;}
protected:
    Link *_next;
};

/**
  This is a queue class of Link objects.
  @see Link
  */
class LinkList {
    friend class LinkListIterator;
public:
    /** Create new list of Links. */
    LinkList (void);

    /** Iteratively deletes memory cells from the list. */
    ~LinkList (void);
    
    /** Inserts the node to the begin of the queue */
    void insert (Link *);

    /** Appends the node to the tail of the queue */
    void append (Link *);

    /**
      Puts link in the list. The desired position in the list 
      is user-dependant and is controlled by the cmpFunc function.
      @param link
      @param cmp
      @return true if operation succeed, false otherwise.
      */
    bool add (Link *link, bool (*cmp)(const Link *, const Link *));

    /**
      Returns data from the list. The type of the data is user-dependant
      and can be controlled by the cmpFunc function.
      */
    Link *remove (void *, bool (*)(const Link *, const void *));

    /** 
      Removes link from the list 
      @return Pointer to the removed link.
      */
    Link *remove (Link *link);

    /** Returns and removes the first element of the queue. */
    Link *remove (void);
    
private:
    static bool sameObject(const Link *, const void *);
    class Link *first;
    class Link *last;
};

/**
  LinkListIterator does iterative manipulations of LinkList.
  @see LinkList
  */
class LinkListIterator {
public:
    /** Initializes the class members */
    LinkListIterator (const LinkList * = 0);
    ~LinkListIterator (void);

    /** 
      Copies data members from the right hand side object to itself and
      returns its own reference
      */
    LinkListIterator &operator = (const LinkListIterator &);

    /** Resets the cursor back to the first node of the queue. */
    void reset (void);

    /** Returns pointer to the next data element in the queue, zero,
      if end of queue. */
    Link *next (void);

    /** Returns pointer to the Link if any elements in the queue.
      Leaves the Link into the queue. */
    Link *head (void) const;

    /** Returns pointer to the last element in the queue.
      Leaves the Link into the queue. */
    Link *last (void) const;

    /** Return pointer length of list. */
    Load length (void) const;
private:
    const LinkList *host;
    Link *cursor;
};

#endif



