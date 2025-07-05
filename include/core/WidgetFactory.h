//
// Created by Tony Horrobin on 06/04/2025.
//

#pragma once

#include "config/Export.h"

#include <stack>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class Shape;
    class ShapeFactory;
    class Widget;

    class DAGUI_API WidgetFactory
    {
    public:
        Widget *create(dagbase::ConfigurationElement &config, ShapeFactory &shapeFactory);

        //Shape* createShape(dagbase::ConfigurationElement& config);
    private:
        Widget* _root{nullptr};
        using ParentStack = std::stack<Widget*>;
        ParentStack _parentStack;
    };
}