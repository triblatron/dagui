//
// Created by Tony Horrobin on 25/12/2024.
//

#pragma once

#include "config/Export.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string_view>

namespace dagui
{
    class TextureAtlas;
    class BinImageDef;
    class ImageDef;

    class DAGUI_API Renderer
    {
    public:
        virtual ~Renderer() = default;

        virtual void drawText(FT_Face face, TextureAtlas& atlas, std::string_view text) = 0;
        virtual void generateTextureCoordinates(ImageDef& imageDef, BinImageDef& binImageDef) = 0;
    };
}
