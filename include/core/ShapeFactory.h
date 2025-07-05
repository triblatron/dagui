//
// Created by Tony Horrobin on 05/07/2025.
//

#pragma once

#include "config/Export.h"

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class Shape;

    class DAGUI_API ShapeFactory
    {
    public:
        Shape* createShape(dagbase::ConfigurationElement& config);
    };
}