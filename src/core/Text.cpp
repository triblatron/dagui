//
// Created by Tony Horrobin on 30/04/2025.
//

#include "config/config.h"

#include "core/Text.h"
#include "util/Searchable.h"
#include "core/ShapeVisitor.h"
#include "gfx/TextureAtlas.h"
#include <ft2build.h>
#include FT_FREETYPE_H

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
        if (auto element = config.findElement("atlas"); element)
        {
            _atlas = new TextureAtlas();
            _atlas->configure(*element);
        }

        if (auto element = config.findElement("face"); element)
        {
            _face = dagbase::Atom::intern(element->asString());
        }

        dagbase::ConfigurationElement::readConfig(config, "text", &_text);
    }
}