#include "config/config.h"

#include "core/EventSystem.h"
#include "core/ConfigurationElement.h"

#include <string>

namespace dagui
{
	void EventFilter::configure(dagbase::ConfigurationElement& config)
	{
		std::string eventTypes;

		dagbase::ConfigurationElement::readConfig(config, "eventTypes", &eventTypes);
		_types = Event::parseTypeMask(eventTypes.c_str());
	}

	void PassthroughEventFilter::onInput(const Event& event, EventSystem& eventSys)
	{
		eventSys.onOutput(event);
	}

	void EventSystem::configure(dagbase::ConfigurationElement& config)
	{
		if (auto element = config.findElement("filters"); element)
		{
			element->eachChild([this](dagbase::ConfigurationElement& child) {
				EventFilter* filter = nullptr;

				std::string className;

				dagbase::ConfigurationElement::readConfig(child, "class", &className);

				if (className == "PassthroughEventFilter")
				{
					filter = new PassthroughEventFilter;
				}

				if (filter)
				{
					filter->configure(child);
				}

				_filters.emplace_back(filter);

				return true;
				});
		}
	}

	void EventSystem::onInput(const Event& event)
	{
		_inputEvents.emplace_back(event);
	}

	void EventSystem::onOutput(const Event& event)
	{
		_outputEvents.emplace_back(event);
	}

	void EventSystem::step()
	{
		while (_inputIndex < _inputEvents.size())
		{
			const Event& inputEvent = _inputEvents[_inputIndex];

			for (auto itFilter = _filters.begin(); itFilter != _filters.end(); ++itFilter)
			{
				if ((*itFilter)->types() & (1<<inputEvent.type()))
				{
					(*itFilter)->onInput(inputEvent, *this);
				}
			}
			++_inputIndex;
		}
	}
}