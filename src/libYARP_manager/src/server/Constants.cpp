#include <yarp/manager/server/Constants.h>
#include <string>
#include <microhttpd.h>

namespace yarp {
    namespace manager {
        namespace server {

            std::string HTTPMIMETypeJSON = "application/json";

            std::string stringRepresentationForHTTPMethod(HTTPMethod method)
            {
                switch (method) {
                    case HTTPMethodGet:
                        return MHD_HTTP_METHOD_GET;
                    case HTTPMethodHead:
                        return MHD_HTTP_METHOD_HEAD;
                    case HTTPMethodPost:
                        return MHD_HTTP_METHOD_POST;
                    case HTTPMethodPut:
                        return MHD_HTTP_METHOD_PUT;
                    case HTTPMethodDelete:
                        return MHD_HTTP_METHOD_DELETE;
                    default:
                        return "";
                }
            }

            HTTPMethod httpMethodFromString(std::string method)
            {
                if (method.compare(MHD_HTTP_METHOD_GET) == 0)
                    return HTTPMethodGet;
                if (method.compare(MHD_HTTP_METHOD_HEAD) == 0)
                    return HTTPMethodHead;
                if (method.compare(MHD_HTTP_METHOD_POST) == 0)
                    return HTTPMethodPost;
                if (method.compare(MHD_HTTP_METHOD_PUT) == 0)
                    return HTTPMethodPut;
                if (method.compare(MHD_HTTP_METHOD_DELETE) == 0)
                    return HTTPMethodDelete;
                return HTTPMethodNotDefined;
            }

        }
    }
}
