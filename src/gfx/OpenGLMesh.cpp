//
// Created by Tony Horrobin on 16/07/2025.
//

#include "config/config.h"

#include "gfx/OpenGLMesh.h"
#include "util/Searchable.h"

#include <cstdint>

namespace dagui
{
    OpenGLMesh::OpenGLMesh(Mesh *mesh)
    :
    _mesh(mesh)
    {
        // Do nothing.
    }

    void OpenGLMesh::allocate()
    {
        _vertices.allocate();
        _indices.allocate();
    }

    void OpenGLMesh::uploadVertices(AttributeArray &a)
    {
        _vertices.setArray(&a);
        _vertices.submit();
    }

    void OpenGLMesh::uploadIndices(IndexArray &a)
    {
        _indices.setArray(&a);
        _indices.submit();
    }

    void OpenGLMesh::draw()
    {
        _vertices.bind();
        _vertices.setPointers();
        _indices.bind();
        _indices.draw(GL_TRIANGLES);
    }

    dagbase::Variant OpenGLMesh::find(std::string_view path) const
    {
        dagbase::Variant retval;

        retval = dagbase::findEndpoint(path, "numVertexBuffers", std::uint32_t(numVertexBuffers()));
        if (retval.has_value())
            return retval;

        return {};
    }
}