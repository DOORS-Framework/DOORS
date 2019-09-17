
#ifndef _uaplugin_h_
#define _uaplugin_h_

#include <doors/ptask.h>
#include <doors/uasap.h>
#include "minislp.h"
#include <list>
#include <pthread.h>

#define DOORS_REQ_SLPOPEN 1
#define DOORS_REQ_FINDSRVS 2

class UAPluginSM;

class UAPlugin : public PTask {

public:

    static UAPlugin *Instance();
    ~UAPlugin();

    bool ready_openind( Message *msg );
    bool ready_srvind( Message *msg);
    bool ready_default( Message *msg );

    typedef struct plugin_request_ {
        pthread_cond_t *wakeup_cond;
        pthread_mutex_t *cond_mutex;
        pthread_t api_thread_id;
        int request_type;
        const char *lang;
        const char *scopeList;
        const char *service_type;
        unsigned int lifetime;
        const char *search_filter;
        const char *attr_ids;
        const char *naming_auth;
        char *result;
        SLPError err_code;
        SLPHandle handle;
    } doors_request;

    // These two control API thread -- DOORS system interface
    pthread_mutex_t *input_mutex;
    std::list<doors_request*> work_queue;

    Message *wakeup_msg;

    UASAP :: User slp_sap;

private:

    UAPlugin( std::string name, Scheduler *s, UAPluginSM *sm );
    std::list<doors_request*> in_process;
    std::list<Uint32*> handles;
    std::map<Uint32, Uint32*> handle_map;

    static UAPlugin *_instance;

};

#endif
