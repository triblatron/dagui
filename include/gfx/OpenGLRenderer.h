//
// Created by Tony Horrobin on 25/12/2024.
//

#pragma once

#include "gfx/Renderer.h"

namespace dagui
{
    class DAGUI_API OpenGLRenderer : public Renderer
    {
    public:
        void drawText(FT_Face face, TextureAtlas& atlas, std::string_view text) override;
        void generateTextureCoordinates(ImageDef& imageDef, BinImageDef& binImageDef) override;
    private:
        void drawTexturedQuad(float x, float y, const ImageDef* imageDef);
    };
}