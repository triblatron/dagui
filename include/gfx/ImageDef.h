//
// Created by tony on 25/09/24.
//

#pragma once

#include "../config/Export.h"

#include <cstdint>

namespace nfe
{
    class Image;

    class NFE_API ImageDef
    {
    public:
        ImageDef(std::size_t width, std::size_t height);

        std::size_t width() const
        {
            return _width;
        }

        std::size_t height() const
        {
            return _height;
        }

        Image* createImage(std::size_t numComponents) const;
    private:
        std::size_t _width{0};
        std::size_t _height{0};
    };
}