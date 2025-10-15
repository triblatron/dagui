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
#include "core/Tessellation.h"

#include "gfx/AttributeArray.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES 1
#endif

#include <math.h>

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
        dagbase::ConfigurationElement::readConfig(config, "numCornerVertices", &_numCornerVertices);
        if (auto element = config.findElement("colour"); element && element->numChildren()==4)
        {
            _colour.r = float(element->child(0)->asDouble());
            _colour.g = float(element->child(1)->asDouble());
            _colour.b = float(element->child(2)->asDouble());
            _colour.a = float(element->child(3)->asDouble());
        }
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

        if (_cornerRadius==0.0)
        {
            ShapeVertex v[]=
                    {
                            {_x, _y, _colour.r, _colour.g, _colour.b, _colour.a},
                            {_x+_width, _y, _colour.r, _colour.g, _colour.b, _colour.a},
                            {_x+_width, _y+_height, _colour.r, _colour.g, _colour.b, _colour.a},
                            {_x,_y+_height, _colour.r, _colour.g, _colour.b, _colour.a}
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
        }
        else
        {
            Tessellation tess;

            addSubRectangle(_x+_cornerRadius, _y, _width-2.0f*_cornerRadius, _cornerRadius, tess);
            addSubRectangle(_x+_width-_cornerRadius, _y+_cornerRadius, _cornerRadius, _height - 2.0f * _cornerRadius, tess);
            addSubRectangle(_x+_cornerRadius, _y+_height-_cornerRadius, _width-2.0f*_cornerRadius, _cornerRadius, tess);
            addSubRectangle(_x, _y+_cornerRadius, _cornerRadius, _height-2.0f*_cornerRadius, tess);
            addSubRectangle(_x+_cornerRadius, _y+_cornerRadius, _width-2.0f*_cornerRadius, _height-2.0f*_cornerRadius, tess);
            addCorner(_x+_cornerRadius, _y+_cornerRadius, -_cornerRadius, -_cornerRadius, tess);
            addCorner(_x+(_width-_cornerRadius), _y+_cornerRadius, _cornerRadius, -_cornerRadius, tess);
            addCorner(_x+_cornerRadius, _y+(_height-_cornerRadius), -_cornerRadius, _cornerRadius, tess);
            addCorner(_x+(_width-_cornerRadius), _y+(_height-_cornerRadius), _cornerRadius, _cornerRadius, tess);
            tess.write(mesh);
        }
        clearFlag(FLAGS_DIRTY_TESSELLATION_BIT);
    }

    void Rectangle::addSubRectangle(float x, float y, float width, float height, Tessellation& tess)
    {
        ShapeVertex v[]=
                {
                        {x, y, _colour.r, _colour.g, _colour.b, _colour.a},
                        {x+width, y, _colour.r, _colour.g, _colour.b, _colour.a},
                        {x+width, y+height, _colour.r, _colour.g, _colour.b, _colour.a},
                        {x, y+height, _colour.r, _colour.g, _colour.b, _colour.a}
                };

        tess.addQuad(v);
    }

    void Rectangle::addCorner(float x, float y, float xRadius, float yRadius, Tessellation &tess)
    {
        std::vector<ShapeVertex> v(_numCornerVertices + 1);

        v[0].x = x;
        v[0].y = y;
        v[0].r = _colour.r;
        v[0].g = _colour.g;
        v[0].b = _colour.b;
        v[0].a = _colour.a;
        for (std::size_t i=1; i<v.size(); ++i)
        {
            float theta = float(i-1) * M_PI_2 / (_numCornerVertices-1);
            v[i] = { x + xRadius * cos(theta), y + yRadius * sin(theta), _colour.r, _colour.g, _colour.b, _colour.a };
        }
        tess.addTriangleFan(v);
    }

}
