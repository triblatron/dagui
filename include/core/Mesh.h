//
// Created by Tony Horrobin on 05/07/2025.
//

#pragma once

#include "config/Export.h"

#include <cstdlib>
#include <cstdint>

namespace dagui
{
    class AttributeArray;

    //! \class Mesh
    //! Abstract base class for meshes consisting of index triangle lists
    class DAGUI_API Mesh
    {
    public:
        virtual ~Mesh() = default;

        virtual void addVertex(const char* buf, std::size_t bufLen) = 0;

        virtual std::size_t numVertices() const = 0;

        //! \retval The attribute array with the given index
        //! \retval nullptr if the index is out of bounds
        virtual const AttributeArray* attributeArray(std::size_t arrayIndex) const = 0;

        virtual void addIndex(std::uint16_t value) = 0;

        virtual std::size_t numIndices() const = 0;
    };
}
