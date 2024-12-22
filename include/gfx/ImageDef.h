//
// Created by tony on 25/09/24.
//

#pragma once

#include "../config/Export.h"

#include <cstdint>
#include <cstdlib>

namespace dagui
{
    class Image;

    class DAGUI_API ImageDef
    {
    public:
        ImageDef(std::size_t width, std::size_t height);

        virtual ~ImageDef() = default;

        virtual Image* createImage() const = 0;
    public:
        std::uint32_t x{0};
        std::uint32_t y{0};
        std::uint32_t width{0};
        std::uint32_t height{0};
        float u0{0.0f};
        float v0{0.0f};
        float u1{0.0f};
        float v1{0.0f};
        float u2{0.0f};
        float v2{0.0f};
        float u3{0.0f};
        float v3{0.0f};
    };
}
