//
// Created by tony on 25/09/24.
//

#pragma once

#include "config/Export.h"
#include "ImageDef.h"

#include <ft2build.h>
#include <benchmark/benchmark.h>

#include FT_FREETYPE_H

namespace nfe
{
    class NFE_API GlyphImageDef : public ImageDef
    {
    public:
        GlyphImageDef(std::size_t width, std::size_t height, FT_GlyphSlot glyphSlot);

        Image* createImage() const override;
    private:
        FT_GlyphSlot _glyph;
    };
}