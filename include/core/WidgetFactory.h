//
// Created by Tony Horrobin on 06/04/2025.
//

#pragma once

#include "config/Export.h"

#include <stack>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class RootWidget;
    class Shape;
    class ShapeFactory;
    class Widget;

    class DAGUI_API WidgetFactory
    {
    public:
        Widget *create(dagbase::ConfigurationElement &config, ShapeFactory &shapeFactory);

        void setFreeType(FT_Library freeTypeLib)
        {
            _freeTypeLib = freeTypeLib;
        }
    private:
        RootWidget* _root{nullptr};
        using ParentStack = std::stack<Widget*>;
        ParentStack _parentStack;
        FT_Library _freeTypeLib{nullptr};
    };
}