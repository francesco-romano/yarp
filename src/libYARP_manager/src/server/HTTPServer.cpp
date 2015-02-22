
#include <yarp/manager/server/HTTPServer.h>
#include <yarp/manager/server/HTTPServerSerialization.h>
#include <yarp/manager/server/HTTPServerDispatchElement.h>
#include <microhttpd.h>
#include <map>

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


            struct HTTPServerImplementation {

                typedef std::map<HTTPServerDispatchElementKey,
                HTTPServerDispatchElementValue,
                HTTPServerDispatchElementKey::Compare> DispatchTableType;

                DispatchTableType dispatchTable;

                HTTPServerSerializer *serializer;

                ~HTTPServerImplementation();
            };
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
            static int
            handleHTTPRequest(void *cls,
                              struct MHD_Connection *connection,
                              const char *url,
                              const char *method,
                              const char *version,
                              const char *upload_data,
                              size_t *upload_data_size,
                              void **con_cls);

            static int parseHeaderRequest(void *cls, enum MHD_ValueKind kind,
                                          const char *key, const char *value);



            //Remove copy
            HTTPServer & HTTPServer::operator=(const HTTPServer &) {return *this;}
            HTTPServer::HTTPServer(const HTTPServer&) {}

            HTTPServer::HTTPServer()
            {
                this->m_implementation = new HTTPServerImplementation();
            }

            HTTPServer::~HTTPServer()
            {
                HTTPServerImplementation *implementation = static_cast<HTTPServerImplementation*>(m_implementation);
                if (implementation) {
                    //                    delete (HTTPServerImplementation*)this->m_implementation;
                    delete implementation;
                    this->m_implementation = NULL;
                }
            }


            bool HTTPServer::startServer()
            {
                m_running = true;
                return m_running;
            }

            bool HTTPServer::stopServer()
            {
                m_running = false;
                return m_running;
            }

            bool HTTPServer::isRunning() { return m_running; }

            bool HTTPServer::addRequestHandle(HTTPMethod method, std::string relativeURL, RequestHandler handler, void* context)
            {
                if (m_running) return false;
                HTTPServerImplementation *implementation = static_cast<HTTPServerImplementation*>(m_implementation);
                if (!implementation) return false;

                HTTPServerDispatchElementKey key(method, relativeURL);
                HTTPServerDispatchElementValue value;
                value.requestHandler = handler;

                HTTPServerImplementation::DispatchTableType::value_type pair(key, value);
                implementation->dispatchTable.insert(pair);

                return true;
            }

            bool HTTPServer::removeRequestHandle(HTTPMethod method, std::string relativeURL)
            {
                if (m_running) return false;
                HTTPServerImplementation *implementation = static_cast<HTTPServerImplementation*>(m_implementation);
                if (!implementation) return false;

                HTTPServerDispatchElementKey key(method, relativeURL);
                implementation->dispatchTable.erase(key);

                return true;
            }

            HTTPResponse::HTTPResponse() : returnCode(-1), content(0) {}

            //HTTP C functions

            int handleHTTPRequest(void *cls,
                                  struct MHD_Connection *connection,
                                  const char *url,
                                  const char *method,
                                  const char *version,
                                  const char *upload_data,
                                  size_t *upload_data_size,
                                  void **con_cls)
            {
                HTTPServerImplementation *serverData = static_cast<HTTPServerImplementation*>(cls);
                if (!serverData) return MHD_NO;

                //Avoid to do stuff if this is the first call
                if (!*con_cls) {*con_cls = connection; return MHD_YES;}

                HTTPServerDispatchElementKey key(httpMethodFromString(method), url);

                HTTPServerImplementation::DispatchTableType::const_iterator found = serverData->dispatchTable.find(key);

                HTTPResponse response;
                if (found == serverData->dispatchTable.end()) {
                    response.returnCode = MHD_HTTP_NOT_FOUND;
                } else {
                    //Parse parameters
                    std::map<std::string, std::string> header;
                    MHD_get_connection_values(connection, MHD_HEADER_KIND, &parseHeaderRequest, &header);
                    response = found->second.processRequest(); //pass parameters here
                }

                std::string responseContent = "";
                if (response.content) {
                    responseContent = serverData->serializer->serialize(response.content);
                }
                MHD_Response *mhdResponse;
                mhdResponse = MHD_create_response_from_buffer(responseContent.size(), (void*)responseContent.c_str(), MHD_RESPMEM_MUST_COPY);
                int result = MHD_queue_response(connection, response.returnCode, mhdResponse);
                MHD_destroy_response(mhdResponse);
                //generate response
                return result;
            }

            int parseHeaderRequest(void *cls, enum MHD_ValueKind kind,
                                   const char *key, const char *value)
            {
                std::map<std::string, std::string> *header = static_cast<std::map<std::string, std::string>*>(cls);
                if (!header) return MHD_NO;
                header->insert(std::pair<std::string, std::string>(key, value ? : ""));
                return MHD_YES;
            }

            HTTPServerImplementation::~HTTPServerImplementation() {
                if (serializer) {
                    delete serializer;
                    serializer = NULL;
                }
            }
            
        }
    }
}
