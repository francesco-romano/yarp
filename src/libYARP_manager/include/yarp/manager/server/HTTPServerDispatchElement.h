#ifndef YARP_MANAGER_HTTPSERVERDISPATCHELEMENT_H
#define YARP_MANAGER_HTTPSERVERDISPATCHELEMENT_H

#include <string>
#include <yarp/manager/server/HTTPServer.h>

namespace yarp {
    namespace manager {
        namespace server {

        struct HTTPServerDispatchElementKey;
        struct HTTPServerDispatchElementValue;
        }
    }
}

struct yarp::manager::server::HTTPServerDispatchElementKey
{
    HTTPServerDispatchElementKey(HTTPMethod method, std::string relativeURL);

    struct Compare {
        bool operator()(const HTTPServerDispatchElementKey& left, const HTTPServerDispatchElementKey& right) const;
    };

    HTTPMethod method;
    std::string url;
};


struct yarp::manager::server::HTTPServerDispatchElementValue {
    HTTPResponse processRequest() const;
    HTTPServer::RequestHandler requestHandler;
    void * context;
};


#endif /* end of include guard: YARP_MANAGER_HTTPSERVERDISPATCHELEMENT_H */
