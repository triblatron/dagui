//
// Created by tony on 30/07/24.
//

#include "config/config.h"
#include "core/Widget.h"
#include "core/ConfigurationElement.h"
#include "core/WidgetFactory.h"
#include "util/Searchable.h"

namespace dagui
{
    Widget::Widget(Widget *parent)
    :
    _parent(parent)
    {
        // Do nothing.
    }

    void Widget::configure(dagbase::ConfigurationElement& config, WidgetFactory& factory)
    {
        if (auto element=config.findElement("id"); element)
        {
            _id = element->asString();
        }
    }

    dagbase::ConfigurationElement::ValueType Widget::find(std::string_view path)
    {
        dagbase::ConfigurationElement::ValueType retval;

        retval = dagbase::findEndpoint(path, "id", _id);
        if (retval.has_value())
            return retval;

        return {};
    }
}
