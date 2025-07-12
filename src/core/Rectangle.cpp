//
// Created by tony on 30/07/24.
//

#include "config/config.h"

#include "core/Rectangle.h"
#include "core/ShapeVisitor.h"
#include "core/ConfigurationElement.h"
#include "util/Searchable.h"
#include "core/DrawCommandBuffer.h"
#include "core/Mesh.h"
#include "gfx/AttributeDescriptor.h"
#include "core/ShapeFactory.h"

#include "gfx/AttributeArray.h"

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

    void Rectangle::configure(dagbase::ConfigurationElement &config, ShapeFactory &factory)
    {
        dagbase::ConfigurationElement::readConfig( config, "x", &_x);
        dagbase::ConfigurationElement::readConfig( config, "y", &_y);
        dagbase::ConfigurationElement::readConfig( config, "width", &_width);
        dagbase::ConfigurationElement::readConfig( config, "height", &_height);
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

        retval = dagbase::findEndpoint(path, "x", _x);
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "y", _y);
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "width", _width);
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "height", _height);
        if (retval.has_value())
            return retval;

        return {};
    }

    void Rectangle::render(DrawCommandBuffer &buffer)
    {
        buffer.drawRect(*this);
    }

    void Rectangle::tessellate(ShapeMesh &mesh)
    {
        ShapeVertex v[]=
                {
                        {_x, _y},
                        {_x+_width, _y},
                        {_x+_width, _y+_height},
                        {_x,_y+_height}
                };
        std::uint16_t indices[]=
                {
                    0,
                    1,
                    2,
                    2,
                    3,
                    0,
                };

        for (auto i=0; i<4; ++i)
        {
            mesh.addVertex(v[i]);
        }

        for (auto i=0; i<6; ++i)
        {
            mesh.addIndex((const char*)&indices[i], sizeof(std::uint16_t));
        }
//        std::size_t attrIndex = std::numeric_limits<std::size_t>::max();
//        auto positionArray = mesh.attributeArrayForUsage(AttributeDescriptor::USAGE_POSITION, &attrIndex);
//        if (positionArray)
//        {
//            for (auto i=0; i<4; ++i)
//            {
//                positionArray->addVertex(&v[i], 2 * sizeof(float));
//            }
//        }
    }

}
