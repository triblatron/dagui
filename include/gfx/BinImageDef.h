//
// Created by tony on 25/09/24.
//

#pragma once

#include "config/Export.h"

#include "gfx/ImageDef.h"

namespace nfe
{
    class NFE_API BinImageDef : public ImageDef
    {
    public:
        BinImageDef(std::size_t width, std::size_t height, std::size_t numComponents);

        Image* createImage() const override;
    private:
        std::size_t _numComponents{0};
    };
}