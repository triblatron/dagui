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
        _colourBuffer.allocate();
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
        _vertexBuffer.bind();
        glBufferData(GL_ARRAY_BUFFER, _vertices.size()*sizeof(float), _vertices.data(), GL_STATIC_DRAW);
        _colourBuffer.bind();
        glBufferData(GL_ARRAY_BUFFER, _colours.size()*sizeof(float), _colours.data(), GL_STATIC_DRAW);
        std::cout << glGetError() << std::endl;
    }


    void OpenGLMesh2D::draw()
    {
        _colourBuffer.bind();
        glColorPointer(4, GL_FLOAT, 0, (void*)0);
        _vertexBuffer.bind();
        glVertexPointer(2, GL_FLOAT, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
        std::cout << glGetError() << std::endl;
    }
}
