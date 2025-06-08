//
// Created by tony on 30/07/24.
//

#include "config/config.h"

#include "core/Rectangle.h"
#include "core/ShapeVisitor.h"
#include "core/ConfigurationElement.h"
#include "util/Searchable.h"

namespace dagui
{
    Rectangle::Rectangle()
    :
    Shape(dagbase::Atom::intern("Rectangle"))
    {
        // Do notbing.
    }

    bool Rectangle::isInside(float x, float y)
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

    bool Rectangle::isInCorner(float x, float y, float d, float d1)
    {
        double deltaX = x - d;
        deltaX *= deltaX;
        double deltaY = y - d1;
        deltaY *= deltaY;
        double dist = deltaX + deltaY;
        if (dist < _cornerRadius * _cornerRadius)
        {
            return true;
        }

        return false;
    }

    void Rectangle::configure(dagbase::ConfigurationElement &config, WidgetFactory &factory)
    {
        dagbase::ConfigurationElement::readConfig(config, "cornerRadius", &_cornerRadius);
    }

    dagbase::Variant Rectangle::find(std::string_view path) const
    {
        dagbase::Variant retval = Shape::find(path);
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "cornerRadius", _cornerRadius);
        if (retval.has_value())
            return retval;

        return {};
    }
}
