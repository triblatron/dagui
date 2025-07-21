//
// Created by Tony Horrobin on 20/07/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Variant.h"
#include "core/Shape.h"

#include <string_view>
#include <map>
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
            if (auto it=_uniqueVertices.find(v); it==_uniqueVertices.end())
            {
                std::size_t index = _vertices.size();
                _uniqueVertices.emplace(v, index);
                _vertices.emplace_back(v);
                _indices.emplace_back(index);
            }
            else
            {
                _indices.emplace_back(it->second);
            }
        }

        void addQuad(const ShapeVertex v[4]);

        std::size_t numVertices() const
        {
            return _vertices.size();
        }

        std::size_t numIndices() const
        {
            return _indices.size();
        }

        std::size_t numTriangles() const
        {
            return _numTriangles;
        }

        void configure(dagbase::ConfigurationElement& config);

        void write(Mesh& mesh);

        dagbase::Variant find(std::string_view path) const;
    private:
        std::map<ShapeVertex, std::size_t> _uniqueVertices;
        std::vector<ShapeVertex> _vertices;
        std::vector<std::uint16_t> _indices;
        std::size_t _numTriangles{0};
    };
}