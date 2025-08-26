//
// Created by Tony Horrobin on 26/08/2025.
//

#pragma once

#include "config/Export.h"

#include "core/PositionStack.h"

namespace dagui
{
    class OpenGLPositionStack : public PositionStack
    {
    public:
        void push() override;

        void translate(float x, float y) override;

        bool empty() const override
        {
            return _empty;
        }

        glm::vec2 top() const override;

        void pop() override;
    private:
        bool _empty{true};
        glm::vec2 _translation{};
    };
}