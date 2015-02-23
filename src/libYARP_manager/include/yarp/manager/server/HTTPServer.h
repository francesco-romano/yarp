#ifndef YARP_HTTPD_HTTPSERVER_H
#define YARP_HTTPD_HTTPSERVER_H

#include <string>
#include <map>
#include <yarp/manager/server/Constants.h>

namespace yarp {
    namespace manager {
        namespace server {
            class HTTPServer;
            class HTTPResponse;
            class HTTPServerSerializable;
        }
    }
}

class yarp::manager::server::HTTPServer {
public:
    typedef yarp::manager::server::HTTPResponse (*RequestHandler)(void * context, std::map<std::string, std::string> headerParameters);

private:
    HTTPServer(const HTTPServer&);
    HTTPServer& operator=(const HTTPServer&);

    uint16_t m_serverPort;

    bool m_running;
    void * m_implementation;
public:
    HTTPServer();
    virtual ~HTTPServer();

    virtual bool startServer();
    virtual bool stopServer();
    virtual bool isRunning();

//    bool setContentType(HTTPContentType contentType);
    virtual bool addRequestHandle(HTTPMethod method, std::string relativeURL, RequestHandler handler, void* context);
    virtual bool removeRequestHandle(HTTPMethod method, std::string relativeURL);
    
};

class yarp::manager::server::HTTPResponse {
public:
    HTTPResponse();

    int returnCode;
    HTTPServerSerializable *content; //this is a risk for leak. copy it or explicitly make assumptions on the memory model
    std::string responseContentType;
};


#endif /* end of include guard: YARP_HTTPD_HTTPSERVER_H */
