
#include <yarp/manager/server/HTTPServerDispatchElement.h>
#include <microhttpd.h>

namespace yarp {
    namespace manager {
        namespace server {

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


            HTTPResponse HTTPServerDispatchElementValue::processRequest(std::map<std::string, std::string> headerParameters) const
            {
                HTTPResponse response;
                if (this->requestHandler) {
                    response = requestHandler(this->context);
                    //prepare response object
                    return response;
                }
                response.returnCode = MHD_HTTP_NOT_IMPLEMENTED;
                return response;
            }
            
        }
    }
}
