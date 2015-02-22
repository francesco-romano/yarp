#ifndef YARP_MANAGER_HTTPSERVERSERIALIZATION_H
#define YARP_MANAGER_HTTPSERVERSERIALIZATION_H

#include <string>
#include <vector>
#include <map>
#include <sstream>

namespace yarp {
    namespace manager {
        namespace server {

            class HTTPServerSerializer;
            class HTTPServerSerializable;

            class HTTPServerSerializableArray;
            class HTTPServerSerializableDictionary;
            template <typename T>
            class HTTPServerSerializableElement;

            class HTTPServerSerializable {
            public:
                virtual ~HTTPServerSerializable();
                virtual std::string serialization(HTTPServerSerializer* serializer) = 0;
            };

            class HTTPServerSerializer {
            public:
                virtual ~HTTPServerSerializer();
                virtual std::string serialize(HTTPServerSerializable* serializable) = 0;
                virtual std::string contentType() const = 0;
            };


            //Concrete visitors

            class HTTPServerJSONSerializer : public HTTPServerSerializer {
            public:
                virtual ~HTTPServerJSONSerializer();
                virtual std::string serialize(HTTPServerSerializable* serializable);
                virtual std::string serialize(HTTPServerSerializableArray* serializable);
                virtual std::string serialize(HTTPServerSerializableDictionary* serializable);
                virtual std::string contentType() const;
            };


            //Concrete elements
            class HTTPServerSerializableArray : public std::vector<HTTPServerSerializable*>, HTTPServerSerializable
            {
            public:
                virtual ~HTTPServerSerializableArray();
                virtual std::string serialization(HTTPServerSerializer* serializer);
            };

            class HTTPServerSerializableDictionary : public std::map<std::string, HTTPServerSerializable*>, HTTPServerSerializable
            {
            public:
                virtual ~HTTPServerSerializableDictionary();
                virtual std::string serialization(HTTPServerSerializer* serializer);
            };

            template <typename T>
            class HTTPServerSerializableElement : public HTTPServerSerializable
            {
                T object;
            public:
                HTTPServerSerializableElement(T object) : object(object) {}
                virtual ~HTTPServerSerializableElement() {}
                virtual std::string serialization(HTTPServerSerializer* serializer)
                {
                    std::ostringstream string;
                    return string.str();
                }
            };

        }
    }
}

#endif /* end of include guard: YARP_MANAGER_HTTPSERVERSERIALIZATION_H */
