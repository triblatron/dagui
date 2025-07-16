//
// Created by Tony Horrobin on 25/12/2024.
//

#pragma once

#include "gfx/Renderer.h"
#include "util/APIVersion.h"
#include "core/ConfigurationElement.h"

namespace dagui
{
    namespace gl
    {
        class DrawingCommand;
        class OpenGL;
    }

    class DAGUI_API OpenGLRenderer : public Renderer
    {
    public:
        OpenGLRenderer();

        const APIVersion& apiVersion() const override
        {
            return _version;
        }

        void setOpenGL(gl::OpenGL* gl)
        {
            _gl = gl;
        }

        void configure(dagbase::ConfigurationElement& config);
        dagbase::ConfigurationElement::ValueType find(std::string_view path) const;
        void makeItSo(DrawCommandBuffer& buffer) override;
        void drawRect(const Rectangle& rect) override;
        void drawText(FT_Face face, TextureAtlas& atlas, std::string_view text) override;
        void drawMesh(Mesh& mesh) override;
        void drawMesh2D(const Mesh2D& mesh) override;
        void generateTextureCoordinates(ImageDef& imageDef, BinImageDef& binImageDef) override;
    private:
        APIVersion _version;
        void drawTexturedQuad(float x, float y, const ImageDef* imageDef);
        gl::OpenGL* _gl{nullptr};
        gl::DrawingCommand* _command{nullptr};
    };
}