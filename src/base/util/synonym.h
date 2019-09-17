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

$Log: synonym.h,v $
Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:05  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:39  bilhanan
Full licence clause added into doors 0.1


*/



// DO NOT USE THIS

#ifndef _synonym_h
#define _synonym_h

/*
 *Immune: Byte
 *Description:
 *      Unsigned integer type for other purposes (holding strings,
 *      octets, etc) than calculations. 
 */


/*
 *Immune: Word
 *Description:
 *      Unsigned integer type for other purposes (holding strings,
 *      octets, etc) than calculations. 
 */


/*
 *Immune: Dword
 *Description:
 *      Unsigned integer type for other purposes (holding strings,
 *      octets, etc) than calculations. 
 */



/*
 *Immune: Uint8
 *Description:
 *      Unsigned integer type for calculations.
 */

/*
 *Immune: Uint16
 *Description:
 *      Unsigned integer type for calculations.
 */


/*
 *Immune: Uint32
 *Description:
 *      Unsigned integer type for calculations.
 */


/*
 *Immune: Sint8
 *Description:
 *      Signed integer type for calculations.
 */

/*
 *Immune: Sint16
 *Description:
 *      Signed integer type for calculations.
 */

/*
 *Immune: Sint32
 *Description:
 *      Signed integer type for calculations.
 */


/*
 *Immune: bool
 *Description:
 * ANSI C++ bool definition in case compiler has no one.
 *Commentary:
 * has to be defined to *int* instead enum, because
 * compiler results operations, like (a == b), as int.
 */ 

/*
 *Immune: Boolean
 *Description:
 *      Boolean type for truth values.
 */

/*
 *Immune: Load
 *Description:
 *      Counter type for holding the number of messages the task has.
 */

#endif /* _synonym_h */


