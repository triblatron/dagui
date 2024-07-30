//
// Created by tony on 30/07/24.
//

#pragma once

#include "config/Export.h"

namespace nfe
{
    class Rectangle;

    class NFE_API ShapeVisitor
    {
    public:
        virtual ~ShapeVisitor() = default;

        virtual void visitRectangle(Rectangle& rectangle) = 0;
    };
}