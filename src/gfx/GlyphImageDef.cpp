//
// Created by tony on 25/09/24.
//

#include "config/config.h"

#include "gfx/GlyphImageDef.h"
#include "gfx/Image.h"

#include <iostream>

namespace nfe
{
    GlyphImageDef::GlyphImageDef(std::size_t width, std::size_t height, FT_GlyphSlot glyphSlot)
        :
    ImageDef(width, height),
    _glyph(glyphSlot)
    {
        // Do nothing
    }

    Image* GlyphImageDef::createImage() const
    {
        auto image = new Image(width(), height(), 1, _glyph->bitmap.buffer);

        return image;
    }
}
