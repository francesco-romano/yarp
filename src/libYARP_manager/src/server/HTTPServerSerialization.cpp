#include <yarp/manager/server/HTTPServerSerialization.h>
#include <sstream>

namespace yarp {
    namespace manager {
        namespace server {

            HTTPServerSerializable::~HTTPServerSerializable() {};

            HTTPServerSerializer::~HTTPServerSerializer() {}

            //Concrete visitors
            HTTPServerJSONSerializer::~HTTPServerJSONSerializer() {}
            std::string HTTPServerJSONSerializer::serialize(HTTPServerSerializable* serializable) {
                return serializable->serialization(this);
            }
            std::string HTTPServerJSONSerializer::serialize(HTTPServerSerializableArray* serializable)
            {
                std::ostringstream string;
                string << "[";
                for (size_t index = 0; index < serializable->size() - 1; index++) {
                    string << serializable->at(index)->serialization(this) << ",";
                }
                if (serializable->size() > 0)
                    string << serializable->at(serializable->size() - 1)->serialization(this);

                string << "]";
                return string.str();

            }
            std::string HTTPServerJSONSerializer::serialize(HTTPServerSerializableDictionary* serializable)
            {
                std::ostringstream string;
                string << "{";
                std::map<std::string, HTTPServerSerializable*>::iterator it = serializable->begin();

                while (it != serializable->end()) {
                    string << "\"" << it->first << "\"";
                    string << it->second->serialization(this);
                    it++;
                    if (it != serializable->end()) {
                        string << ",";
                    }
                }
                string << "}";
                return string.str();
            }
            std::string HTTPServerJSONSerializer::contentType() const
            {
                return "application/json";
            }

            //Concrete elements
            HTTPServerSerializableArray::~HTTPServerSerializableArray() {};
            std::string HTTPServerSerializableArray::serialization(HTTPServerSerializer* serializer)
            {
                return serializer->serialize(this);
            }

            HTTPServerSerializableDictionary::~HTTPServerSerializableDictionary() {}
            std::string HTTPServerSerializableDictionary::serialization(HTTPServerSerializer* serializer)
            {
                return serializer->serialize(this);
            }

        }
    }
}
