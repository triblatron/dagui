//
// Created by tony on 30/07/24.
//

#pragma once

#include "config/Export.h"

namespace nfe
{
    class ShapeVisitor;

    class NFE_API Shape
    {
    public:
        virtual ~Shape() = default;

        virtual void accept(ShapeVisitor& visitor) = 0;

        virtual bool isInside(double x, double y) = 0;
    };
}