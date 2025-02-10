//
// Created by Tony Horrobin on 09/02/2025.
//

#include "config/config.h"

#include "gfx/OpenGLMesh2D.h"

#include <iostream>
#include <ostream>

namespace dagui::gl
{
    void OpenGLMesh2D::allocate()
    {
        _vertexBuffer.allocate();
        _indexBuffer.allocate();
    }

    void OpenGLMesh2D::bind()
    {
        _vertexBuffer.bind();
        // _indexBuffer.bind();
    }

    void OpenGLMesh2D::unbind()
    {
        _vertexBuffer.unbind();
        // _indexBuffer.unbind();
    }

    void OpenGLMesh2D::submit()
    {
        bind();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, (void*)_vertices.data());
        glColorPointer(4, GL_FLOAT, 0, (void*)_colours.data());
        //glBufferData(GL_ARRAY_BUFFER, _vertices.size()*2, _vertices.data(), GL_STATIC_DRAW);

        std::cout << glGetError() << std::endl;
    }


    void OpenGLMesh2D::draw()
    {
        bind();
        glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    }
}
