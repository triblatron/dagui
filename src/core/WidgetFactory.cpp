//
// Created by Tony Horrobin on 06/04/2025.
//

#include "config/config.h"

#include "core/WidgetFactory.h"
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
        }
        else if (className == "Window")
        {
            widget = new Window(nullptr);
        }

        if (widget)
        {
            widget->configure(config, *this);
        }

        return widget;
    }
}