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

$Log: othread.C,v $
Revision 1.2  2002/06/03 09:19:54  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:24  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:20  bilhanan
Full licence clause added into doors 0.1


*/

#include <doors/othread.h>
#include <thread.h>


//
//Constructor: OThread
//
//Description:
//      This class in not ment to be instantiated. All its member
//      functions are static and can be called without creating
//      a instance out of this class.
//      

OThread :: OThread (void) {

}

//
//Destructor: OThread
//
//Description:
//      Just to be
//      

OThread :: ~OThread (void) {

}

//
//Function: create
//
//Member-Of: Othread
//
//Parameters:
//     void *
//     unsigned
//     void *(*)(void *)
//     void *
//     long
//     unsigned *
//
//Return: unsigned
//
//Description:
//     Function wraps thr_create function
//
unsigned OThread :: create (void * b, unsigned s, void *(*f)(void*), 
			    void *a, long l, unsigned *i) {

  return thr_create (b, s, f, a, l, i);
} 

//
//Function: join
//
//Member-Of: Othread
//
//Parameters:
//     unsigned
//     unsigned *
//     void **
//
//Return: unsigned
//
//Description:
//     Function wraps thr_join function
//
unsigned OThread :: join (unsigned w, unsigned *d, void** s) {

  return thr_join (w, d, s);
}

//
//Function: setConcurrency
//
//Member-Of: Othread
//
//Parameters:
//     unsigned
//
//Return: unsigned
//
//Description:
//     Function wraps thr_setconcurrency function
//
unsigned OThread :: setConcurrency (unsigned i) {

  return thr_setconcurrency (i);
}

//
//Function: getConcurrency
//
//Member-Of: Othread
//
//Return: unsigned
//
//Description:
//     Function wraps thr_getconcurrency function
//
unsigned OThread :: getConcurrency (void) {

  return thr_getconcurrency ();
}

//
//Function: keycreate
//
//Member-Of: Othread
//
//Parameters:
//     unsigned*
//     void (*) (void *)
//
//Return: unsigned
//
//Description:
//     Function wraps thr_keycreate function
//
unsigned OThread :: keycreate(unsigned *k, void (*d)(void *v)) {

  return thr_keycreate(k, d);
}

//
//Function: setspecific
//
//Member-Of: Othread
//
//Parameters:
//     unsigned
//     void *
//
//Return: unsigned
//
//Description:
//     Function wraps thr_setspecific function
//
unsigned OThread :: setspecific(unsigned k, void *v) {

  return thr_setspecific(k, v);
}

//
//Function: getspecific
//
//Member-Of: Othread
//
//Parameters:
//     unsigned
//     void **
//
//Return: unsigned
//
//Description:
//     Function wraps thr_getspecific function
//
unsigned OThread :: getspecific(unsigned k, void **v) {

  return thr_getspecific(k, v);
}

//
//Function: self
//
//Member-Of: Othread
//
//Return: unsigned
//
//Description:
//     Function wraps thr_self function
//
unsigned OThread :: self(void) {

  return thr_self();
}

//
//Function: exit
//
//Member-Of: Othread
//
//Return: unsigned
//
//Description:
//     Function wraps thr_exit function
//
void OThread :: exit (void *s) {

  thr_exit (s);
}


