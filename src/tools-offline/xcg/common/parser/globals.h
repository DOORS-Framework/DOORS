/*
 * globals.h: interface for all global variables of the library
 *
 * The bottom of this file is automatically generated by build_glob.py
 * based on the description file global.data
 *
 * See Copyright for the status of this software.
 *
 * Gary Pennington <Gary.Pennington@uk.sun.com>
 * daniel@veillard.com
 */

#ifndef __XML_GLOBALS_H
#define __XML_GLOBALS_H

#include <doors/parser.h>
#include <doors/xmlerror.h>
#include <doors/SAX.h>
#include <doors/xmlmemory.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Externally global symbols which need to be protected for backwards
 * compatibility support.
 */

#undef	docbDefaultSAXHandler
#undef	htmlDefaultSAXHandler
#undef	oldXMLWDcompatibility
#undef	xmlBufferAllocScheme
#undef	xmlDefaultBufferSize
#undef	xmlDefaultSAXHandler
#undef	xmlDefaultSAXLocator
#undef	xmlDoValidityCheckingDefaultValue
#undef	xmlFree
#undef	xmlGenericError
#undef	xmlGenericErrorContext
#undef	xmlGetWarningsDefaultValue
#undef	xmlIndentTreeOutput
#undef	xmlKeepBlanksDefaultValue
#undef	xmlLineNumbersDefaultValue
#undef	xmlLoadExtDtdDefaultValue
#undef	xmlMalloc
#undef	xmlMemStrdup
#undef	xmlParserDebugEntities
#undef	xmlParserVersion
#undef	xmlPedanticParserDefaultValue
#undef	xmlRealloc
#undef	xmlSaveNoEmptyTags
#undef	xmlSubstituteEntitiesDefaultValue

typedef struct _xmlGlobalState xmlGlobalState;
typedef xmlGlobalState *xmlGlobalStatePtr;
struct _xmlGlobalState 
{
	const char *xmlParserVersion;

	xmlSAXLocator xmlDefaultSAXLocator;
	xmlSAXHandler xmlDefaultSAXHandler;
	xmlSAXHandler docbDefaultSAXHandler;
	xmlSAXHandler htmlDefaultSAXHandler;

	xmlFreeFunc xmlFree;
	xmlMallocFunc xmlMalloc;
	xmlStrdupFunc xmlMemStrdup;
	xmlReallocFunc xmlRealloc;

	xmlGenericErrorFunc xmlGenericError;
	void *xmlGenericErrorContext;

	int oldXMLWDcompatibility;

	xmlBufferAllocationScheme xmlBufferAllocScheme;
	int xmlDefaultBufferSize;

	int xmlSubstituteEntitiesDefaultValue;
	int xmlDoValidityCheckingDefaultValue;
	int xmlGetWarningsDefaultValue;
	int xmlKeepBlanksDefaultValue;
	int xmlLineNumbersDefaultValue;
	int xmlLoadExtDtdDefaultValue;
	int xmlParserDebugEntities;
	int xmlPedanticParserDefaultValue;

	int xmlSaveNoEmptyTags;
	int xmlIndentTreeOutput;
};

#ifdef __cplusplus
}
#endif
#include <doors/threads.h>
#ifdef __cplusplus
extern "C" {
#endif

void	xmlInitializeGlobalState(xmlGlobalStatePtr gs);

/*
 * In general the memory allocation entry points are not kept
 * thread specific but this can be overridden by LIBXML_THREAD_ALLOC_ENABLED
 *    - xmlMalloc
 *    - xmlRealloc
 *    - xmlMemStrdup
 *    - xmlFree
 */

#ifdef LIBXML_THREAD_ALLOC_ENABLED
#ifdef LIBXML_THREAD_ENABLED
extern xmlMallocFunc *__xmlMalloc(void);
#define xmlMalloc \
(*(__xmlMalloc()))
#else
LIBXML_DLL_IMPORT extern xmlMallocFunc xmlMalloc;
#endif

#ifdef LIBXML_THREAD_ENABLED
extern xmlReallocFunc *__xmlRealloc(void);
#define xmlRealloc \
(*(__xmlRealloc()))
#else
LIBXML_DLL_IMPORT extern xmlReallocFunc xmlRealloc;
#endif

#ifdef LIBXML_THREAD_ENABLED
extern xmlFreeFunc *__xmlFree(void);
#define xmlFree \
(*(__xmlFree()))
#else
LIBXML_DLL_IMPORT extern xmlFreeFunc xmlFree;
#endif

#ifdef LIBXML_THREAD_ENABLED
extern xmlStrdupFunc *__xmlMemStrdup(void);
#define xmlMemStrdup \
(*(__xmlMemStrdup()))
#else
LIBXML_DLL_IMPORT extern xmlStrdupFunc xmlMemStrdup;
#endif
#else /* !LIBXML_THREAD_ALLOC_ENABLED */
LIBXML_DLL_IMPORT extern xmlMallocFunc xmlMalloc;
LIBXML_DLL_IMPORT extern xmlReallocFunc xmlRealloc;
LIBXML_DLL_IMPORT extern xmlFreeFunc xmlFree;
LIBXML_DLL_IMPORT extern xmlStrdupFunc xmlMemStrdup;
#endif /* LIBXML_THREAD_ALLOC_ENABLED */

#ifdef LIBXML_DOCB_ENABLED
extern xmlSAXHandler *__docbDefaultSAXHandler(void);
#ifdef LIBXML_THREAD_ENABLED
#define docbDefaultSAXHandler \
(*(__docbDefaultSAXHandler()))
#else
LIBXML_DLL_IMPORT extern xmlSAXHandler docbDefaultSAXHandler;
#endif
#endif

#ifdef LIBXML_HTML_ENABLED
extern xmlSAXHandler *__htmlDefaultSAXHandler(void);
#ifdef LIBXML_THREAD_ENABLED
#define htmlDefaultSAXHandler \
(*(__htmlDefaultSAXHandler()))
#else
LIBXML_DLL_IMPORT extern xmlSAXHandler htmlDefaultSAXHandler;
#endif
#endif


/*
 * Everything starting from the line below is
 * Automatically generated by build_glob.py.
 * Do not modify the previous line.
 */


extern int *__oldXMLWDcompatibility(void);
#ifdef LIBXML_THREAD_ENABLED
#define oldXMLWDcompatibility \
(*(__oldXMLWDcompatibility()))
#else
LIBXML_DLL_IMPORT extern int oldXMLWDcompatibility;
#endif

extern xmlBufferAllocationScheme *__xmlBufferAllocScheme(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlBufferAllocScheme \
(*(__xmlBufferAllocScheme()))
#else
LIBXML_DLL_IMPORT extern xmlBufferAllocationScheme xmlBufferAllocScheme;
#endif

extern int *__xmlDefaultBufferSize(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlDefaultBufferSize \
(*(__xmlDefaultBufferSize()))
#else
LIBXML_DLL_IMPORT extern int xmlDefaultBufferSize;
#endif

extern xmlSAXHandler *__xmlDefaultSAXHandler(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlDefaultSAXHandler \
(*(__xmlDefaultSAXHandler()))
#else
LIBXML_DLL_IMPORT extern xmlSAXHandler xmlDefaultSAXHandler;
#endif

extern xmlSAXLocator *__xmlDefaultSAXLocator(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlDefaultSAXLocator \
(*(__xmlDefaultSAXLocator()))
#else
LIBXML_DLL_IMPORT extern xmlSAXLocator xmlDefaultSAXLocator;
#endif

extern int *__xmlDoValidityCheckingDefaultValue(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlDoValidityCheckingDefaultValue \
(*(__xmlDoValidityCheckingDefaultValue()))
#else
LIBXML_DLL_IMPORT extern int xmlDoValidityCheckingDefaultValue;
#endif

extern xmlGenericErrorFunc *__xmlGenericError(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlGenericError \
(*(__xmlGenericError()))
#else
LIBXML_DLL_IMPORT extern xmlGenericErrorFunc xmlGenericError;
#endif

extern void * *__xmlGenericErrorContext(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlGenericErrorContext \
(*(__xmlGenericErrorContext()))
#else
LIBXML_DLL_IMPORT extern void * xmlGenericErrorContext;
#endif

extern int *__xmlGetWarningsDefaultValue(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlGetWarningsDefaultValue \
(*(__xmlGetWarningsDefaultValue()))
#else
LIBXML_DLL_IMPORT extern int xmlGetWarningsDefaultValue;
#endif

extern int *__xmlIndentTreeOutput(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlIndentTreeOutput \
(*(__xmlIndentTreeOutput()))
#else
LIBXML_DLL_IMPORT extern int xmlIndentTreeOutput;
#endif

extern int *__xmlKeepBlanksDefaultValue(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlKeepBlanksDefaultValue \
(*(__xmlKeepBlanksDefaultValue()))
#else
LIBXML_DLL_IMPORT extern int xmlKeepBlanksDefaultValue;
#endif

extern int *__xmlLineNumbersDefaultValue(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlLineNumbersDefaultValue \
(*(__xmlLineNumbersDefaultValue()))
#else
LIBXML_DLL_IMPORT extern int xmlLineNumbersDefaultValue;
#endif

extern int *__xmlLoadExtDtdDefaultValue(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlLoadExtDtdDefaultValue \
(*(__xmlLoadExtDtdDefaultValue()))
#else
LIBXML_DLL_IMPORT extern int xmlLoadExtDtdDefaultValue;
#endif

extern int *__xmlParserDebugEntities(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlParserDebugEntities \
(*(__xmlParserDebugEntities()))
#else
LIBXML_DLL_IMPORT extern int xmlParserDebugEntities;
#endif

extern const char * *__xmlParserVersion(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlParserVersion \
(*(__xmlParserVersion()))
#else
LIBXML_DLL_IMPORT extern const char * xmlParserVersion;
#endif

extern int *__xmlPedanticParserDefaultValue(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlPedanticParserDefaultValue \
(*(__xmlPedanticParserDefaultValue()))
#else
LIBXML_DLL_IMPORT extern int xmlPedanticParserDefaultValue;
#endif

extern int *__xmlSaveNoEmptyTags(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlSaveNoEmptyTags \
(*(__xmlSaveNoEmptyTags()))
#else
LIBXML_DLL_IMPORT extern int xmlSaveNoEmptyTags;
#endif

extern int *__xmlSubstituteEntitiesDefaultValue(void);
#ifdef LIBXML_THREAD_ENABLED
#define xmlSubstituteEntitiesDefaultValue \
(*(__xmlSubstituteEntitiesDefaultValue()))
#else
LIBXML_DLL_IMPORT extern int xmlSubstituteEntitiesDefaultValue;
#endif

#ifdef __cplusplus
}
#endif

#endif /* __XML_GLOBALS_H */
