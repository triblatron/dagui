//
// Created by tony on 25/09/24.
//

#include "config/config.h"

#include "gfx/BinImageDef.h"
#include "gfx/Image.h"

namespace dagui
{
    BinImageDef::BinImageDef(std::uint32_t width, std::uint32_t height, std::size_t numComponents)
        :
    ImageDef(width, height),
    _numComponents(numComponents)
    {
        // Do nothing.
    }

    Image* BinImageDef::createImage() const
    {
        return new Image(width, height, _numComponents);
    }

}