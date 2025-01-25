//
// Created by Tony Horrobin on 25/12/2024.
//

#pragma once

#include "gfx/Renderer.h"
#include "util/APIVersion.h"

namespace dagui
{
    class DAGUI_API OpenGLRenderer : public Renderer
    {
    public:
        OpenGLRenderer();

        const APIVersion& apiVersion() const override
        {
            return _version;
        }

        void drawText(FT_Face face, TextureAtlas& atlas, std::string_view text) override;
        void drawMesh2D(const Mesh2D& mesh) override;
        void generateTextureCoordinates(ImageDef& imageDef, BinImageDef& binImageDef) override;
    private:
        APIVersion _version;
        void drawTexturedQuad(float x, float y, const ImageDef* imageDef);
    };
}