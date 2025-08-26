//
// Created by Tony Horrobin on 26/08/2025.
//

#pragma once

#include "config/Export.h"

#include <glm/glm.hpp>

namespace dagui
{
    class DAGUI_API PositionStack
    {
    public:
        virtual ~PositionStack() = default;

        virtual void push() = 0;

        virtual void translate(float x, float y) = 0;

        virtual bool empty() const = 0;

        virtual glm::vec2 top() const = 0;

        virtual void pop() = 0;
    };
}