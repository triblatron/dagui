//
// Created by Tony Horrobin on 06/04/2025.
//

#pragma once

#include "config/Export.h"

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class Widget;

    class DAGUI_API WidgetFactory
    {
    public:
        Widget* create(dagbase::ConfigurationElement& config);
    };
}