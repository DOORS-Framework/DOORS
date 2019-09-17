
#include <doors/ptb.h>
#include "minislp.h"
#include <cstring> // strlen
#include "uaplugin.h"

pthread_t doors_tid = 0;
extern UAPlugin *ua_plugin;
extern void doors_main(void);


void *api_block_func( void *arg ) {

    std::cout << "api_block_func started" << std::endl;

    if( doors_tid == 0 ) {
        doors_main( );
    }

    UAPlugin :: doors_request *slp_req = (UAPlugin :: doors_request*)arg;

    pthread_mutex_lock( ua_plugin->input_mutex );
    ua_plugin->work_queue.push_back( slp_req );
    ua_plugin->slp_sap.getMessage( ua_plugin->wakeup_msg );

    pthread_mutex_unlock( ua_plugin->input_mutex );

    int res = pthread_kill( doors_tid, SIGUSR1 );

    if( res ) {
        std::cerr << "Error sending wake-up signal to DOORS thread!" << std::endl;
    }
    std::cout << "api_block_func going to sleep till wake-up time" << std::endl;
    slp_req->wakeup_cond = new pthread_cond_t;
    slp_req->cond_mutex = new pthread_mutex_t;
    res = pthread_cond_init( slp_req->wakeup_cond, 0 );

    if( res ) {
        _DOORS_DEBUG(0, "Error: pthread_cond_init( slp_req->wakeup_cond, 0 ) returned " << res);
    }

    res = pthread_mutex_init( slp_req->cond_mutex, 0 );

    if( res ) {
        _DOORS_DEBUG(0, "Error: pthread_cond_init( slp_req->wakeup_cond, 0 ) returned " << res);
    }

    pthread_mutex_lock( slp_req->cond_mutex );
    std::cout << "pthread_cond_wait returned " << pthread_cond_wait( slp_req->wakeup_cond, slp_req->cond_mutex ) << std::endl;
    std::cout << "EINVAL: " << EINVAL << std::endl;
    pthread_mutex_unlock( slp_req->cond_mutex );
    std::cout << "api_block_func woke up" << std::endl;
    std::cout << "api_block_func ended" << std::endl;
}


SLPError SLPOpen(const char *pcLang, SLPBoolean isAsync, SLPHandle *phSLP) {

    std::cout << "SLPOpen called. Operation is " << (isAsync ? "async " : "sync") << std::endl;

    UAPlugin :: doors_request *slp_req = new UAPlugin :: doors_request;

    slp_req->request_type = DOORS_REQ_SLPOPEN;
    slp_req->lang = new char[ strlen(pcLang) ];
    slp_req->scopeList = 0;
    slp_req->service_type = 0;
    slp_req->search_filter = 0;
    slp_req->attr_ids = 0;
    slp_req->naming_auth = 0;
    slp_req->result = 0;

    int res = pthread_create( &slp_req->api_thread_id, 0, api_block_func, (void*)slp_req );
    if( res ) {
        _DOORS_DEBUG(0, "Error: pthread_cond_init( slp_req->wakeup_cond, 0 ) returned " << res);
    }

    res = pthread_join( slp_req->api_thread_id, 0 );

    if( res ) {
        _DOORS_DEBUG(0,"Error: pthread_cond_init( slp_req->wakeup_cond, 0 ) returned " << res);
    }

    *phSLP = slp_req->handle;


    delete slp_req->wakeup_cond;
    delete slp_req->cond_mutex;
    delete [] slp_req->lang;
    SLPError ret = slp_req->err_code;
    delete slp_req;

    return ret;
}


void SLPClose(SLPHandle hSLP) {

}


SLPError SLPFindSrvTypes(SLPHandle hSLP,
                         const char  *pcNamingAuthority,
                         const char  *pcScopeList,
                         SLPSrvTypeCallback callback,
                         void *pvCookie) {

}


SLPError SLPFindSrvs(SLPHandle  hSLP,
                     const char *pcServiceType,
                     const char *pcScopeList,
                     const char *pcSearchFilter,
                     SLPSrvURLCallback callback,
                     void *pvCookie) {
    std::cout << "SLPFindSrvs called with " << pcServiceType << std::endl;
    UAPlugin :: doors_request *slp_req = new UAPlugin :: doors_request;
    std::string scope_list = std::string( pcScopeList );
    std::string servicetype = std::string( pcServiceType );
    std::string searchfilter = std::string( pcSearchFilter );

    slp_req->request_type = DOORS_REQ_FINDSRVS;
    slp_req->handle = hSLP;
    std::cout << "slp_req->handle = " << slp_req->handle << std::endl;
    slp_req->scopeList = scope_list.c_str();
    slp_req->service_type = servicetype.c_str();
    slp_req->search_filter = searchfilter.c_str();
    slp_req->attr_ids = 0;
    slp_req->naming_auth = 0;
    slp_req->result = 0;

    int res = pthread_create( &slp_req->api_thread_id, 0, api_block_func, (void*)slp_req );

    if( res ) {
        _DOORS_DEBUG(0,"Error: pthread_cond_init( slp_req->wakeup_cond, 0 ) returned " << res);
    }

    res = pthread_join( slp_req->api_thread_id, 0 );

    if( res ) {
        _DOORS_DEBUG(0,"Error: pthread_cond_init( slp_req->wakeup_cond, 0 ) returned " << res);
    }

    // Call back client's function for results
    (callback)(hSLP, slp_req->result, slp_req->lifetime, slp_req->err_code, pvCookie);

    delete slp_req->wakeup_cond;
    delete slp_req->cond_mutex;
    SLPError ret = slp_req->err_code;
    delete slp_req;

    return ret;
    

}


SLPError SLPFindAttrs(SLPHandle   hSLP,
                      const char *pcURLOrServiceType,
                      const char *pcScopeList,
                      const char *pcAttrIds,
                      SLPAttrCallback callback,
                      void *pvCookie) {

}


SLPError SLPParseSrvURL(const char *pcSrvURL,
                        SLPSrvURL** ppSrvURL) {



}
