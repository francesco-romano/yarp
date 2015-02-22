#ifndef YARP_MANAGER_HTTPSERVER_H
#define YARP_MANAGER_HTTPSERVER_H

#include <string>

namespace yarp {
    namespace manager {
        namespace server {
            class HTTPServer;
            class HTTPResponse;
            class HTTPServerSerializable;

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

        }
    }
}

class yarp::manager::server::HTTPServer {
public:
    typedef yarp::manager::server::HTTPResponse (*RequestHandler)(void * context);

private:
    HTTPServer(const HTTPServer&);
    HTTPServer& operator=(const HTTPServer&);

    bool m_running;
    void * m_implementation;
public:
    HTTPServer();
    ~HTTPServer();
    
    bool startServer();
    bool stopServer();
    bool isRunning();
    
    bool addRequestHandle(HTTPMethod method, std::string relativeURL, RequestHandler handler, void* context);
    bool removeRequestHandle(HTTPMethod method, std::string relativeURL);
    
};

class yarp::manager::server::HTTPResponse {
public:
    HTTPResponse();

    int returnCode;
    HTTPServerSerializable *content;
};


#endif /* end of include guard: YARP_MANAGER_HTTPSERVER_H */
