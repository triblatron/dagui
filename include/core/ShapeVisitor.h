//
// Created by tony on 30/07/24.
//

#pragma once

#include "config/Export.h"

namespace dagui
{
    class Rectangle;

    class DAGUI_API ShapeVisitor
    {
    public:
        virtual ~ShapeVisitor() = default;

        virtual void visitRectangle(Rectangle& rectangle) = 0;
    };
}