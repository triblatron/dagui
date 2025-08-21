//
// Created by Tony Horrobin on 06/04/2025.
//

#include "config/config.h"

#include "core/WidgetFactory.h"

#include "core/Button.h"
#include "core/ConfigurationElement.h"
#include "core/Window.h"
#include "core/RootWidget.h"
#include "core/Vertical.h"
#include "core/Label.h"
#include "core/Rectangle.h"

namespace dagui
{
    Widget * WidgetFactory::create(dagbase::ConfigurationElement &config, ShapeFactory &shapeFactory)
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
            _root = dynamic_cast<RootWidget*>(widget);
            _root->setFreeTypeLib(_freeTypeLib);
        }
        else if (className == "Window")
        {
            widget = new Window(!_parentStack.empty()?_parentStack.top():nullptr);
        }
        else if (className == "Button")
        {
            widget = new Button(!_parentStack.empty()?_parentStack.top():nullptr);
        }
        else if (className == "Vertical")
        {
            widget = new Vertical(!_parentStack.empty()?_parentStack.top():nullptr);
        }
        else if (className == "Label")
        {
            widget = new Label(!_parentStack.empty()?_parentStack.top():nullptr);
        }

        _parentStack.push(widget);

        if (widget)
        {
            widget->configure(config, *this, shapeFactory);
        }

        _parentStack.pop();

        return widget;
    }
}
