//
// Created by Tony Horrobin on 06/04/2025.
//

#include "config/config.h"

#include "core/WidgetFactory.h"
#include "core/ConfigurationElement.h"
#include "core/Widget.h"

namespace dagui
{
    Widget* WidgetFactory::create(dagbase::ConfigurationElement& config)
    {
        return new Widget();
    }
}