//
// Created by tony on 25/09/24.
//

#pragma once

#include "config/Export.h"
#include "ImageDef.h"

#include <ft2build.h>

#include FT_FREETYPE_H
#include <cstdint>
#include <cstdlib>

namespace dagui
{
    class DAGUI_API GlyphImageDef : public ImageDef
    {
    public:
        GlyphImageDef(FT_Face face, FT_UInt glyphIndex, std::uint32_t width, std::uint32_t height);

        Image* createImage() const override;
    private:
        FT_Face _face{nullptr};
        FT_UInt _glyphIndex{0};
    };
}