#ifndef YARP_MANAGER_MANAGERHTTPSERVER_H
#define YARP_MANAGER_MANAGERHTTPSERVER_H

#include <yarp/manager/server/HTTPServer.h>

namespace yarp {
    namespace manager {

        class Manager;

        namespace server {
            class ManagerHTTPServer;
        }
    }
}

//HTTP server for yarp manager
//Content-type: application/json
//It handles the following
//- (GET) Applications => return the list of applications (name)
//- (GET) Application/<app_name> => return the informations associated with the application <app_name>



class yarp::manager::server::ManagerHTTPServer : public yarp::manager::server::HTTPServer
{
private:
    const yarp::manager::Manager& m_manager;
    void * m_private;
public:
    ManagerHTTPServer(const yarp::manager::Manager& manager);

    bool initRequestHandlers();
};


#endif /* end of include guard: YARP_MANAGER_MANAGERHTTPSERVER_H */
