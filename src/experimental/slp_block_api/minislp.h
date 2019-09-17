
//  Copyright 2004
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.

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

$Log: minislp.h,v $
Revision 1.1  2004/08/03 05:31:07  hartmanj
Initial revision


*/


#ifndef _minislp_h_
#define _minislp_h_

#if(defined __cplusplus)
extern "C"
{
#endif
  

typedef int SLPError;

#define SLP_LAST_CALL 1

// A small subset of SLP error codes

#define SLP_OK 0
#define SLP_LANGUAGE_NOT_SUPPORTED -1
#define SLP_PARSE_ERROR -2
#define SLP_SCOPE_NOT_SUPPORTED -4
#define SLP_NOT_IMPLEMENTED -17
#define SLP_HANDLE_IN_USE -25
#define SLP_TYPE_ERROR -26


typedef enum {
  SLP_FALSE = 0,
  SLP_TRUE = 1
} SLPBoolean;


// From OpenSLP's slp.h:
typedef struct srvurl
{
    char *s_pcSrvType;
    /* A pointer to a character string containing the service              */
    /* type name, including naming authority.  The service type            */
    /* name includes the "service:" if the URL is of the service:          */
    /* scheme.                                                             */

    char *s_pcHost;
    /* A pointer to a character string containing the host                 */
    /* identification information.                                         */

    int   s_iPort;
    /* The port number, or zero if none.  The port is only available       */
    /* if the transport is IP.                                             */

    char *s_pcNetFamily;
    /* A pointer to a character string containing the network address      */
    /* family identifier.  Possible values are "ipx" for the IPX           */
    /* family, "at" for the Appletalk family, and "" (i.e.  the empty      */
    /* string) for the IP address family.                                  */

    char *s_pcSrvPart;
    /* The remainder of the URL, after the host identification.            */

} SLPSrvURL;


typedef void* SLPHandle;


// Only functions implementing UA are included for now

typedef SLPBoolean SLPSrvTypeCallback(SLPHandle hSLP,
                                      const char* pcSrvTypes,
                                      SLPError errCode,
                                      void *pvCookie);


typedef SLPBoolean SLPSrvURLCallback(SLPHandle hSLP,
                                     const char* pcSrvURL,
                                     unsigned short sLifetime,
                                     SLPError errCode,
                                     void *pvCookie);


typedef SLPBoolean SLPAttrCallback(SLPHandle hSLP,
                                   const char* pcAttrList,
                                   SLPError errCode,
                                   void *pvCookie);

SLPError SLPOpen(const char *pcLang, SLPBoolean isAsync, SLPHandle *phSLP);

void SLPClose(SLPHandle hSLP);

SLPError SLPFindSrvTypes(SLPHandle    hSLP,
                         const char  *pcNamingAuthority,
                         const char  *pcScopeList,
                         SLPSrvTypeCallback callback,
                         void *pvCookie);  

SLPError SLPFindSrvs(SLPHandle  hSLP,
                     const char *pcServiceType,
                     const char *pcScopeList,
                     const char *pcSearchFilter,
                     SLPSrvURLCallback callback,
                     void *pvCookie);


SLPError SLPFindAttrs(SLPHandle   hSLP,
                      const char *pcURLOrServiceType,
                      const char *pcScopeList,
                      const char *pcAttrIds,
                      SLPAttrCallback callback,
                      void *pvCookie);


SLPError SLPParseSrvURL(const char *pcSrvURL,
                        SLPSrvURL** ppSrvURL);



#if(defined __cplusplus)
}
#endif

#endif // _minislp_h_
