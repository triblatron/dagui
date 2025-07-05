//
// Created by tony on 31/07/24.
//

#include "config/config.h"

#include "core/Circle.h"
#include "core/ConfigurationElement.h"
#include "util/Searchable.h"
#include "core/WidgetFactory.h"

namespace dagui
{
    void Circle::accept(ShapeVisitor &visitor)
    {

    }

    bool Circle::isInside(float x, float y)
    {
        return (x - _x) * (x - _x) + (y - _y) * (y - _y) < _radius * _radius;
    }

    void Circle::configure(dagbase::ConfigurationElement &config, ShapeFactory &factory)
    {
        dagbase::ConfigurationElement::readConfig(config, "radius", &_radius);
    }

    dagbase::Variant Circle::find(std::string_view path) const
    {
        dagbase::Variant retval = Shape::find(path);
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "radius", _radius);
        if (retval.has_value())
            return retval;

        return {};
    }

    Circle::Circle()
    :
    Shape(dagbase::Atom::intern("Circle"))
    {
        // Do nothing.
    }
}
