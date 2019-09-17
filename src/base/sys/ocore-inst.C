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

$Log: ocore-inst.C,v $
Revision 1.2  2002/06/03 09:19:53  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:23  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:19  bilhanan
Full licence clause added into doors 0.1


*/

#include <doors/device.h>
#include <doors/classes.h>
#include <doors/otask.h>
#include <doors/timer.h>
#include <doors/port.h>
#include <doors/iface.h>

template class TQueue<Device>;
template class TQueue<Name>;
template class TQueue<Otask>;
template class TQueue<Timer>;
template class TQueue<Port>;
template class TQueue<Iface>;

template class TQueueListIterator<Device>;
template class TQueueListIterator<Name>;
template class TQueueListIterator<Otask>;
template class TQueueListIterator<Timer>;
template class TQueueListIterator<Port>;
template class TQueueListIterator<Iface>;

template class Item<Device>;
template class Item<Name>;
template class Item<Otask>;
template class Item<Timer>;
template class Item<Port>;
template class Item<Iface>;

