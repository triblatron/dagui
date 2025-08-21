//
// Created by Tony Horrobin on 30/04/2025.
//

#include "config/config.h"

#include "core/Text.h"
#include "util/Searchable.h"
#include "core/ShapeVisitor.h"

namespace dagui
{
    dagbase::Variant Text::find(std::string_view path) const
    {
        auto retval = Shape::find(path);

        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "text", _text);
        if (retval.has_value())
            return retval;

        return {};
    }

    Text::Text()
    :
    Shape(dagbase::Atom::intern("Text"))
    {
        // Do nothing.s
    }

    bool Text::isInside(float x, float y)
    {
        return false;
    }

    void Text::accept(ShapeVisitor &visitor)
    {
        //visitor.(*this);
    }

    void Text::configure(dagbase::ConfigurationElement &config, ShapeFactory& shapeFactory)
    {
        dagbase::ConfigurationElement::readConfig(config, "text", &_text);
    }
}