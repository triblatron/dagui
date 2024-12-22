//
// Created by tony on 25/09/24.
//

#pragma once

#include "config/Export.h"

#include "gfx/ImageDef.h"

namespace dagui
{
    class DAGUI_API BinImageDef : public ImageDef
    {
    public:
        BinImageDef(std::uint32_t width, std::uint32_t height, std::uint32_t numComponents);

        Image* createImage() const override;

        std::uint32_t numComponents{0};
    };
}