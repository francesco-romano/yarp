
#include <yarp/manager/server/HTTPServer.h>
#include <yarp/manager/server/HTTPServerSerialization.h>
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

                typedef std::map<std::string, HTTPServerSerializer*> SerializerTable;
                SerializerTable serializers; //key: content-type

                MHD_Daemon *daemon;

                HTTPServerImplementation();
                ~HTTPServerImplementation();
            };

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

            //FIXME: remove hardcoded port
            HTTPServer::HTTPServer() : m_serverPort(9999)
            {
                this->m_implementation = new HTTPServerImplementation();
            }

            HTTPServer::~HTTPServer()
            {
                HTTPServerImplementation *implementation = static_cast<HTTPServerImplementation*>(m_implementation);
                if (implementation) {
                    delete implementation;
                    this->m_implementation = NULL;
                }
            }

            bool HTTPServer::startServer()
            {
                HTTPServerImplementation *implementation = static_cast<HTTPServerImplementation*>(m_implementation);
                if (!implementation) return false;

                //Check if server is already running
                if (implementation->daemon) return false;

                implementation->daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION,
                                                          m_serverPort,
                                                          NULL, NULL,
                                                          &handleHTTPRequest, implementation);

                return implementation->daemon != NULL;
            }

            bool HTTPServer::stopServer()
            {
                HTTPServerImplementation *implementation = static_cast<HTTPServerImplementation*>(m_implementation);
                if (!implementation) return false;

                //server is not running
                if (!implementation->daemon) return false;

                MHD_stop_daemon(implementation->daemon);
                implementation->daemon = NULL;

                return true;
            }

            bool HTTPServer::isRunning()
            {
                HTTPServerImplementation *implementation = static_cast<HTTPServerImplementation*>(m_implementation);
                if (!implementation) return false;
                return implementation->daemon != NULL;
            }

//            bool HTTPServer::setContentType(HTTPContentType contentType)
//            {
//                HTTPServerImplementation *implementation = static_cast<HTTPServerImplementation*>(m_implementation);
//                if (!implementation) return false;
//
//                switch (contentType) {
//                    case HTTPContentTypeJSON:
//                        break;
//                    default: return false; //content type not recognized
//                }
//
//                if (implementation->serializer) {
//                    delete implementation->serializer;
//                }
//
//                switch (contentType) {
//                    case HTTPContentTypeJSON:
//                        implementation->serializer = new HTTPServerJSONSerializer();
//                        break;
//                    default: break;
//                }
//                return true;
//            }

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

            HTTPResponse::HTTPResponse() : returnCode(-1), content(NULL) {}

            HTTPServerImplementation::HTTPServerImplementation()
            : daemon(NULL)
            {
                HTTPServerJSONSerializer *jsonSerializer = new HTTPServerJSONSerializer();
                serializers.insert(SerializerTable::value_type(jsonSerializer->contentType(), jsonSerializer));
            }

            HTTPServerImplementation::~HTTPServerImplementation() {

            }

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
                    HTTPServerImplementation::SerializerTable::iterator serializer = serverData->serializers.find(response.responseContentType);
                    if (serializer != serverData->serializers.end())
                        responseContent = serializer->second->serialize(response.content);
                    else {
                        response.returnCode = MHD_HTTP_INTERNAL_SERVER_ERROR;
                    }
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
            
        }
    }
}
