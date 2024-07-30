//
// Created by tony on 30/07/24.
//

#pragma once

#include "config/Export.h"

namespace nfe
{
    class Shape;

    class NFE_API Widget
    {
    public:
        virtual ~Widget() = default;

    private:
        Shape* _shape{nullptr};
    };
}