#ifndef YARP_MANAGER_HTTPSERVERDISPATCHELEMENT_H
#define YARP_MANAGER_HTTPSERVERDISPATCHELEMENT_H

#include <string>

namespace yarp {
    namespace manager {
        namespace server {
        struct HTTPServerDispatchElementKey;
        struct HTTPServerDispatchElementValue;
        }
    }
}

typedef enum {
    HTTPMethodNotDefined = 0,
    HTTPMethodGet = 1,
    HTTPMethodHead = 1 << 1,
    HTTPMethodPost = 1 << 2,
    HTTPMethodPut = 1 << 3,
    HTTPMethodDelete = 1 << 4,
    HTTPMethodAny =
    HTTPMethodGet
    | HTTPMethodHead
    | HTTPMethodPost
    | HTTPMethodPut
    | HTTPMethodDelete
} HTTPMethod;

extern std::string stringRepresentationForHTTPMethod(HTTPMethod method);
extern HTTPMethod httpMethodFromString(std::string method);


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
    int processRequest() const;
    int (*requestHandler)();
};


#endif /* end of include guard: YARP_MANAGER_HTTPSERVERDISPATCHELEMENT_H */
