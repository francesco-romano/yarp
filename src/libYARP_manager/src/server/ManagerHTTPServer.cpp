#include <yarp/manager/server/ManagerHTTPServer.h>
#include <yarp/manager/server/HTTPServerSerialization.h>
#include <yarp/manager/manager.h>
#include <yarp/manager/application.h>
#include <vector>
#include <microhttpd.h>

namespace yarp {
    namespace manager {
        namespace server {

            std::string ManagerHTTPServerGetApplications = "/Applications";
            std::string ManagerHTTPServerGetApplicationNamed = "/Application/";


            struct ManagerHTTPServerContext {
                yarp::manager::Manager *manager;
            };

            static HTTPResponse listApplications(void * context, std::map<std::string, std::string> headerParameters) {
                ManagerHTTPServerContext* managerData = static_cast<ManagerHTTPServerContext*>(context);
                HTTPResponse response;
                response.responseContentType = HTTPMIMETypeJSON;

                if (!managerData) { response.returnCode = MHD_NO; return response; }
                const ApplicaitonPContainer& applications = managerData->manager->getKnowledgeBase()->getApplications();

                HTTPServerSerializableArray applicationNames;
                applicationNames.reserve(applications.size());

                for (ApplicaitonPContainer::const_iterator it = applications.begin(); it != applications.end(); it++) {
                    applicationNames.push_back(new HTTPServerSerializableElement<std::string>((*it)->getName()));
                }


                return response;
            }

            ManagerHTTPServer::ManagerHTTPServer(const yarp::manager::Manager& manager):
            m_manager(manager)
            {
                ManagerHTTPServerContext* context = new ManagerHTTPServerContext();
                context->manager = const_cast<yarp::manager::Manager*>(&m_manager);
                m_private = context;
                initRequestHandlers();
            }

            bool ManagerHTTPServer::initRequestHandlers()
            {
                bool result = true;
                result = result && this->addRequestHandle(HTTPMethodGet, "/Applications", &listApplications, m_private);

                return result;
            }

        }
    }
}