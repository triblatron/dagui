//
// Created by Tony Horrobin on 20/07/2025.
//

#include "config/config.h"

#include "core/Tessellation.h"
#include "core/ConfigurationElement.h"
#include "util/Searchable.h"
#include "gfx/Mesh2D.h"

namespace dagui
{
    void Tessellation::configure(dagbase::ConfigurationElement &config)
    {
        Mesh2D::PrimitiveType primitiveType = Mesh2D::PRIMITIVE_UNKNOWN;

        dagbase::ConfigurationElement::readConfig<Mesh2D::PrimitiveType>(config, "primitiveType", Mesh2D::parsePrimitiveType, &primitiveType);
        std::vector<ShapeVertex> vertices;
        if (auto element=config.findElement("vertices"); element)
        {
            element->eachChild([this,&vertices](dagbase::ConfigurationElement& child) {
                ShapeVertex v;
                v.configure(child);
                vertices.emplace_back(v);

                return true;
            });
        }
        switch (primitiveType)
        {
            case Mesh2D::PRIMITIVE_TRIANGLE_FAN:
                addTriangleFan(vertices);
                break;
        }

    }

    dagbase::Variant Tessellation::find(std::string_view path) const
    {
        dagbase::Variant retval;

        retval = dagbase::findEndpoint(path, "numVertices", std::uint32_t(numVertices()));
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "numIndices", std::uint32_t(numIndices()));
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "numTriangles", std::uint32_t(numTriangles()));
        if (retval.has_value())
            return retval;

        return {};
    }

    //! Add a triangle fan to the tessellation.
    //! Creates a series of triangles because we use index triangle
    //! lists internally which are allegedly efficient o modern GPU hardware.
    void Tessellation::addTriangleFan(const std::vector<ShapeVertex> &vertices)
    {
        // Convert to triangle list.
        for (auto vertIndex=1; vertIndex<vertices.size()-1; ++vertIndex)
        {
            addVertex(vertices[0]);
            addVertex(vertices[vertIndex]);
            addVertex(vertices[vertIndex+1]);
        }
        _numTriangles += vertices.size() - 2;
    }

    //! Emit vertices and indices to the given Mesh
    //! \note Assumes the mesh has ShapeVertex as
    //! its Vertex type and std::uint16_t as its index type.
    void Tessellation::write(ShapeMesh &mesh)
    {
        for (auto v : _vertices)
        {
            mesh.addVertex((const char*)&v,sizeof(ShapeVertex));
        }
        for (auto i : _indices)
        {
            mesh.addIndex((const char*)&i, sizeof(std::uint16_t));
        }
    }

    //! Add a quadrilateral to the tessellation.
    //! \note Assumes counter clock-wise winding.
    void Tessellation::addQuad(const ShapeVertex v[4])
    {
        addVertex(v[0]);
        addVertex(v[1]);
        addVertex(v[2]);
        addVertex(v[2]);
        addVertex(v[3]);
        addVertex(v[0]);
        _numTriangles+=2;
    }
}