//
// Created by Tony Horrobin on 17/04/2025.
//

#include "config/config.h"

#include "core/Label.h"
#include "util/Searchable.h"

namespace dagui
{

    Label::Label(Widget *parent)
    :
    Widget(dagbase::Atom::intern("Label"), parent)
    {
        // Do nothing.
    }

    void Label::configure(dagbase::ConfigurationElement &config, WidgetFactory& factory)
    {
        Widget::configure(config, factory);

        if (auto element=config.findElement("text"); element)
        {
            _text = element->asString();
        }
    }

    dagbase::ConfigurationElement::ValueType Label::find(std::string_view path) const
    {
        auto retval = Widget::find(path);
        if (retval.has_value())
        {
            return retval;
        }

        retval = dagbase::findEndpoint(path, "text", _text);
        if (retval.has_value())
            return retval;

        return {};
    }
}
