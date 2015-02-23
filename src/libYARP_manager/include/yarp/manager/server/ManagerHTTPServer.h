#ifndef YARP_HTTPD_MANAGERHTTPSERVER_H
#define YARP_HTTPD_MANAGERHTTPSERVER_H

#include <yarp/manager/server/HTTPServer.h>
#include <string>

namespace yarp {
    namespace manager {

        class Manager;

        namespace server {
            class ManagerHTTPServer;


            //HTTP server for yarp manager
            //Content-type: application/json
            //It handles the following
            //- (GET) Applications => return the list of applications (name)
            //- (GET) Application/<app_name> => return the informations associated with the application <app_name>

            extern std::string ManagerHTTPServerGetApplications;
            extern std::string ManagerHTTPServerGetApplicationNamed;

        }
    }
}

class yarp::manager::server::ManagerHTTPServer : public yarp::manager::server::HTTPServer
{
private:
    const yarp::manager::Manager& m_manager;
    void * m_private;
public:
    ManagerHTTPServer(const yarp::manager::Manager& manager);
    virtual ~ManagerHTTPServer();

    bool initRequestHandlers();
};


#endif /* end of include guard: YARP_HTTPD_MANAGERHTTPSERVER_H */
