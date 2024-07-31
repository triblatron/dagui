//
// Created by tony on 31/07/24.
//

#include "config/config.h"

#include "core/Circle.h"

namespace nfe
{
    void Circle::accept(ShapeVisitor &visitor)
    {

    }

    bool Circle::isInside(double x, double y)
    {
        return (x - _x) * (x - _x) + (y - _y) * (y - _y) < _radius * _radius;
    }
}
