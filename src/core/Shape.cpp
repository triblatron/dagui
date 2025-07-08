//
// Created by tony on 30/07/24.
//

#include "config/config.h"
#include "core/Shape.h"
#include "util/Searchable.h"

namespace dagui
{
    dagbase::Variant dagui::Shape::find(std::string_view path) const
    {
        dagbase::Variant retval;

        if (!_className.empty()) {
            retval = dagbase::findEndpoint(path, "class", std::string(_className.value()));
            if (retval.has_value())
                return retval;
        }

        return {};
    }

    Shape::Shape(dagbase::Atom className)
    :
    _className(className)
    {
        // Do nothing.
    }

    dagbase::Variant ShapeVertex::find(std::string_view path) const
    {
        dagbase::Variant retval;

        retval = dagbase::findEndpoint(path, "x", x);
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "y", y);
        if (retval.has_value())
            return retval;

        return {};
    }

    void ShapeVertex::configure(dagbase::ConfigurationElement &config)
    {
        dagbase::ConfigurationElement::readConfig(config, "x", &x);
        dagbase::ConfigurationElement::readConfig(config, "y", &y);
    }
}