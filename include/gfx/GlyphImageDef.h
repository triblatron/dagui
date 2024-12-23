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
        GlyphImageDef(std::uint32_t width, std::uint32_t height, FT_GlyphSlot glyphSlot);

        Image* createImage() const override;
    private:
        FT_GlyphSlot _glyph;
    };
}