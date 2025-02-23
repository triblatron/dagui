//
// Created by Tony Horrobin on 12/02/2025.
//

#include "config/Export.h"

#include <vector>

#include "GenericAttributeArray.h"
#include "OpaqueAttributeArray.h"
#include "core/ConfigurationElement.h"

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    struct ArrayDescriptor;
    class AttributeArray;
    struct AttributeLayout;

    template<typename Vertex>
    class GenericMesh
    {
    public:
        enum Flags : std::uint32_t
        {
            FLAG_NONE = 0,
            FLAG_OWN = 1<<0
        };
    public:
        GenericMesh() = default;

        ~GenericMesh()
        {
            for (auto a : _data)
                delete a;
        }

        void addVertex(const Vertex& v)
        {
            bool vertexAdded = false;

            for (auto a : _data)
            {
                a->addVertex(&v, sizeof(Vertex));
                vertexAdded = true;
            }

            if (vertexAdded)
                ++_numVertices;
        }

        void getVertex(std::size_t i, Vertex* v)
        {
            if (v)
            {
                for (auto a : _data)
                {
                    a->getVertex(i, v, sizeof(Vertex));
                }
            }
        }

        std::size_t size() const
        {
            return _numVertices;
        }

        void configure(dagbase::ConfigurationElement& config)
        {
            if (auto element = config.findElement("arrays"); element)
            {
                std::size_t offsetSoFar = 0;
                element->eachChild([this,&offsetSoFar](dagbase::ConfigurationElement& child)
                {
                    ArrayDescriptor descriptor;
                    descriptor.offsetSoFar = offsetSoFar;
                    descriptor.configure(child);
                    offsetSoFar += descriptor.size();
                    auto array = new OpaqueAttributeArray();
                    array->setDescriptor(descriptor);
                    _data.push_back(array);
                    return true;
                });
            }
        }
    private:
        using AttributeArrays = std::vector<AttributeArray*>;
        AttributeArrays _data;
        std::size_t _numVertices{ 0 };
    };
}
