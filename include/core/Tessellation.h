//
// Created by Tony Horrobin on 20/07/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Variant.h"
#include "core/Shape.h"

#include <string_view>
#include <vector>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class DAGUI_API Tessellation
    {
    public:
        Tessellation() = default;

        void addTriangleFan(const std::vector<ShapeVertex>& vertices);

        void addVertex(const ShapeVertex& v)
        {
            _vertices.emplace_back(v);
        }

        std::size_t numVertices() const
        {
            return _vertices.size();
        }

        std::size_t numTriangles() const
        {
            return _numTriangles;
        }

        void configure(dagbase::ConfigurationElement& config);

        dagbase::Variant find(std::string_view path) const;
    private:
        std::vector<ShapeVertex> _vertices;
        std::size_t _numTriangles{0};
    };
}