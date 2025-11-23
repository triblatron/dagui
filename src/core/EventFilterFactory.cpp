//
// Created by Tony Horrobin on 22/11/2025.
//

#include "config/config.h"

#include "core/EventFilterFactory.h"
#include "core/EventSystem.h"
#include "core/ConfigurationElement.h"

namespace dagui
{
    EventFilter *EventFilterFactory::createEventFilter(dagbase::ConfigurationElement& config, EventSystem* eventSys)
    {
        std::string className;

        dagbase::ConfigurationElement::readConfig(config, "class", &className);
        EventFilter* eventFilter = nullptr;
        if (className == "PassthroughEventFilter")
        {
            eventFilter = new PassthroughEventFilter(eventSys);
        }
        else if (className == "TimedSequenceEventFilter")
        {
            eventFilter = new TimedSequenceEventFilter(eventSys);
        }
        else if (className == "ChordEventFilter")
        {
            eventFilter = new ChordEventFilter(eventSys);
        }

        if (eventFilter)
        {
            eventFilter->configure(config, *this);
        }
        return eventFilter;
    }
}