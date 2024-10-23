//
// Created by tony on 25/09/24.
//

#pragma once

#include "../config/Export.h"

#include <cstdint>
#include <cstdlib>

namespace nfe
{
    class Image;

    class NFE_API ImageDef
    {
    public:
        ImageDef(std::size_t width, std::size_t height);

        virtual ~ImageDef() = default;

        std::size_t width() const
        {
            return _width;
        }

        std::size_t height() const
        {
            return _height;
        }

        void setPos(std::size_t x, std::size_t y)
        {
            _x = x;
            _y = y;
        }

        std::size_t x() const
        {
            return _x;
        }

        std::size_t y() const
        {
            return _y;
        }

        virtual Image* createImage() const = 0;
    private:
        std::size_t _x{0};
        std::size_t _y{0};
        std::size_t _width{0};
        std::size_t _height{0};
    };
}