//
// Created by tony on 30/07/24.
//

#include "config/config.h"
#include "core/Shape.h"
#include "util/Searchable.h"
#include "util/enums.h"

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

    std::string Shape::flagsToString(Shape::Flags value)
    {
        std::string retval;

        BIT_NAME(value, FLAGS_DIRTY_TESSELLATION_BIT, retval)

        if (!retval.empty() && retval.back()==' ')
        {
            retval = retval.substr(0, retval.length()-1);
        }

        if (retval.empty())
            retval = "FLAGS_NONE";

        return retval;
    }

    Shape::Flags Shape::parseFlags(const std::string& str)
    {
        Flags value = FLAGS_NONE;

        TEST_BIT(FLAGS_DIRTY_TESSELLATION_BIT, str, value);

        return value;
    }

    FontImageSource *Shape::lookupFontImageSource(dagbase::Atom name)
    {
        if (_fontImageSourceLookup)
        {
            return _fontImageSourceLookup->lookup(name);
        }

        return nullptr;
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

        retval = dagbase::findEndpoint(path, "r", r);
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "g", g);
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "b", b);
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "a", a);
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "u", u);
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "v", v);
        if (retval.has_value())
            return retval;

        return {};
    }

    void ShapeVertex::configure(dagbase::ConfigurationElement &config)
    {
        dagbase::ConfigurationElement::readConfig(config, "x", &x);
        dagbase::ConfigurationElement::readConfig(config, "y", &y);
        dagbase::ConfigurationElement::readConfig(config, "r", &r);
        dagbase::ConfigurationElement::readConfig(config, "g", &g);
        dagbase::ConfigurationElement::readConfig(config, "b", &b);
        dagbase::ConfigurationElement::readConfig(config, "a", &a);
        dagbase::ConfigurationElement::readConfig(config, "u", &u);
        dagbase::ConfigurationElement::readConfig(config, "v", &v);
    }

    bool ShapeVertex::operator<(const ShapeVertex &other) const
    {
        if (x<other.x)
        {
            return true;
        }
        else if (std::abs(x-other.x)<0.001 && y<other.y)
        {
            return true;
        }
        return false;
    }
}