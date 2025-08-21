//
// Created by Tony Horrobin on 17/04/2025.
//

#include "config/config.h"

#include "core/Label.h"
#include "util/Searchable.h"
#include "core/Batcher.h"

namespace dagui
{

    Label::Label(Widget *parent)
    :
    Widget(dagbase::Atom::intern("Label"), parent)
    {
    }

    void Label::configure(dagbase::ConfigurationElement &config, WidgetFactory &factory, ShapeFactory &shapeFactory)
    {
        Widget::configure(config, factory, shapeFactory);

        if (auto element = config.findElement("face"); element)
        {
            _face = shapeFactory.createShape(*element);
            addShape(_face);
        }

        if (auto element=config.findElement("text"); element)
        {
            _text = dynamic_cast<Text*>(shapeFactory.createShape(*element));
            addShape(_text);
        }

        if (auto element=config.findElement("bounds"); element)
        {
            _bounds = element->value().asVec2();
        }
    }

    dagbase::ConfigurationElement::ValueType Label::find(std::string_view path) const
    {
        auto retval = Widget::find(path);
        if (retval.has_value())
        {
            return retval;
        }

        if (_text)
        {
            retval = dagbase::findInternal(path, "text", *_text);
            if (retval.has_value())
                return retval;
        }

        retval = dagbase::findEndpoint(path, "bounds", dagbase::Variant(_bounds));
        if (retval.has_value())
            return retval;

        return {};
    }

    void Label::draw(Batcher &batcher, GraphicsBackendFactory& factory)
    {
        Widget::draw(batcher, factory);
    }
}
