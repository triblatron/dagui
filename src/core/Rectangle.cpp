//
// Created by tony on 30/07/24.
//

#include "config/config.h"

#include "core/Rectangle.h"
#include "core/ShapeVisitor.h"

namespace nfe
{
    Rectangle::Rectangle()
    {

    }

    bool Rectangle::isInside(double x, double y)
    {
        return x>=_x && x<_x+_width && y>=_y && y<_y+_height;
    }

    void Rectangle::accept(ShapeVisitor &visitor)
    {
        visitor.visitRectangle(*this);
    }
}