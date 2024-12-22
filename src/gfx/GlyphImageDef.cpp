//
// Created by tony on 25/09/24.
//

#include "config/config.h"

#include "gfx/GlyphImageDef.h"
#include "gfx/Image.h"

#include <iostream>

namespace dagui
{
    GlyphImageDef::GlyphImageDef(std::uint32_t width, std::uint32_t height, FT_GlyphSlot glyphSlot)
        :
    ImageDef(width, height),
    _glyph(glyphSlot)
    {
        // Do nothing
    }

    Image* GlyphImageDef::createImage() const
    {
        if (_glyph->format != FT_GLYPH_FORMAT_BITMAP)
        {
            std::cout << "Rendering glyph\n";

            int error = FT_Render_Glyph( _glyph,   /* glyph slot  */
                                     FT_RENDER_MODE_NORMAL ); /* render mode */
            if (error)
            {
                std::cerr << "Failed to render glyph, bailing\n";

                // _errod = ERR_FAILED_TO_RENDER_GLYPH;
                return nullptr;
            }
        }
        auto image = new Image(width, height, 1, _glyph->bitmap.buffer);

        return image;
    }
}
