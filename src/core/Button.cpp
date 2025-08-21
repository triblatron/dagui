//
// Created by Tony Horrobin on 12/04/2025.
//

#include "config/config.h"

#include "core/Button.h"

#include "util/Searchable.h"
#include "core/ShapeFactory.h"

namespace dagui
{
    Button::Button(Widget* parent)
        :
    Widget(dagbase::Atom::intern("Button"), parent)
    {
        _text = new Text();
    }

    void
    Button::configure(dagbase::ConfigurationElement &config, WidgetFactory &factory, dagui::ShapeFactory &shapeFactory)
    {
        Widget::configure(config, factory, shapeFactory);

        if (auto element=config.findElement("text"); element)
        {
            _text->setText(element->asString());
        }
    }

    dagbase::ConfigurationElement::ValueType Button::find(std::string_view path) const
    {
        auto retval = Widget::find(path);
        if (retval.has_value())
            return retval;

        if (_text)
        {
            retval = dagbase::findInternal(path, "text", _text);
            if (retval.has_value())
                return retval;
        }

        return {};
    }
}
