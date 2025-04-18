//
// Created by Tony Horrobin on 17/04/2025.
//

#include "config/config.h"

#include "core/Vertical.h"

namespace dagui
{
    void Vertical::configure(dagbase::ConfigurationElement &config, WidgetFactory& factory)
    {
        Widget::configure(config, factory);
    }

    dagbase::ConfigurationElement::ValueType Vertical::find(std::string_view path) const
    {
        return Widget::find(path);
    }

    Vertical::Vertical(Widget *parent) : Widget(dagbase::Atom::intern("Vertical"), parent)
    {
        // Do nothing.
    }
}
