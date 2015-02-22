#ifndef YARP_MANAGER_HTTPSERVER_H
#define YARP_MANAGER_HTTPSERVER_H

namespace yarp {
    namespace manager {
        namespace server {
            class HTTPServer;
        }
    }
}


class yarp::manager::server::HTTPServer {
private:
    HTTPServer(const HTTPServer&);
    HTTPServer& operator=(const HTTPServer&);

    void * _implementation;
public:
    HTTPServer();
    ~HTTPServer();


};



#endif /* end of include guard: YARP_MANAGER_HTTPSERVER_H */
