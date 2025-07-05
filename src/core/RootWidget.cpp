//
// Created by Tony Horrobin on 10/04/2025.
//

#include "config/config.h"

#include "core/RootWidget.h"
#include "core/Style.h"

namespace dagui
{
    void
    RootWidget::configure(dagbase::ConfigurationElement &config, WidgetFactory &factory, ShapeFactory &shapeFactory)
    {
        Widget::configure(config, factory, shapeFactory);
        if (auto element = config.findElement("size"); element)
        {
            dagbase::ConfigurationElement::readConfig(*element, "width", &_size.x);
            dagbase::ConfigurationElement::readConfig(*element, "height", &_size.y);
        }

        if (auto element = config.findElement("styles"); element)
        {
            element->eachChild([this](dagbase::ConfigurationElement& child) {
                auto style = new Style();
                std::string name;
                dagbase::ConfigurationElement::readConfig(child, "name", &name);
                style->configure(child);
                addStyle(dagbase::Atom::intern(name), style);
                return true;
            });
        }
    }

    void RootWidget::addIdentified(Widget* widget)
    {
        if (widget)
        {
            _widgetLookup.m.insert(WidgetLookup::value_type(widget->id(), widget));
        }
    }

    Widget* RootWidget::lookup(dagbase::Atom name)
    {
        if (auto it=_widgetLookup.m.find(name); it!=_widgetLookup.m.end())
        {
            return it->second;
        }

        return nullptr;
    }

    dagbase::ConfigurationElement::ValueType RootWidget::find(std::string_view path) const
    {
        auto retval = Widget::find(path);

        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "width", std::int64_t(_size.x));
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "height", std::int64_t(_size.y));
        if (retval.has_value())
            return retval;

        retval = dagbase::findInternal(path, "lookup", _widgetLookup);
        if (retval.has_value())
            return retval;

        retval = dagbase::findInternal(path, "styles", _styles);
        if (retval.has_value())
            return retval;

        return {};
    }

    RootWidget::RootWidget()
    :
    Widget(dagbase::Atom::intern("RootWidget"))
    {
        // Do nothing.
    }
}
