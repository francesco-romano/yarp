
#include <yarp/manager/server/HTTPServerDispatchElement.h>
#include <microhttpd.h>

namespace yarp {
    namespace manager {
        namespace server {

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


            HTTPServerDispatchElementKey::HTTPServerDispatchElementKey(HTTPMethod method, std::string relativeURL)
            : method(method)
            , url(relativeURL)
            { }


            bool HTTPServerDispatchElementKey::Compare::operator()(const HTTPServerDispatchElementKey& left, const HTTPServerDispatchElementKey& right) const
            {
                //Exception for ordering: method ANY
//                int mask = left.method & HTTPMethodAny || right.method & HTTPMethodAny;


                return left.method < right.method
                || (left.method == right.method && left.url.compare(right.url));
            }


            int HTTPServerDispatchElementValue::processRequest() const
            {
                if (this->requestHandler)
                    return requestHandler();
                return MHD_HTTP_NOT_IMPLEMENTED;
            }
            
        }
    }
}
