//
// Created by Tony Horrobin on 25/01/2025.
//

#pragma once

#include <vector>

#include "config/Export.h"
#include "core/Vec2f.h"

namespace dagui
{
    class DAGUI_API Mesh2D
    {
    public:
        enum PrimitiveType
        {
            PRIMITIVE_UNKNOWN,
            PRIMITIVE_POINT,
            PRIMITIVE_LINE,
            PRIMITIVE_LINE_STRIP,
            PRIMITIVE_LINE_LOOP,
            PRIMITIVE_TRIANGLE,
            PRIMITIVE_TRIANGLE_LIST,
            PRIMITIVE_TRIANGLE_STRIP,
            PRIMITIVE_TRIANGLE_FAN
        };
    public:
        Mesh2D() = default;

        ~Mesh2D() = default;

        void setPrimitiveType(PrimitiveType type)
        {
            _primitiveType = type;
        }

        PrimitiveType primitiveType() const
        {
            return _primitiveType;
        }

        std::size_t numVertices() const
        {
            return _vertices.size();
        }

        void addVertex(const Vec2f& vertex)
        {
            _vertices.push_back(vertex);
        }

        const Vec2f* vertices() const
        {
            return _vertices.data();
        }

        std::size_t numIndices() const
        {
            return _indices.size();
        }

        void addIndex(const std::size_t index)
        {
            _indices.push_back(index);
        }

        const unsigned int* indices() const
        {
            return _indices.data();
        }

        static const char* primitiveTypeToString(PrimitiveType);
        static PrimitiveType parsePrimitiveType(const char *str);
    private:
        PrimitiveType _primitiveType = PRIMITIVE_UNKNOWN;
        using VertexArray = std::vector<Vec2f>;
        VertexArray _vertices;
        using IndexArray = std::vector<std::uint32_t>;
        IndexArray _indices;
    };
}
