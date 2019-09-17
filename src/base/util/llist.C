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

$Log: llist.C,v $
Revision 1.3  2002/07/23 14:25:23  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:55  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:36  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:37  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/llist.h>
#include <doors/debug.h>  // for ASSERT macro


#define LINK_HEADER sizeof(Link)

Link :: Link (Link *next)
{
    _next = next;
}

Link :: ~Link (void)
{
}

LinkList :: LinkList (void)
{
    first = last = 0;
}

LinkList :: ~LinkList (void)
{
    register Link *link = first, *tmp;
    while (link) {
	tmp = link->_next;
	delete link;
	link = tmp;
    }
}

void LinkList :: insert (Link *link)
{
    link->_next = first;
    first = link;
    if (!last) {
	last = link;
    }
    return;
}

void LinkList :: append (Link *link)
{
    link->_next = 0;
    if (last) {
	last = last->_next = link;
    } else {
        first = last = link;
    }
    return;
}

bool LinkList :: add
(Link *data, bool (*cmpFunc)(const Link *, const Link *))
{
    register Link *item;
    register Link *previous = 0;

    for (item = first; item; item = item->_next) {
        if (cmpFunc (item, data)) {
	    break;
	}
	previous = item;
    }

    //
    // to add to end of list, compare function must return
    // true for first argument equal to zero
    //
    if (!item) {
	if (!cmpFunc (0, data)) {
	    return false;
	}
	if (last) {
	    last->_next = data;
	}
	last = data;        
    }	

    if (previous) {
	previous->_next = data;
    } else {
	first = data;
    }
    data->_next = item;

    return true;
}

Link *LinkList :: remove (void)
{
    register Link *item = first;

    if (item) {
        if (item == last) {
            first = last = 0;
        } else {
            first = item->_next;
        }
	item->_next = 0;
    }
    return item;
}

bool LinkList :: sameObject(const Link *a, const void *b) 
{
    return (a == b) ? true : false;
}

Link *LinkList :: remove (Link *link)
{
    return
      remove ( link, (bool (*)(const Link *a, const void *b)) sameObject);
}

Link *LinkList :: remove 
(void *data, bool (*cmpFunc)(const Link *, const void *))
{
    register Link *item;
    register Link *previous = 0;

    for (item = first; item; item = item->_next) {
        if (cmpFunc (item, data)) {
	    break;
	}
	previous = item;
    }

    if (!item) {
	return 0;
    }

    if (previous) {
	previous->_next = item->_next;
    } else {
	first = item->_next;
    }

    if (item == last) {
	last = previous;
    }

    return item;
}

LinkListIterator :: LinkListIterator (const LinkList *hostQueue)
{
    cursor = 0;
    host   = hostQueue;
}

LinkListIterator :: ~LinkListIterator (void)
{
}

LinkListIterator &
LinkListIterator :: operator = (const LinkListIterator &another)
{
    cursor = another.cursor;
    host   = another.host;
    return *this;
}

void LinkListIterator :: reset (void)
{
    cursor = 0;
    return;
}

Link *LinkListIterator :: next (void)
{
    if (host) {
        if (cursor) {
	    cursor = cursor->_next;
	} else {
            cursor = host->first;
        }
    }
    return cursor;
}

Link *LinkListIterator :: head (void) const
{
    if (host) {
        return host->first;
    }
    return 0;
}

Link *LinkListIterator :: last (void) const
{
    if (host) {
        return host->last;
    }
    return 0;
}

Load LinkListIterator :: length (void) const
{
    Load len = 0;
    if (host)
      for (register Link *item = host->first; item; item = item->_next, len++);
    return len;
}

