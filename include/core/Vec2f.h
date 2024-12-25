//
// Created by Tony Horrobin on 25/12/2024.
//

#pragma once

namespace dagui
{
    struct Vec2f
    {
        Vec2f() = default;

        Vec2f(float x, float y)
            :
        x(x),
        y(y)
        {
            // Do nothing.
        }

        bool operator==(const Vec2f& rhs) const
        {
            return x == rhs.x && y == rhs.y;
        }

        float x{0.0f};
        float y{0.0f};
    };
}