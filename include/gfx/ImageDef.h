//
// Created by tony on 25/09/24.
//

#pragma once

#include "../config/Export.h"
#include "core/Vec2f.h"

#include <cstdint>
#include <cstdlib>

namespace dagui
{
    class Image;

    class DAGUI_API ImageDef
    {
    public:
        ImageDef() = default;

        ImageDef(std::size_t width, std::size_t height);

        virtual ~ImageDef() = default;

        virtual Image* createImage() const = 0;
    public:
        std::uint32_t x{0};
        std::uint32_t y{0};
        std::uint32_t width{0};
        std::uint32_t height{0};
        Vec2f p0;
        Vec2f p1;
        Vec2f p2;
        Vec2f p3;
    };
}
