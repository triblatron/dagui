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
            if (x >= _x + _cornerRadius && x < _x + _width - _cornerRadius && y >= _y && y < _y + _height)
            {
                return true;
            }
            else if (x >= _x && x <_x + _cornerRadius && y >= _y + _cornerRadius && y < _y + _height - _cornerRadius)
            {
                return true;
            }
            else if (x >= _x + _width - _cornerRadius && x < _x + _width && y >= _y + _cornerRadius && y < _y + _height - _cornerRadius)
            {
                return true;
            }
            else
            {
                // Top-left corner
                if (isInCorner(x, y, _x + _cornerRadius, _y + _cornerRadius))
                {
                    return true;
                }
                else if (isInCorner(x, y, _x + _width - _cornerRadius, _y + _cornerRadius))
                {
                    return true;
                }
                else if (isInCorner(x, y, _x + _cornerRadius, _y + _height - _cornerRadius))
                {
                    return true;
                }
                else if (isInCorner(x, y, _x + _width - _cornerRadius, _y + _height - _cornerRadius))
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

    bool Rectangle::isInCorner(double x, double y, double cornerX, double cornerY)
    {
        double deltaX = x - cornerX;
        deltaX *= deltaX;
        double deltaY = y - cornerY;
        deltaY *= deltaY;
        double dist = deltaX + deltaY;
        if (dist < _cornerRadius * _cornerRadius)
        {
            return true;
        }

        return false;
    }
}