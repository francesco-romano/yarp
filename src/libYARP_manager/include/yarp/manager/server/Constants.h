#ifndef YARP_HTTPD_CONSTANTS_H
#define YARP_HTTPD_CONSTANTS_H

#include <string>

namespace yarp {
    namespace manager {
        namespace server {
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

            typedef enum {
                HTTPContentTypeUnknown = 0,
                HTTPContentTypeJSON = 1
            } HTTPContentType;

            extern std::string HTTPMIMETypeJSON;
        }
    }
}

#endif /* end of include guard: YARP_HTTPD_CONSTANTS_H */
