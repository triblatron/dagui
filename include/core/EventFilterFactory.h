//
// Created by Tony Horrobin on 22/11/2025.
//

#ifndef DAGUI_EVENTFILTERFACTORY_H
#define DAGUI_EVENTFILTERFACTORY_H

#include "config/Export.h"

#include <string_view>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class EventFilter;
    class EventSystem;

    class DAGUI_API EventFilterFactory
    {
    public:
        EventFilter* createEventFilter(dagbase::ConfigurationElement& config, EventSystem* eventSys);
    };
}
#endif //DAGUI_EVENTFILTERFACTORY_H
