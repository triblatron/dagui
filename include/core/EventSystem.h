#pragma once

#include "config/Export.h"

#include "core/Event.h"

#include <vector>
#include <cstdlib>

namespace dagbase
{
	class ConfigurationElement;
}

namespace dagui
{
	class EventSystem;

	//! A filter taking in an Event and producing another Event
	//! which may be a copy of the original.
	class DAGUI_API EventFilter
	{
	public:
		Event::TypeMask types() const
		{
			return _types;
		}

		virtual void configure(dagbase::ConfigurationElement& config);

		virtual void onInput(const Event& inputEvent, EventSystem& eventSys) = 0;
	private:
		Event::TypeMask _types{ Event::EVENT_NONE };
	};

	class DAGUI_API PassthroughEventFilter : public EventFilter
	{
	public:
		void onInput(const Event& inputEvent, EventSystem& eventSys) override;
	};

	class DAGUI_API EventSystem
	{
	public:
		using EventQueue = std::vector<Event>;
	public:
		const EventQueue& outputEvents() const
		{
			return _outputEvents;
		}

		void onInput(const Event& event);

		void onOutput(const Event& event);

		void configure(dagbase::ConfigurationElement& config);

		void step();
	private:
		EventQueue _inputEvents;
		std::size_t _inputIndex{ 0 };
		EventQueue _outputEvents;
		using FilterList = std::vector<EventFilter*>;
		FilterList _filters;
	};
}