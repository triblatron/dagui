#pragma once

#include "config/Export.h"

#include "core/Event.h"
#include "util/TimeProvider.h"

#include <vector>
#include <cstdlib>
#include <chrono>

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
        explicit EventFilter(EventSystem* eventSys);

        ~EventFilter() = default;

		Event::TypeMask types() const
		{
			return _types;
		}

		virtual void configure(dagbase::ConfigurationElement& config);

		virtual void onInput(const Event& inputEvent) = 0;

        virtual void step()
        {
            // Do nothing.
        }
    protected:
        EventSystem* _eventSys{nullptr};
	private:
		Event::TypeMask _types{ Event::EVENT_NONE };
	};

	class DAGUI_API PassthroughEventFilter : public EventFilter
	{
	public:
        explicit PassthroughEventFilter(EventSystem* eventSys);

		void onInput(const Event& inputEvent) override;
	};

    class DAGUI_API TimedSequenceEventFilter : public EventFilter
    {
    public:
        enum State : std::uint32_t
        {
            STATE_INITIAL,
            STATE_EVENT,
            STATE_FINAL
        };
    public:
        explicit TimedSequenceEventFilter(EventSystem* eventSys);

        void configure(dagbase::ConfigurationElement& config) override;

        void onInput(const Event& inputEvent) override;

        void step() override;
    private:
        struct EventTiming
        {
            Event::Type type{Event::TYPE_UNKNOWN};
            double interval{0.0};

            void configure(dagbase::ConfigurationElement& config);
        };
        Event _output;
        using TimingSequence = std::vector<EventTiming>;
        TimingSequence _sequence;
        std::size_t _seqIndex{0};
        State _state{STATE_INITIAL};
        double _stateEntryTick{};
        void changeState(State nextState);
    };

	class DAGUI_API EventSystem
	{
	public:
		using EventQueue = std::vector<Event>;
	public:
        void setTimeProvider(dagbase::TimeProvider* timeProvider)
        {
            _timeProvider = timeProvider;
        }

        double eventTime() const
        {
            if (_timeProvider)
                return _timeProvider->provideTime();
            else
                return 0.0;
        }

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
        EventQueue _outputEvents;
        using FilterList = std::vector<EventFilter*>;
        FilterList _filters;
        dagbase::TimeProvider* _timeProvider{nullptr};
		std::size_t _inputIndex{ 0 };
	};
}