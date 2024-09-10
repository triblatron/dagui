//
// Created by tony on 07/08/24.
//

#include "config/config.h"

#include "core/CompositeShape.h"

namespace dagui
{

    void CompositeShape::accept(ShapeVisitor &visitor)
    {
        for (auto shape : _shapes)
        {
            shape->accept(visitor);
        }
    }

    bool CompositeShape::isInside(double x, double y)
    {
        for (auto shape : _shapes)
        {
            if (shape->isInside(x,y))
            {
                return true;
            }
        }
        return false;
    }

    void CompositeShape::addShape(Shape *shape)
    {
        if (shape != nullptr)
        {
            _shapes.push_back(shape);
        }
    }
}