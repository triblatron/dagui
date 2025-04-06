//
// Created by tony on 30/07/24.
//

#include "config/config.h"
#include "core/Widget.h"


namespace dagui
{
    Widget::Widget(Widget *parent)
    :
    _parent(parent)
    {
        // Do nothing.
    }

    dagbase::ConfigurationElement::ValueType Widget::find(std::string_view path)
    {
        return {};
    }
}
