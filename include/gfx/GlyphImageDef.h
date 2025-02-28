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
        GlyphImageDef(FT_Face face, FT_UInt glyphIndex, std::uint32_t width, std::uint32_t height, std::uint32_t advance);

        Image* createImage() const override;

        std::uint32_t index() const override
        {
            return _glyphIndex;
        }

        std::uint32_t advance() const override
        {
            return _advance;
        }
    private:
        FT_Face _face{nullptr};
        FT_UInt _glyphIndex{0};
        std::uint32_t _advance{0};
    };
}