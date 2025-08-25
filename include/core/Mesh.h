//
// Created by Tony Horrobin on 05/07/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Variant.h"
#include "gfx/AttributeDescriptor.h"

#include <cstdlib>
#include <cstdint>
#include <string_view>

namespace dagui
{
    class AttributeArray;
    class IndexArray;
    class MeshBackend;

    //! \class Mesh
    //! Abstract base class for meshes consisting of index triangle lists
    class DAGUI_API Mesh
    {
    public:
        virtual ~Mesh() = default;

        virtual void addVertex(const char* buf, std::size_t bufLen) = 0;

        virtual std::size_t numVertices() const = 0;

        virtual std::size_t numTriangles() const = 0;

        virtual void getVertex(std::size_t index, char* buf, std::size_t* bufferLen) = 0;

        //! \retval The attribute array with the given index
        //! \retval nullptr if the index is out of bounds
        virtual AttributeArray* attributeArray(std::size_t arrayIndex) = 0;

        virtual IndexArray* indexArray() = 0;

        virtual AttributeArray* attributeArrayForUsage(AttributeDescriptor::Usage usage, std::size_t* outputArrayIndex, std::size_t* attrIndex) = 0;

        virtual void addIndex(const char* buf, std::size_t bufLen) = 0;

        virtual std::size_t numIndices() const = 0;

        virtual void getIndex(std::size_t i, char* buf, std::size_t bufLen) = 0;

        virtual void configure(dagbase::ConfigurationElement& config) = 0;

        virtual dagbase::Variant find(std::string_view path) const = 0;

        virtual void allocateBuffers() = 0;

        virtual void sendToBackend() = 0;

        void setBackend(MeshBackend* backend)
        {
            _backend = backend;
        }

        MeshBackend* backend()
        {
            return _backend;
        }

        virtual void draw() = 0;
    private:
        MeshBackend* _backend{nullptr};
    };
}
