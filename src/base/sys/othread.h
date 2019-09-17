//
// $Id: othread.h,v 1.1.1.1 2002/04/30 12:32:52 bilhanan Exp $
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

$Log: othread.h,v $
Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:49  bilhanan
Full Licence clause into DOORS 0.2


*/

#ifndef OTHREAD_H
#define OTHREAD_H

/** OThread class is used for wrapping thread specific
  C function calls inside the class.
  Underlying Solaris implementation can also be changed
  into Posix implementation and minimize the changes needed
  outside of this class.<p>
  
  Only the used features of thread library are implemented. 
  That is why there are no scheduling functions done here.
  They can be easyly added by user later if needed.
  */
class OThread {
  public:
  /** Function wraps thr_create function. */
  static unsigned create (void *b, unsigned s, void *(*f)(void*), 
			  void *a, long l, unsigned *i);
  /** Function wraps thr_join function */
  static unsigned join (unsigned w, unsigned *d, void** s);
  /** Function wraps thr_setconcurrency function */
  static unsigned setConcurrency (unsigned i);
  /** Function wraps thr_getconcurrency function */
  static unsigned getConcurrency (void);
  /** Function wraps thr_keycreate function*/
  static unsigned keycreate(unsigned *k, void (*d)(void *v));
  /** Function wraps thr_setspecific function*/
  static unsigned setspecific(unsigned k, void *v);
  /** Function wraps thr_getspecific function*/
  static unsigned getspecific(unsigned k, void **v);
  /** Function wraps thr_self function*/
  static unsigned self (void);
  /** Function wraps thr_exit function*/
  static void exit (void *s);

 private:
  /** This class in not ment to be instantiated. All its member
    functions are static and can be called without creating
    a instance out of this class.
    */
  OThread (void);
  /** Just to be */
  ~OThread (void);
};

#endif
