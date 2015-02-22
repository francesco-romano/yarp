
#include <yarp/manager/server/HTTPServer.h>
#include <yarp/manager/server/HTTPServerDispatchElement.h>
#include <microhttpd.h>
#include <map>

namespace yarp {
    namespace manager {
        namespace server {

        struct HTTPServerImplementation {

            typedef std::map<HTTPServerDispatchElementKey,
            HTTPServerDispatchElementValue,
            HTTPServerDispatchElementKey::Compare> DispatchTableType;

            DispatchTableType dispatchTable;

            /**
             * A client has requested the given url using the given method
             * (#MHD_HTTP_METHOD_GET, #MHD_HTTP_METHOD_PUT,
             * #MHD_HTTP_METHOD_DELETE, #MHD_HTTP_METHOD_POST, etc).  The callback
             * must call MHD callbacks to provide content to give back to the
             * client and return an HTTP status code (i.e. #MHD_HTTP_OK,
             * #MHD_HTTP_NOT_FOUND, etc.).
             *
             * @param cls argument given together with the function
             *        pointer when the handler was registered with MHD
             * @param connection the connection
             * @param url the requested url
             * @param method the HTTP method used (#MHD_HTTP_METHOD_GET,
             *        #MHD_HTTP_METHOD_PUT, etc.)
             * @param version the HTTP version string (i.e.
             *        #MHD_HTTP_VERSION_1_1)
             * @param upload_data the data being uploaded (excluding HEADERS,
             *        for a POST that fits into memory and that is encoded
             *        with a supported encoding, the POST data will NOT be
             *        given in upload_data and is instead available as
             *        part of #MHD_get_connection_values; very large POST
             *        data *will* be made available incrementally in
             *        @a upload_data)
             * @param upload_data_size set initially to the size of the
             *        @a upload_data provided; the method must update this
             *        value to the number of bytes NOT processed;
             * @param con_cls pointer that the callback can set to some
             *        address and that will be preserved by MHD for future
             *        calls for this request; since the access handler may
             *        be called many times (i.e., for a PUT/POST operation
             *        with plenty of upload data) this allows the application
             *        to easily associate some request-specific state.
             *        If necessary, this state can be cleaned up in the
             *        global #MHD_RequestCompletedCallback (which
             *        can be set with the #MHD_OPTION_NOTIFY_COMPLETED).
             *        Initially, `*con_cls` will be NULL.
             * @return #MHD_YES if the connection was handled successfully,
             *         #MHD_NO if the socket must be closed due to a serios
             *         error while handling the request
             */
            int
            handleHTTPRequest (void *cls,
                               struct MHD_Connection *connection,
                               const char *url,
                               const char *method,
                               const char *version,
                               const char *upload_data,
                               size_t *upload_data_size,
                               void **con_cls);

        };


        //Remove copy
        HTTPServer & HTTPServer::operator=(const HTTPServer &) {return *this;}
        HTTPServer::HTTPServer(const HTTPServer&) {}

        HTTPServer::HTTPServer()
        {
            this->_implementation = new HTTPServerImplementation();
        }

        HTTPServer::~HTTPServer()
        {
            delete (HTTPServerImplementation*)this->_implementation;
            this->_implementation = NULL;
        }


        //HTTP C functions

            int HTTPServerImplementation::handleHTTPRequest (void *cls,
                                                             struct MHD_Connection *connection,
                                                             const char *url,
                                                             const char *method,
                                                             const char *version,
                                                             const char *upload_data,
                                                             size_t *upload_data_size,
                                                             void **con_cls)
        {

            HTTPServerDispatchElementKey key(httpMethodFromString(method), url);

            DispatchTableType::const_iterator found = dispatchTable.find(key);

            if (found == dispatchTable.end()) {
                //respond with
//                MHD_HTTP_NOT_FOUND;
                return MHD_NO;
            }

            return found->second.processRequest();

        }

        }
    }
}
