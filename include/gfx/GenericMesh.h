//
// Created by Tony Horrobin on 12/02/2025.
//

#pragma once

#include "config/Export.h"

#include <vector>

#include "GenericAttributeArray.h"
#include "OpaqueAttributeArray.h"
#include "OpaqueIndexArray.h"
#include "core/ConfigurationElement.h"
#include "core/Mesh.h"
#include "util/Searchable.h"
#include "util/SearchableArray.h"
#include "gfx/MeshBackend.h"

#include <limits>

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

        ~GenericMesh() override
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

        AttributeArray* attributeArrayForUsage(AttributeDescriptor::Usage usage, std::size_t* outputArrayIndex, std::size_t* outputAttrIndex) override
        {
            for (auto arrayIndex=0; arrayIndex<_data.size(); ++arrayIndex)
            {
                for (auto attrIndex=0; attrIndex<_data.a[arrayIndex]->desciptor().attributes.size(); ++attrIndex)
                {
                    if (_data.a[arrayIndex]->desciptor().attributes[attrIndex].attr.usage == usage)
                    {
                        if (outputArrayIndex)
                            *outputArrayIndex = arrayIndex;

                        if (outputAttrIndex)
                            *outputAttrIndex = attrIndex;

                        return _data.a[arrayIndex];
                    }
                }
            }

            if (outputArrayIndex)
                *outputArrayIndex = std::numeric_limits<std::size_t>::max();

            if (outputAttrIndex)
                *outputAttrIndex = std::numeric_limits<std::size_t>::max();

            return nullptr;
        }

        void addIndex(const char* buf, std::size_t bufLen) override
        {
            if (_indices && buf && bufLen>=_indices->descriptor().size())
            {
                _indices->addIndex(buf, bufLen);
            }
        }

        void getIndex(std::size_t i, char* buf, std::size_t bufLen) override
        {
            if (_indices)
                _indices->getIndex(i, buf, bufLen);
        }

        std::size_t numVertices() const override
        {
            return _numVertices;
        }

        IndexArray* indexArray() override
        {
            return _indices;
        }

        std::size_t numIndices() const override
        {
            if (_indices)
                return _indices->size();
            else
                return 0;
        }

        std::size_t numTriangles() const override
        {
            return numIndices()/3;
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
                    _data.a.emplace_back(array);
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

            if (auto element = config.findElement("indexArray"); element)
            {
                IndexArrayDescriptor descriptor;

                descriptor.configure(*element);
                _indices = new OpaqueIndexArray();
                _indices->setDescriptor(descriptor);
            }

            if (auto element = config.findElement("indices"); element && _indices)
            {
                element->eachChild([this](dagbase::ConfigurationElement& child) {
                    std::uint16_t index = child.asInteger();
                    _indices->addIndex(&index, sizeof(std::uint16_t));
                    return true;
                });
            }
        }

        dagbase::Variant find(std::string_view path) const override
        {
            dagbase::Variant retval;

            retval = dagbase::findEndpoint(path, "numVertices", std::uint32_t(numVertices()));
            if (retval.has_value())
                return retval;

            retval = dagbase::findEndpoint(path, "numIndices", std::uint32_t(numIndices()));
            if (retval.has_value())
                return retval;

            retval = dagbase::findEndpoint(path, "numTriangles", std::uint32_t(numIndices()/3));
            if (retval.has_value())
                return retval;

            retval = dagbase::findInternal(path, "vertices", _data);
            if (retval.has_value())
                return retval;

            return {};
        }

        void allocateBuffers() override
        {
            if (backend())
            {
                for (auto arrayIndex = 0; arrayIndex < _data.size(); ++arrayIndex)
                {
                    backend()->addVertexBuffer();
                }
            }
        }

        void sendToBackend() override
        {
            if (backend())
            {
                backend()->allocate();
                for (auto arrayIndex = 0; arrayIndex<_data.size(); ++arrayIndex)
                {
                    auto attrArray = _data.a[arrayIndex];
                    backend()->uploadVertices(arrayIndex, *attrArray);
                }
                if (indexArray())
                    backend()->uploadIndices(*indexArray());
            }
        }

        void draw() override
        {
            if (backend())
            {
                backend()->draw();
            }
        }

        bool operator==(const GenericMesh<Vertex>& other) const;

        bool operator!=(const GenericMesh<Vertex>& other) const
        {
            return !(*this == other);
        }

        GenericMesh(const GenericMesh<Vertex>& other)
        {
            _data = other._data;
            _numVertices = other._numVertices;
            if (other._indices)
            {
                _indices = new OpaqueIndexArray(*other._indices);
            }
        }

        GenericMesh<Vertex>& operator=(const GenericMesh<Vertex>& other)
        {
            if (this != &other)
            {
                for (auto a : _data.a)
                {
                    delete a;
                }
                _data = other._data;
                _numVertices = other._numVertices;
                delete _indices;
                if (other._indices)
                {
                    _indices = new OpaqueIndexArray(*other._indices);
                }
            }
            return *this;
        }
    private:
        using AttributeArrays = dagbase::SearchableArray<std::vector<OpaqueAttributeArray*>>;
        AttributeArrays _data;
        std::size_t _numVertices{ 0 };
        OpaqueIndexArray* _indices{nullptr};
    };

    template<typename Vertex>
    bool GenericMesh<Vertex>::operator==(const GenericMesh<Vertex> &other) const
    {
        if (!(_data == other._data))
        {
            return false;
        }

        if (_numVertices != other._numVertices)
            return false;

        if ((_indices && !other._indices) || (!_indices && other._indices))
            return false;

        if (_indices && other._indices && !(*_indices == *other._indices))
            return false;

        return true;
    }
}
