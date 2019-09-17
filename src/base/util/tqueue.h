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

$Log: tqueue.h,v $
Revision 1.3  2005/01/20 10:25:14  bilhanan
bugfix: proper mapping to SameObject comparison template function

Revision 1.2  2002/07/23 14:25:24  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:05  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:40  bilhanan
Full licence clause added into doors 0.1


*/



#ifndef TQUEUE_H
#define TQUEUE_H

#include <doors/obl_dcl.h>

const bool NotDestroyed = false;

// forward declarations

template <class Object> class TQueueListIterator;

//
//Class: Item
//
//Description:
//      This is the template queue's private list node class, which
//      is used only by the template queue class. This list node holds
//      information about the actual data object, a pointer to the next
//      node and a flag that tells whether to delete the data object
//      or not when this tqueue is deleted. 
//
template<class Object> class Item {
    friend class TQueueListIterator<Object>;
   public:
        Item (bool destroyed) : data(0), next(0), destroy(destroyed) {}
        Object *data;
        Item<Object> *next;
        bool destroy;
};


//
//Class: TQueue
//
//Description:
//      A general queue template class. The queue does not require
//      any changes to the linked object. The list structure is 
//      hidden totally. All operations are done through the member
//      functions. 
//
//      If you need to access all the data in the queue sequentially
//      use the iterator class TQueueIterator. 
//
template<class Object> class TQueue {
public:
    friend class TQueueListIterator<Object>;
        TQueue (void);
        virtual ~TQueue (void);
        void insert (Object *data, bool destroyed = true);
        void append (Object *data, bool destroyed = true);
        bool add (Object *data, 
		     bool (*cmpFunc)(const Object *, const Object *), 
		     bool destroyed = true);
        Object *remove (const void *data, 
			bool (*cmpFunc)(const Object *, const void *));
        Object *remove (Object *data);
        Object *remove (void);
        bool isEmpty (void) const {return first ? false : true;}
          //. return true if list is empty
    private:
	static bool SameObject(const Object *, const void *);
        Item<Object> *first;
        Item<Object> *last;
};


//
//Class: TQueueListIterator
//
//Description:
//      An iterator class for TQueue template class that can be used 
//      to process nodes in a template queue without taking data 
//      out of the queue. 
//
//      You must be careful when taking nodes off from the queue 
//      while iterating. The iterator cannot handle the situation 
//      where you remove the node that is pointed by the iterator and
//      after that you use the iterator. In other words, the iterator
//      does not know if the iterated queue is modified. 
//
//      This iterator is designed to be used when you need access
//      sequentially to all the members of the queue. 
//
template<class Object> class TQueueListIterator {
    public:
        TQueueListIterator (const TQueue<Object> *hostQueue = 0);
        TQueueListIterator &operator = (const TQueueListIterator &other);

	void reset (void);
        Object *next (void);
        Object *head (void) const;
        Object *last (void) const;
    private:
        const TQueue<Object> *host;
        Item<Object> *cursor;
};

//
//Constructor: TQueue
//
//Description:
//      The constructor of the queue initializes the first and last
//      pointers, which are internal varibles of the class.
//
template<class Object> TQueue<Object> :: TQueue (void) : first (0), last (0)
{
}


//
//Destructor: TQueue
//
//Description:
//	Frees memory used by the queue and its data elements. Elements
//      are destroyed only if they have been added into the queue
//      with the destroy flag on. 
//
template<class Object> TQueue<Object> :: ~TQueue (void)
{
    register Item<Object> *itemPtr = first;
    register Item<Object> *next;
    register Object *temp;

    if (itemPtr)
    {
        while (first && itemPtr->data) 
        {
            next = itemPtr->next;
            if (itemPtr->data && itemPtr->destroy) 
            {
                temp = itemPtr->data;
                itemPtr->data = 0;
                delete temp;
            }
            else
	    {
                itemPtr->data = 0;
	    }

            itemPtr = next;
        }

        itemPtr = first;
        while (itemPtr)
        {
            next = itemPtr->next;
            itemPtr->next = 0;
            delete itemPtr;

            if (itemPtr == last)
                break;

            itemPtr = next;
        }
    }
}


//
//Function: insert
//
//Member-Of: TQueue
//
//Parameters:
//      Object *object, 
//      bool destroyed
//
//Description:
//      Creates a new queue node for storing the given object. The
//      object is inserted into the beginning of the queue. 
//
//      The parameter object is the element inserted, and 
//      destroyed is a flag indicating whether the element should
//      be deleted, or not, when the element is fetched.
//      from the queue, or when the queue is deleted.
//
template<class Object> 
void TQueue<Object> :: insert (Object *object, bool destroyed)
{
    register Item<Object> *itemPtr = new Item<Object> (destroyed);

    itemPtr->data = object;
    if (first) {
	// There is stuff in the queue
        itemPtr->next = first;
        first = last->next = itemPtr;
    } else {
	// The queue is empty
        first = last = itemPtr;
        itemPtr->next = first;
    }
    return;
}


//
//Function: append
//
//Member-Of: TQueue
//
//Parameters:
//      Object *object,
//      bool destroyed
//
//Description:
//      Creates a new queue node for storing the given object. The
//      object is appended into the end of the queue. 
//
//      The parameter object is the element appended, and destroyed is 
//      a flag indicating whether the element should be deleted, or not, 
//      when the element is fetched from the queue, or when the queue 
//      is deleted.
//

template<class Object> 
void TQueue<Object> :: append (Object *object, bool destroyed)
{
    register Item<Object> *itemPtr = new Item<Object> (destroyed);

    itemPtr->data = object;
    if (last) {
	// There is stuff in the queue
        last->next = itemPtr;
        last = itemPtr;
        itemPtr->next = first;
    } else {
	// The queue is empty
        first = last = itemPtr;
        itemPtr->next = first;
    }
    return;
}
//
//Function: add
//
//Member-Of: TQueue
//
//Parameters:
//      Object *data, 
//      bool (*cmpFunc)(const Object *item, const Object *inserted), 
//      bool destroyed
//
//Return:
//      bool
//
//Description:
//      For each element in the queue calls the compare function to
//      determine if this is the right place. If the compare function
//      returns non-zero value the element is inserted in this place. 
//      Otherwise, the compare function is called with the next
//      element in the queue. 
//
//      If the element is supposed to be added to the tail of the queue or
//      a empty queue the compare function is called with zero pointer
//      and pointer to data to be inserted. In this case the compare
//      function should return non-zero value.
//
//      The parameter data is the element appended, and cmpFunc is 
//      an address of the compare function that is used to find 
//      the correct place in the queue for the element, and destroyed 
//      is a flag indicating whether the element should be deleted, 
//      or not, when the element is fetched from the queue, or when 
//      the queue is deleted.
//
//      If the element was added into the queue non-zero value is
//      returned. Otherwise zero is returned.
//
template<class Object> bool TQueue<Object> 
    :: add (Object *data, 
            bool (*cmpFunc)(const Object *item, const Object *inserted), 
            bool destroyed)
{
    register Item<Object> *itemPtr = first;
    register Item<Object> *prevPtr = 0;
    register Object *dataPtr;

    while (1) {
        dataPtr = 0;
        if (itemPtr) {
            dataPtr = itemPtr->data;
        } 
        if ((*cmpFunc)(dataPtr, data)) {
            Item<Object> *item = new Item<Object> (destroyed);
            item->data = data;
            if (itemPtr)  {
                if (prevPtr) {
                    prevPtr->next = item;
                    item->next = itemPtr;
                } else {
                    last->next = item;
                    item->next = first;
                    first = item;
                }
            } else {
                if (prevPtr) {
                    last = prevPtr->next = item;
                    item->next = first; 
                } else {
                    first = last = item;
                    last->next = item;
                }
            }
            return true;
        }
        if (itemPtr == 0) {
            break;
        }
        prevPtr = itemPtr;
        itemPtr = itemPtr->next;
        if (itemPtr == first) {
            itemPtr = 0;
        }
    }
    return false;
}


//
//Function: remove
//
//Member-Of: TQueue
//
//Parameters:
//      const void *data, 
//      bool (*cmpFunc)(const Object *, const void *)
//
//Return:
//      Object *
//
//Description:
//      The remove function calls the compare function for each
//      element in the queue to determine if the element is supposed
//      to be removed from the queue. 
//
//      The parameter data is the element appended, and cmpFunc is
//      an address of the compare function that is used to find 
//      the correct element in the queue 
//
//      The function returns the removed element. If nothing was
//      removed zero is returned. 
//
template<class Object> Object *TQueue<Object> :: 
    remove (const void *data, bool (*cmpFunc)(const Object *, const void *))
{
    register Item<Object> *itemPtr = first;
    register Item<Object> *prevPtr = 0;
    Object *removed = 0;

    while (itemPtr) {
        if ((*cmpFunc)(itemPtr->data, data)) {
            if (prevPtr) {
                if (itemPtr == last) {
                    prevPtr->next = first;
                    last = prevPtr;
                } else {
                    prevPtr->next = itemPtr->next;
                }
            } else {
                if (itemPtr == last)
		    last = first = 0;
		else
		    last->next = first = itemPtr->next;
            }
            removed = itemPtr->data;
            itemPtr->data = 0;
            delete itemPtr;
            break;
        }
        prevPtr = itemPtr;
        itemPtr = itemPtr->next;
	if (itemPtr == first) {
	    break;
	}
    }
    return removed;
}

//
//Function: SameObject
//
//Member-Of: TQueue
//
//Parameters:
//      const Object *a, 
//      const void *b
//
//Return:
//      bool
//
//Description:
//      Compares using addresses if the parameters are the same
//      object. This function is used when the user do not want
//      to use a specialized compare function of her own. 
//
//      The parameter a is a list element, and b is a tested object.
//
//      Returns non-zero value if the parameters are the same. 
//      Otherwise zero is returned.
//
template<class Object> 
bool TQueue<Object> :: SameObject(const Object *a, const void *b) 
{
    return (a == b) ? true : false;
}
//
//Function: remove
//
//Member-Of: TQueue
//
//Parameters:
//      Object *data
//
//Return:
//      Object *
//
//Description:
//      Removed the given object from the queue. The remove function
//      removes the first object that has the same address as the 
//      parameter. This is a simplified version of the general version
//      of the remove function.
//
//      The parameter data is the list element to be removed.
//
//      The function returns the removed element. If nothing was
//      removed zero is returned. 
//
template<class Object> 
Object *TQueue<Object> :: remove (Object *data)
{
    return remove (data, TQueue<Object> :: SameObject);
}

//
//Function: remove
//
//Member-Of: TQueue
//
//Return:
//      Object *
//
//Description:
//      Returns the first element in the queue. Zero is returned if
//      the queue is empty.
//
template<class Object> Object *TQueue<Object> :: remove (void)
{
    register Item<Object> *itemPtr = first;
    Object *object = 0;

    if (itemPtr) {
        object = itemPtr->data;
        itemPtr->data = 0;
        if (first == last) {
            first = last = 0;
        } else {
            first = last->next = itemPtr->next;
        }
        delete itemPtr;
    }
    return object;
}


//
//Constructor: TQueueListIterator
//
//Parameters:
//      const TQueue<Object> *hostQueue
//
//Description:
//      The constructor of the queue iterator initializes the 
//      internal variables. The hostQueue parameter must be given
//      for proper opeation.
//
//      The parameter hostQueue is the queue that you want to iterate.
//
template<class Object>
TQueueListIterator<Object> :: TQueueListIterator (const TQueue<Object> *hostQueue)
    : host (hostQueue), cursor (0)
{
}
//
//Function: operator=
//
//Member-Of: TQueueListIterator
//
//Parameters:
//      const TQueueListIterator<Object> &another
//
//Return:
//      TQueueListIterator<Object> &
//
//Description:
//      Copies the given iterator to itself.
//
//      The parameter another is another iterator that you want to
//      copy.
//
//      Returns the reference to itself.
//
template<class Object>
TQueueListIterator<Object> &TQueueListIterator<Object>
        :: operator = (const TQueueListIterator<Object> &another)
{
    cursor = another.cursor;
    host   = another.host;
    return *this;
}
//
//Function: reset
//
//Member-Of: TQueueListIterator
//
//Description:
//      Resets the iterator back to the first element of the queue.
//
template<class Object>
void TQueueListIterator<Object> :: reset (void)
{
    cursor = 0;
    return;
}


//
//Function: next
//
//Member-Of: TQueueListIterator
//
//Return:
//      Object *
//
//Description:
//      Finds the next element. After the last element the iterator
//      can be called again and it returns the first element, again.
//
//      Returns the next element in the queue. Zero is returned in the 
//      end of the queue, or if the queue was not given.
//

template<class Object>
Object *TQueueListIterator<Object> :: next (void)
{
    Object *object = 0;
    if (host) {
        if (cursor) {
            // the cursor is pointing somewhere
            if (cursor == host->first) {
                // we are at the end of queue
                cursor = 0;
                object = 0;
            } else {
                object = cursor->data;
                cursor = cursor->next;
            }           
        } else {
            // the cursor is pointing to the first element, virtually
            cursor = host->first;
            if (cursor) {
                // there is something in the queue
                object = cursor->data;
                cursor = cursor->next;
            } else {
                // the queue is empty
                object = 0;
            }
        }
    }
    return object;
}

//
//Function: head
//
//Member-Of: TQueueListIterator
//
//Return:
//      Object *
//
//Description:
//      Return pointer to the first data object. Returns zero if
//      the queue is empty. Leaves the data object into the queue.
//

template<class Object> Object *TQueueListIterator<Object> :: head (void) const
{
    if (host) 
    {
        if (host->first) 
        {
            return host->first->data;
        }
    }
    return 0;
}


//
//Function: last
//
//Member-Of: TQueueListIterator
//
//Return:
//      Object *
//
//Description:
//      Return pointer to the last data object. Returns zero if
//      the queue is empty. Leaves the data object into the queue.
//
template<class Object> Object *TQueueListIterator<Object> :: last (void) const
{
    if (host) 
    {
        if (host->last) 
        {
            return host->last->data;
        }
    }
    return 0;
}



#endif  // TQUEUE_H

