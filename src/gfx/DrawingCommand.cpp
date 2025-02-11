//
// Created by Tony Horrobin on 11/02/2025.
//

#include "config/config.h"

#include "gfx/DrawingCommand.h"

namespace dagui::gl
{
    void DrawArrays::configure(dagbase::ConfigurationElement& config)
    {
        // Do nothing.
    }

    void DrawArrays::setup(const Mesh2D& mesh)
    {
        _vertexBuffer.bind();
        glBufferData(GL_ARRAY_BUFFER, mesh.numVertices()*sizeof(float), mesh.vertices(), GL_STATIC_DRAW);
        _colourBuffer.bind();
        glBufferData(GL_ARRAY_BUFFER, mesh.numColours()*sizeof(float), mesh.colours(), GL_STATIC_DRAW);
    }

    void DrawArrays::draw(const Mesh2D& mesh)
    {
        _colourBuffer.bind();
        glColorPointer(4, GL_FLOAT, 0, (void*)0);
        _vertexBuffer.bind();
        glVertexPointer(2, GL_FLOAT, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, mesh.numVertices()/2);
    }
}
