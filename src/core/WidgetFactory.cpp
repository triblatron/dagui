//
// Created by Tony Horrobin on 06/04/2025.
//

#include "config/config.h"

#include "core/WidgetFactory.h"

#include "core/Button.h"
#include "core/ConfigurationElement.h"
#include "core/Window.h"
#include "core/RootWidget.h"

namespace dagui
{
    Widget* WidgetFactory::create(dagbase::ConfigurationElement& config)
    {
        std::string className;

        if (auto element = config.findElement("class"); element)
        {
            className = element->asString();
        }

        Widget* widget = nullptr;

        if (className == "RootWidget")
        {
            widget = new RootWidget();
            _root = widget;
        }
        else if (className == "Window")
        {
            widget = new Window(!_parentStack.empty()?_parentStack.top():nullptr);
        }
        else if (className == "Button")
        {
            widget = new Button(!_parentStack.empty()?_parentStack.top():nullptr);
        }

        _parentStack.push(widget);

        if (widget)
        {
            widget->configure(config, *this);
        }

        _parentStack.pop();

        return widget;
    }
}