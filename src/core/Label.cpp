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

        if (auto element=config.findElement("text"); element)
        {
            _text = new Text();
            _text->setText(element->asString());
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

    void Label::draw(Batcher &batcher)
    {
        Widget::draw(batcher);
    }
}
