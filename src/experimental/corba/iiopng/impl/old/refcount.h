//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
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

$Log: refcount.h,v $
Revision 1.1.1.1  2002/05/13 14:38:37  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:25  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.2  2001/06/08 08:40:11  bilhanan
Full licence clause added into doors 0.1


*/
// refcount.h
// reference counting head file
// Yingfan Liu
// 21/04/98

#if !defined(__refcount_h)
#define __refcount_h

/////////////////////////////////////////////////////////////////////////////
//
//  Base class for reference-counted objects
//
/////////////////////////////////////////////////////////////////////////////
class TRCObject {
public:
    void    AddRef();
    void    DelRef();

    void    UnShare();
    bool    IsShareable() const;
    bool    IsShared() const;

protected:
    TRCObject();
    TRCObject(const TRCObject& rhs);
    TRCObject& operator = (const TRCObject& rhs);
    virtual ~TRCObject();

private:
    int     refs_;
    bool    shareable_;
};

inline  TRCObject::TRCObject() : refs_(0), shareable_(true)
                                        {};

inline  TRCObject::TRCObject(const TRCObject&) : refs_(0), shareable_(true)
                                        {};

inline  TRCObject::~TRCObject()         {};

inline  TRCObject&  TRCObject::operator = (const TRCObject&)
                                        {   return *this; };

inline  void        TRCObject::AddRef() {   ++refs_; };

inline  void        TRCObject::DelRef() {   if (--refs_ == 0) delete this; };

inline  void        TRCObject::UnShare(){   shareable_ = false; };

inline  bool        TRCObject::IsShareable() const
                                        {   return shareable_; };

inline  bool        TRCObject::IsShared() const
                                        {   return refs_ > 1; };


/////////////////////////////////////////////////////////////////////////////
//
//  Template class for smart pointers-to-T objects; 
//  T must inherit from RCObject
//
/////////////////////////////////////////////////////////////////////////////
template <class T>
class TRCPtr {
public:                                       
    TRCPtr (T* ptr = 0);                // allow implicit conversion
    TRCPtr (const TRCPtr& rhs);
    ~TRCPtr();

    TRCPtr& operator = (const TRCPtr& rhs);

    T*  operator->() const;
    T&  operator*() const;
    operator void* () const;

private:
    T*  pointee_;

    void Init();
};

template <class T> inline   void        TRCPtr<T>::Init()
                                        {
                                            if (pointee_ == 0) return;

                                            if (pointee_->IsShareable() == false) {
                                                pointee_ = new T(*pointee_);
                                            }
                                            pointee_->AddRef();
                                        };

template <class T> inline               TRCPtr<T>::TRCPtr(T* ptr) : pointee_(ptr)
                                        {   Init(); };

template <class T> inline               TRCPtr<T>::TRCPtr(const TRCPtr& rhs) 
                                              : pointee_(rhs.pointee_)
                                        {   Init(); };

template <class T> inline               TRCPtr<T>::~TRCPtr()
                                        {   if (pointee_) 
                                                pointee_->DelRef(); 
                                        };

template <class T> inline   TRCPtr<T>&  TRCPtr<T>::operator = (const TRCPtr& rhs)
                                        {
                                            if (pointee_ != rhs.pointee_) {
                                                if (pointee_)
                                                    pointee_->DelRef();
                                                pointee_ = rhs.pointee_;
                                                Init();
                                            }

                                            return *this;
                                        };


template <class T> inline               TRCPtr<T>::operator void* () const
                                        {   return pointee_; };

template <class T> inline   T*          TRCPtr<T>::operator -> () const
                                        {   return pointee_; };

template <class T> inline   T&          TRCPtr<T>::operator * () const 
                                        {   return *pointee_; };

#endif                               







