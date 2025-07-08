//
// Created by Tony Horrobin on 12/02/2025.
//

#pragma once

#include "config/Export.h"

#include <vector>

#include "GenericAttributeArray.h"
#include "OpaqueAttributeArray.h"
#include "core/ConfigurationElement.h"
#include "core/Mesh.h"
#include "util/Searchable.h"
#include "util/SearchableArray.h"

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
    class GenericMesh : public Mesh
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
            for (auto a : _data.a)
                delete a;
        }

        void addVertex(const Vertex& v)
        {
            bool vertexAdded = false;

            for (auto a : _data.a)
            {
                a->addVertex(&v, sizeof(Vertex));
                vertexAdded = true;
            }

            if (vertexAdded)
                ++_numVertices;
        }

        void addVertex(const char* buf, std::size_t bufSize) override
        {
            if (bufSize>=sizeof(Vertex))
            {
                auto* vertexBuf = reinterpret_cast<const Vertex*>(buf);
                addVertex(*vertexBuf);
            }
        }

        void getVertex(std::size_t i, Vertex* v)
        {
            if (v)
            {
                for (auto a : _data.a)
                {
                    a->getVertex(i, v, sizeof(Vertex));
                }
            }
        }

        void getVertex(std::size_t i, char* buf, std::size_t* bufLen) override
        {
            if (buf && bufLen && *bufLen >= sizeof(Vertex))
            {
                getVertex(i, (Vertex*)buf);
            }
        }

        AttributeArray* attributeArray(std::size_t arrayIndex) override
        {
            if (arrayIndex<_data.size())
            {
                return _data.a[arrayIndex];
            }

            return nullptr;
        }

        AttributeArray* attributeArrayForUsage(AttributeDescriptor::Usage usage, std::size_t* outputAttrIndex) override
        {
            for (auto arrayIndex=0; arrayIndex<_data.size(); ++arrayIndex)
            {
                for (auto attrIndex=0; attrIndex<_data.a[arrayIndex]->desciptor().size(); ++attrIndex)
                {
                    if (_data.a[arrayIndex]->desciptor().attributes[attrIndex].attr.usage == usage)
                    {
                        if (outputAttrIndex)
                        {
                            *outputAttrIndex = attrIndex;
                        }
                        return _data.a[arrayIndex];
                    }
                }
            }

            return nullptr;
        }

        void addIndex(std::uint16_t index) override
        {
            _indices.emplace_back(index);
        }

        void getIndex(std::size_t index, std::uint16_t *value) override
        {
            if (value && index<_indices.size())
            {
                *value = _indices[index];
            }
        }

        std::size_t numVertices() const override
        {
            return _numVertices;
        }

        std::size_t numIndices() const override
        {
            return _indices.size();
        }

        void configure(dagbase::ConfigurationElement& config) override
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
                    _data.a.push_back(array);
                    return true;
                });
            }

            if (auto element = config.findElement("vertices"); element)
            {
                element->eachChild([this](dagbase::ConfigurationElement& child) {
                    Vertex v;
                    v.configure(child);
                    addVertex(v);

                    return true;
                });
            }

            if (auto element = config.findElement("indices"); element)
            {
                element->eachChild([this](dagbase::ConfigurationElement& child) {
                    std::uint16_t index = child.asInteger();
                    _indices.emplace_back(index);
                    return true;
                });
            }
        }

        dagbase::Variant find(std::string_view path) const override
        {
            dagbase::Variant retval;

            retval = dagbase::findEndpoint(path, "numVertices", std::int64_t(numVertices()));
            if (retval.has_value())
                return retval;

            retval = dagbase::findEndpoint(path, "numIndices", std::int64_t(numIndices()));
            if (retval.has_value())
                return retval;

            retval = dagbase::findInternal(path, "vertices", _data);
            if (retval.has_value())
                return retval;

            return {};
        }
    private:
        using AttributeArrays = dagbase::SearchableArray<std::vector<OpaqueAttributeArray*>>;
        AttributeArrays _data;
        std::size_t _numVertices{ 0 };
        using IndexAray = std::vector<std::uint16_t>;
        IndexAray _indices;
    };
}
