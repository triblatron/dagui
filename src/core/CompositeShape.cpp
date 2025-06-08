//
// Created by tony on 07/08/24.
//

#include "config/config.h"

#include "core/CompositeShape.h"
#include "core/ConfigurationElement.h"
#include "util/Searchable.h"
#include "core/WidgetFactory.h"

namespace dagui
{

    void CompositeShape::accept(ShapeVisitor &visitor)
    {
        for (auto shape : _shapes)
        {
            shape->accept(visitor);
        }
    }

    bool CompositeShape::isInside(float x, float y)
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

    void CompositeShape::configure(dagbase::ConfigurationElement &config, WidgetFactory &factory)
    {
        if (auto element = config.findElement("children"); element)
        {
            element->eachChild([this, &factory](dagbase::ConfigurationElement& child) {
                auto childShape = factory.createShape(child);
                addShape(childShape);
                return true;
            });
        }
    }

    dagbase::Variant CompositeShape::find(std::string_view path) const
    {
        return Shape::find(path);
    }

    CompositeShape::CompositeShape()
    :
    Shape(dagbase::Atom::intern("CompositeShape"))
    {
        // Do nothing.
    }
}