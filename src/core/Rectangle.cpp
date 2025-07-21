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
                            {_x, _y, 0.0f, 1.0f, 0.0f, 1.0f},
                            {_x+_width, _y, 0.0f, 1.0f, 0.0f, 1.0f},
                            {_x+_width, _y+_height, 0.0f, 1.0f, 0.0f, 1.0f},
                            {_x,_y+_height, 0.0f, 1.0f, 0.0f, 1.0f}
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
    }

    void Rectangle::addSubRectangle(float x, float y, float width, float height, Tessellation& tess)
    {
        ShapeVertex v[]=
                {
                        {x, y, 0.0, 1.0f, 0.0, 1.0f},
                        {x+width, y, 0.0f, 1.0f, 0.0, 1.0f},
                        {x+width, y+height, 0.0f, 1.0f, 0.0f, 1.0f},
                        {x, y+height, 0.0f, 1.0f, 0.0f, 1.0f}
                };

        tess.addQuad(v);
    }

    void Rectangle::addCorner(float x, float y, float xRadius, float yRadius, Tessellation &tess)
    {
        std::vector<ShapeVertex> v(_numCornerVertices + 1);

        v[0].x = x;
        v[0].y = y;
        v[0].g = 1.0f;
        v[0].a = 1.0f;
        for (auto i=1; i<v.size(); ++i)
        {
            float theta = float(i-1) * M_PI_2 / (_numCornerVertices-1);
            v[i] = { x + xRadius * cos(theta), y + yRadius * sin(theta), 0.0f, 1.0f, 0.0f, 1.0 };
        }
        tess.addTriangleFan(v);
    }

}
