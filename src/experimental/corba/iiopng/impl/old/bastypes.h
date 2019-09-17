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

$Log: bastypes.h,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:19  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.8  2001/06/08 08:40:10  bilhanan
Full licence clause added into doors 0.1


*/

 
#ifndef __basic_h__ 
#define __basic_h__ 
#include <memory>
#include <string> 

#include <assert.h> 
#include <iostream.h> 

//#include <doors/sockaddr.h> //?? 
#include <doors/datatypes.h>
#include <doors/iiopdefs.h>


namespace CORBA { 

  
  
   
  /***************** 
		    system exceptions 
  *****************/ 
#define GIOP_EXP_UNKNOWN                "UNKNOWN" 
#define GIOP_EXP_BAD_PARAM              "BAD_PARAM" 
#define GIOP_EXP_NO_MEMORY              "NO_MEMORY" 
#define GIOP_EXP_IMP_LIMIT              "IMP_LIMIT" 
#define GIOP_EXP_COMM_FAILURE           "COMM_FAILURE" 
#define GIOP_EXP_INV_OBJREF             "INV_OBJREF" 
#define GIOP_EXP_NO_PERMISSION          "NO_PERMISSION" 

#define GIOP_EXP_INTERNAL               "INTERNAL" 
#define GIOP_EXP_MARSHAL                "MARSHAL" 
#define GIOP_EXP_INITIALIZE             "INITIALIZE" 
#define GIOP_EXP_NO_IMPLEMENT           "NO_IMPLEMENT" 
#define GIOP_EXP_BAD_TYPECODE           "BAD_TYPECODE" 
#define GIOP_EXP_BAD_OPERATION          "BAD_OPERATION" 
#define GIOP_EXP_NO_RESOURCES           "NO_RESOURCES" 
#define GIOP_EXP_NO_RESPONSE            "NO_RESPONSE" 
#define GIOP_EXP_PERSIST_STORE          "PERSIST_STORE" 
#define GIOP_EXP_BAD_INV_ORDER          "BAD_INV_ORDER" 
#define GIOP_EXP_TRANSIENT              "TRANSIENT" 
#define GIOP_EXP_FREE_MEM               "FREE_MEM" 
#define GIOP_EXP_INV_IDENT              "INV_IDENT" 
#define GIOP_EXP_INV_FLAG               "INV_FLAG" 
#define GIOP_EXP_INTF_REPOS             "INTF_REPOS" 
#define GIOP_EXP_BAD_CONTEXT            "BAD_CONTEXT" 
#define GIOP_EXP_OBJ_ADAPTER            "OBJ_ADAPTER" 
#define GIOP_EXP_DATA_CONVERSION        "DATA_CONVERSION" 
#define GIOP_EXP_OBJECT_NOT_EXIST       "OBJECT_NOT_EXIT" 
#define GIOP_EXP_TRANSACTION_REQUIRED   "TRANSACTION_REQUIRED" 
#define GIOP_EXP_TRANSACTION_ROLLEDBACK "TRANSACTION_ROLLEDBACK" 
#define GIOP_EXP_INVALID_TRANSACTION    "INVALID_TRANSACTION" 
 
 
  typedef enum { 
    COMPLETED_YES,  
    COMPLETED_NO,  
    COMPLETED_MAYBE 
  } GIOP_CompletionStatus; 
   

  /* 
     template <class T, class traits = std::char_traits<T>,
     class Allocator = std::allocator<T> > class Basic_String 
     : public std::basic_string< T, traits, Allocator > {
     public:
     
       explicit Basic_String(const Allocator& = Allocator());
       basic_string (const basic_string<charT, traits,
       Allocator>&);
       basic_string(const basic_string&, size_type, 
       size_type = npos, 
       const Allocator& a = Allocator());
       basic_string(const charT*, size_type,
       const Allocator& = Allocator());
       basic_string(const charT*, const Allocator& = Allocator());
       basic_string(size_type, charT,
       const Allocator& = Allocator());
       template <class InputIterator>
       basic_string(InputIterator, InputIterator,
       const Allocator& = Allocator());
       
       std::allocator<T> *all;
       
       explicit Basic_String() : 
       all(new std::allocator<T>()),
       std::basic_string<T, traits, Allocator>(*all){};
       
       Basic_String( T* ptr) : std::basic_string<T>( ptr , std::allocator<T>()) {};
       
       CORBA::ULong Length() const { 
       return length(); 
       }; 
       
       void Length(CORBA::ULong newLen){
       resize( newLen);
       }; 

       bool operator==(const char *rhs) const {
       using namespace std;
       return (basic_string<T>)*this == rhs;
       };
       
       
       // bool operator==(const Basic_String< T> &rhs) const {
       //  using namespace std;
       // return (basic_string<T>)*this == (basic_string<T>)rhs;
       //};
       
       };
       
       typedef Basic_String<char> String;
  */
  /*
    class String : public std::string {
    public:
    
    //   String() : std::string(){};
    
    
    template <class charT, class traits , class Allocator  >
    inline basic_string<charT, traits, Allocator>
    ::basic_string (const Allocator& alloc) : __data_((charT*)0,alloc)
    {
    __data_ = __getNullRep()->data();
    __getNullRep()->__addReference();
    }
    
     String(char *ptr) : std::string(ptr) {};
     
     CORBA::ULong Length() const { 
     return length(); 
     }; 
     
     void Length(CORBA::ULong newLen){
     resize( newLen);
     }; 
     
     //bool operator==(const String &rhs) const {
     // return (std::string)*this == (std::string)rhs;
     //};
     
  };
  */
  
  class iiopString 
    { 
      CORBA::ULong maxLen; 
      CORBA::ULong len;                    /*the actual length of the string*/ 
      char* data;                         /*data member to hold objects*/ 
      
    public: 
      static char* AllocBuf(CORBA::ULong n) { 
      	return new char[n]; 
      } 
      static void FreeBuf(char* p) { delete [] p; } 
      //  const char*   ptr; 
 
      iiopString(); 
 
      class Const_Char { 
      	private: 
				const char*   ptr; 
      	public: 
				Const_Char (const char* d) : ptr(d) {}; 
				const char* Ptr () const { return ptr; } 
      }; 
 
      /* It should be used as: CORBA::iiopString(const char* c); 
	 It seems 'explicit' is not supported. 
	 Otherwise, it should be able to use directly.*/ 
    	iiopString(const Const_Char& c); 
 
      iiopString (const iiopString& rhs)//  copy constructor 
	: maxLen(0), len(0) 
	{ *this = rhs; }; 
 
      ~iiopString() { FreeBuf(data); }  /* destructor*/ 
 
      /*  assign operator*/ 
      iiopString& operator=(const iiopString& rhs); 
 
      bool operator==(const iiopString& rhs) const; 
 
      bool operator ==(const char* rhs) const 
	{ 
	  return *this == iiopString(rhs); 
	}; 
 
      bool operator < (const iiopString& rhs) const 
	{ 
	  if (this == &rhs) 
	    return false; 
 
	    CORBA::ULong l = (len < rhs.len)? len : rhs.len; 
	    if (l) 
	      return (strncmp(data,rhs.data,len) < 0); 
	    else 
	      return len < rhs.len; 
	  }; 
 
	CORBA::ULong Length() const          //  get the length of the sequence 
	  { return len; }; 
 
	/* ?????????????????????????????????????????????????????????????????????? 
	   after expansion, the length was filled with zeros, will that be a bug? 
	*/ 
	void Length(CORBA::ULong newLen); 
	 
	char& operator[](CORBA::ULong idx)              //  array operator 
	  { assert(idx < len); return data[idx]; }; 
	const char& operator[](CORBA::ULong idx) const  // the const version 
	  { assert(idx < len); return data[idx]; }; 
	 
	 
	char*  Data()                       //  get the data member 
	  { return data; }       
	const char* Data() const            //  the const version 
	  { return data; }; 
	 
	const char* In() const { return data; } 
	char*& Inout() { return data; } 
      }; 
             

  
  /*************************************************************** 
  Template for sequence type 
 
  Basic implementation, insert and remove algorithm could be 
  optimized. 
 
  **************************************************************/ 
  
  template <class T> 
    class Sequence { 
    CORBA::ULong maxLen;      // the maximum length of the sequence
    CORBA::ULong len;         // the actual length of the sequence
    T* data;                  // data member to hold objects
    
    public: 
    T* AllocBuf(CORBA::ULong n) 
      { 
	T* ptr = new T[n]; 
	if (!ptr) 
	  throw "memory allocation failed, wrong byte order?"; 
	return ptr; 
      } 
    void FreeBuf(T* p) { delete [] p; } 
    
    Sequence()                          //  default constructor 
      : maxLen(0), len(0), data(0) {}; 
    
    Sequence(CORBA::ULong m)            //  m is the max length 
      : maxLen(m), len(0) 
      { 
	if (maxLen > 0) 
	  data = AllocBuf(maxLen); 
	else 
	  data = 0; 
      }; 
    
    // constructor with an initial data vector, it is user's responsibility 
    // to keep the maxmium size m, the length of data l and the vector pointer 
    // d matching each other. 
    Sequence(CORBA::ULong m, CORBA::ULong l, T* d) 
      : maxLen(m), len(l) 
      { 
	assert(maxLen >= len); 
	
	if (maxLen > 0) 
	  data = AllocBuf(maxLen); 
	else 
	  data = 0; 
	
	for (CORBA::ULong i = 0; i < len; i++) { 
	  data[i] = d[i]; 
	} 
      }; 
      
    Sequence(const Sequence<T>& rhs)    //  copy constructor 
      : maxLen(0), len(0), data(0) 
      { *this = rhs; }; 
    
    ~Sequence() { FreeBuf(data); }      // destructor 
  
    //  assign operator 
    Sequence<T>& operator=(const Sequence<T>& rhs) 
      { 
      if (this != &rhs) 
	FreeBuf(data); 
 
      maxLen = rhs.maxLen; 
      len = rhs.len; 
 
      if (maxLen > 0) 
	data = AllocBuf(maxLen); 
      else 
	data = 0; 
 
      for (CORBA::ULong i = 0; i < len; i++) { 
	data[i] = rhs.data[i]; 
      } 
 
      return *this; 
    }; 
 
  bool operator==(const Sequence<T>& rhs) const 
    { 
      if (this == &rhs) 
	return true; 
 
      if (len == rhs.len) { 
	if (!len) 
	  return true; 
	for (int i = 0; i < len; i++) { 
	  //if (!(data[i] == rhs.data[i])) 
     if (!(!memcmp(&data[i],&rhs.data[i],sizeof(data[i])))) 
	    return false; 
	} 
	return true; 
      } 
      else 
	return false; 
    } 
 
  bool operator<(const Sequence<T>& rhs) const 
    { 
      if (this == &rhs) 
			return false; 
 
      CORBA::ULong l = (len < rhs.len) ? len : rhs.len; 
      if (!l) 
			return len < rhs.len; 
 
      for (int i = 0; i < len; i++) { 
			if (data[i] < rhs.data[i]) 
	  			return true; 
			else if (!(data[i] == rhs.data[i])) 
	  			return false; 
      } 
      return false; 
    } 
    
  CORBA::ULong Maximum() const         //  get the maxium length 
    { return maxLen; } 
 
  void Length(CORBA::ULong newLen)     //  change the length of the sequence 
    { 
      if (newLen < len) { 
            for (CORBA::ULong i = newLen; i < len; i++) { 
	      data[i] = T(); 
            } 
      } 
      else if (newLen > maxLen) { 
	T* old = data; 
	maxLen = newLen > maxLen + 16 ? newLen : maxLen + 16; 
	data = AllocBuf(maxLen); 
 
	for (CORBA::ULong i = 0; i < len; i++) 
	  data[i] = old[i]; 
 
	FreeBuf(old); 
      } 
      len = newLen; 
    }; 
 
  CORBA::ULong Length() const          //  get the length of the sequence 
    { return len; }; 
 
  T&  operator[](CORBA::ULong idx)     //  array operator 
    { assert(idx < len); return data[idx]; }; 
  const T& operator[](CORBA::ULong idx) const  // the const version 
    { assert(idx < len); return data[idx]; }; 
 
 
  T*  Data()  { return data; }        //  get the data member
  const T* Data() const               //  the const version
    { return data; }; 
 
  void Insert(const T& rhs)           //  insert an object at the head 
    { 
      if (len < maxLen) { 
	for (CORBA::ULong i = len; i > 0; i--) 
	  data[i] = data[i-1]; 
      } 
      else { 
	T* old = data; 
	 
	maxLen += 16; 
	data = AllocBuf(maxLen); 
	for (CORBA::ULong i = 0; i < len; i++) 
	  data[i+1] = old[i]; 
      } 
       
      len++; 
      data[0] = rhs; 
    }; 
   
  void Append(const T& rhs)            //  append an object to the tail 
    { 
      if (len >= maxLen) { 
	T* old = data; 
	 
	maxLen += 16; 
	data = AllocBuf(maxLen); 
	for (CORBA::ULong i = 0; i < len; i++) 
	  data[i] = old[i]; 
      } 
      data[len] = rhs; 
      len++; 
    }; 
 
  void Remove(CORBA::ULong idx)         //  remove an object at random location 
    { 
      assert(idx < len); 
 
      for (CORBA::ULong i = idx; i < len; i++) 
	data[i] = data[i+1]; 
       
      data[len] = T(); 
    }; 
}; 
 
   
  
    template <class T> inline 
    ostream& operator << (ostream& lhs, const Sequence <T> & rhs) 
    { 
    CORBA::ULong l = rhs.Length();                 
    lhs << "Length[" << dec << l << "] "; 
    for (CORBA::ULong i = 0; i < l; i++) { 
    if (sizeof(T) == sizeof(CORBA::Octet)) { 
    lhs  << hex << rhs[i] << " "; 
    } 
    else { 
    lhs << rhs[i] << " "; 
    } 
    } 
    
    return lhs; 
    } 
    
  

}; // namespace CORBA
 
/****************************** 
  // CDR for basic sequence types 
  *******************************/ 
 

 
 
 
/********************** 
  // CDR for string types 
  **********************/ 

/* 
inline 
ostream& operator << (ostream& lhs, const CORBA::iiopString& rhs) 
{ 
  CORBA::ULong l = rhs.Length();                 
  lhs << "Length[" << l << "] "; 
  lhs << rhs.Data(); 
   
  return lhs; 
}; 
 
*/
 
 
#endif 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
