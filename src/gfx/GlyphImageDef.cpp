//
// Created by tony on 25/09/24.
//

#include "config/config.h"

#include "gfx/GlyphImageDef.h"
#include "gfx/Image.h"

#include <iostream>

namespace dagui
{
    GlyphImageDef::GlyphImageDef(FT_Face face, FT_UInt glyphIndex, std::uint32_t width, std::uint32_t height)
        :
    ImageDef(width, height),
    _face(face),
    _glyphIndex(glyphIndex)
    {
        // Do nothing
    }

    Image* GlyphImageDef::createImage() const
    {
        int error = FT_Load_Glyph(
    _face,          /* handle to face object */
    _glyphIndex,   /* glyph index           */
    0x0 );  /* load flags, see below */
        if (error)
        {
            std::cerr << "Error loading glyph\n";

            return nullptr;
        }

        if (_face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
        {
            std::cout << "Rendering glyph\n";

            int error = FT_Render_Glyph( _face->glyph,   /* glyph slot  */
                                     FT_RENDER_MODE_NORMAL ); /* render mode */
            if (error)
            {
                std::cerr << "Failed to render glyph, bailing\n";

                // _errod = ERR_FAILED_TO_RENDER_GLYPH;
                return nullptr;
            }
        }
        auto image = new Image(width, height, 1, _face->glyph->bitmap.buffer);
        std::cout << "Image has " << image->count(255,255,255) << " white pixels\n";
        return image;
    }
}
