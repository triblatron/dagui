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
            PRIMITIVE_TRIANGLE_STRIP,
            PRIMITIVE_TRIANGLE_FAN
        };
    public:
        Mesh2D() = default;

        std::size_t numVertices() const
        {
            return _vertices.size();
        }

        const Vec2f* vertices() const
        {
            return _vertices.data();
        }

        static const char* primitiveTypeToString(PrimitiveType);
        static PrimitiveType parsePrimitiveType(const char *str);
    private:
        using VertexArray = std::vector<Vec2f>;
        VertexArray _vertices;
    };
}
