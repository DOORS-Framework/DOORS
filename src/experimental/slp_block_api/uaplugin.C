
#include <pthread.h>
#include <doors/debug.h>
#include "uaplugin.h"
#include "uapluginsm.h"

extern UAPluginSM plugin_sm;
UAPlugin *UAPlugin :: _instance = 0;

UAPlugin* UAPlugin :: Instance( ) {

    if( !_instance ) {
        _instance = new UAPlugin( "ua-plugin.singleton", Scheduler::Instance(),
                                  &plugin_sm );
    }

    return _instance;
}


UAPlugin :: UAPlugin( std::string name, Scheduler *s, UAPluginSM *sm )
    : PTask(name,s,sm),
      slp_sap(this),
      wakeup_msg( new Message ) {

    input_mutex = new pthread_mutex_t;
    pthread_mutex_init(input_mutex, 0);
    wakeup_msg->mark(&slp_sap,&slp_sap);
    wakeup_msg->lock();
    _DOORS_DEBUG(0, "UAPlugin constructed");

}

UAPlugin :: ~UAPlugin() {
    _instance = 0;
}

bool UAPlugin :: ready_srvind( Message *msg ) {

    _DOORS_DEBUG(0, "UAPlugin :: ready_srvind");

    UASAP :: Srv_ind *ind_msg = (UASAP :: Srv_ind*)msg;
    _DOORS_DEBUG(0, "UAPlugin got Ind message for handle: " << ind_msg->SLPHandle);

    std::list<doors_request*>::iterator i = in_process.begin();

    Uint32 *client_handle = 0;

    if( handle_map.find( ind_msg->SLPHandle ) != handle_map.end() ) {
        // Handle found
        std::cout << "Handle found!" << std::endl;
        client_handle = handle_map[ind_msg->SLPHandle];
    } else {
        std::cout << "Handle not found! handles in total: " << handle_map.size() << std::endl;
        client_handle = 0;
        i = in_process.end();
    }

    std::cout << "client_handle: " << client_handle << std::endl;
    std::cout << "Going through in_process list. Handles: ";
    for( ; i != in_process.end() && (*i)->handle != (void*)client_handle
             ; i++ )
    { std::cout << (*i)->handle; }

    std::cout << std::endl;

    if( i != in_process.end() ) {
        _DOORS_DEBUG(0, "Found correct handle! Waking thread up for result.");
        (*i)->result = (char*)(Byte*)ind_msg->urls;
        (*i)->err_code = ind_msg->errorcode;

    } else {
        (*i)->err_code = SLP_TYPE_ERROR;
    }

    pthread_mutex_lock( (*i)->cond_mutex );
    pthread_cond_signal( (*i)->wakeup_cond );
    pthread_mutex_unlock( (*i)->cond_mutex );

    return false;
}

bool UAPlugin :: ready_openind( Message *msg ) {

    _DOORS_DEBUG(0, "UAPlugin :: ready_openind");

    UASAP :: SLPOpen_ind *ind_msg = (UASAP :: SLPOpen_ind*)msg;
    _DOORS_DEBUG(0, "UAPlugin got SLPOpenInd message for handle: " << ind_msg->SLPHandle);

    std::list<doors_request*>::iterator i = in_process.begin();
    std::cout << "Going through in_process list. Handles: ";

    for( ; i != in_process.end() && (*i)->handle != 0
             ; i++ )
    { std::cout << (*i)->handle; }

    std::cout << std::endl;

    if( i != in_process.end() ) {
        _DOORS_DEBUG(0, "Found empty handle! Waking thread up.");
        (*i)->err_code = SLP_OK;
        Uint32 *new_handle = new Uint32;
        handles.push_back( new_handle );
        *new_handle = ind_msg->SLPHandle;
        (*i)->handle = (void*)new_handle;

        std::map<Uint32, Uint32*>::value_type map_val( ind_msg->SLPHandle, new_handle );
        handle_map.insert( map_val );

        pthread_mutex_lock( (*i)->cond_mutex );
        pthread_cond_signal( (*i)->wakeup_cond );
        pthread_mutex_unlock( (*i)->cond_mutex );
    }

    return false;
}

bool UAPlugin :: ready_default( Message *msg ) {
    _DOORS_DEBUG(0, "UAPlugin :: ready_default");

    if( msg->getType() == -1 ) {
        // This is just to wake us up and get
        // run-time from scheduler.
        // -> Check work queue and do some magic.
        _DOORS_DEBUG(0, "Wake-up signalled");

        _DOORS_ERROR(work_queue.size(), "Error! No work queued up!");

        while( work_queue.size() ) {
            // Work!
            // TODO: check slphandle!

            pthread_mutex_lock( input_mutex );
            doors_request *req = (*work_queue.begin());
            work_queue.pop_front();
            pthread_mutex_unlock( input_mutex );
            in_process.push_back( req );

            Message *down_msg = 0;
            UASAP :: Srv_req *find_req = 0;
            UASAP :: SLPOpen_req *open_req = 0;
            _DOORS_DEBUG(0, "req->request_type: " << req->request_type
                         << ", handle: " << req->handle << ", req->service_type: " << req->service_type);
            switch( req->request_type ) {
            case DOORS_REQ_FINDSRVS:
                _DOORS_DEBUG(0, "constructing UASAP::Srv_req " << req->handle);
                find_req = (UASAP :: Srv_req*)slp_sap.create(UASAP :: srv_req);
                find_req->SLPHandle = *(Uint32*)req->handle;
                find_req->sType = (std::string)req->service_type;
                down_msg = (Message*)find_req;
                break;
            case DOORS_REQ_SLPOPEN:
                _DOORS_DEBUG(0, "constructing UASAP::SLPOpen_req");
                open_req = (UASAP :: SLPOpen_req*)slp_sap.create(UASAP :: slpopen_req);
                open_req->langTag = (std::string)req->lang;
                down_msg = (Message*)open_req;
                break;
            default:
                break;
            }

            _DOORS_DEBUG(0, "msg: " << msg << ", msg->next: " << down_msg->next());
            _DOORS_DEBUG(0, "putMessaging request down");
            slp_sap.putMessage( down_msg );
        }
        

        
    }

    return true;
}
