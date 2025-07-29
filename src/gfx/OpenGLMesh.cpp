//
// Created by Tony Horrobin on 16/07/2025.
//

#include "config/config.h"

#include "gfx/OpenGLMesh.h"
#include "util/Searchable.h"

#include <cstdint>

namespace dagui
{
    OpenGLMesh::OpenGLMesh()
    {
        // Do nothing.
    }

    void OpenGLMesh::addVertexBuffer()
    {
        _vertexBuffers.emplace_back(new gl::VertexBuffer());
    }

    void OpenGLMesh::allocate()
    {
        for (auto vb : _vertexBuffers)
        {
            vb->allocate();
        }
        _indices.allocate();
    }
        
    void OpenGLMesh::uploadVertices(std::size_t bufferIndex, AttributeArray &a)
    {
        if (bufferIndex<_vertexBuffers.size())
        {
            _vertexBuffers[bufferIndex]->setArray(&a);
            _vertexBuffers[bufferIndex]->submit();
        }
    }

    void OpenGLMesh::uploadIndices(IndexArray &a)
    {
        _indices.setArray(&a);
        _indices.submit();
    }

    void OpenGLMesh::draw()
    {
        for (auto vb : _vertexBuffers)
        {
            vb->bind();
            vb->setPointers();
        }
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