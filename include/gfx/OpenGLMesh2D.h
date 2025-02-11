//
// Created by Tony Horrobin on 09/02/2025.
//

#pragma once

#include "config/Export.h"

#include "gfx/Mesh2D.h"
#include "gfx/OpenGL.h"

namespace dagui::gl
{
    class DAGUI_API OpenGLMesh2D : public Mesh2D
    {
    public:
        OpenGLMesh2D() = default;

        ~OpenGLMesh2D() override = default;

        void allocate();

        void bind();

        void unbind();

        void submit();

        void draw() override;
    private:
        gl::VertexBuffer _vertexBuffer;
        gl::VertexBuffer _colourBuffer;
        gl::IndexBuffer _indexBuffer;
    };
}