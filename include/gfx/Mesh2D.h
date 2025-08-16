//
// Created by Tony Horrobin on 25/01/2025.
//

#pragma once

#include "config/Export.h"
#include "core/Vec2f.h"
#include "util/Searchable.h"

#include <vector>

namespace dagbase
{
    class ConfigurationElement;
}

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

        virtual ~Mesh2D() = default;

        void configure(dagbase::ConfigurationElement& config);

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
            _vertices.emplace_back(vertex.x);
            _vertices.emplace_back(vertex.y);
        }

        const float* vertices() const
        {
            return _vertices.data();
        }

        std::size_t numColours() const
        {
            return _colours.size();
        }

        void addColour(float r, float g, float b, float a)
        {
            _colours.emplace_back(r);
            _colours.emplace_back(g);
            _colours.emplace_back(b);
            _colours.emplace_back(a);
        }

        const float* colours() const
        {
            return _colours.data();
        }

        std::size_t numIndices() const
        {
            return _indices.size();
        }

        void addIndex(const std::size_t index)
        {
            _indices.emplace_back(index);
        }

        const std::uint32_t* indices() const
        {
            return _indices.data();
        }

        static const char* primitiveTypeToString(PrimitiveType);
        static PrimitiveType parsePrimitiveType(const char *str);
    protected:
        PrimitiveType _primitiveType = PRIMITIVE_UNKNOWN;
        using VertexArray = std::vector<float>;
        VertexArray _vertices;
        using IndexArray = std::vector<std::uint32_t>;
        IndexArray _indices;
        using ColourArray = std::vector<float>;
        ColourArray _colours;
    };
}
