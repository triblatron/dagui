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
        if (_cornerRadius==0.0)
        {
            return x>=_x && x<_x+_width && y>=_y && y<_y+_height;
        }
        else
        {
            if (x >= _x + _cornerRadius && x < _x + _width - _cornerRadius && y >= _y + _cornerRadius && y < _y + _height - _cornerRadius)
            {
                return true;
            }
            else
            {
                double deltaX = x - (_x + _cornerRadius);
                deltaX *= deltaX;
                double deltaY = y - (_y + _cornerRadius);
                deltaY *= deltaY;
                double dist = deltaX + deltaY;
                if (dist < _cornerRadius * _cornerRadius)
                {
                    return true;
                }
            }
        }

        return false;
    }

    void Rectangle::accept(ShapeVisitor &visitor)
    {
        visitor.visitRectangle(*this);
    }
}