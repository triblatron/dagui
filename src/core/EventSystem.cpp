#include "config/config.h"

#include "core/EventSystem.h"
#include "core/ConfigurationElement.h"
#include "util/enums.h"
#include "core/EventFilterFactory.h"
#include <string>
#include <cmath>

namespace dagui
{
	void EventFilter::configure(dagbase::ConfigurationElement& config, EventFilterFactory& factory)
	{
		std::string eventTypes;

		dagbase::ConfigurationElement::readConfig(config, "eventTypes", &eventTypes);
		_types = Event::parseTypeMask(eventTypes);

        dagbase::ConfigurationElement::readConfig<ChildType>(config, "childType", &parseChildType, &_childType);

        if (auto element = config.findElement("sequence"); element)
        {
            element->eachChild([this, &factory](dagbase::ConfigurationElement& child) {

                switch (_childType)
                {
                    case CHILD_TIMED_EVENT:
                    {
                        EventTiming timing;

                        timing.configure(child);

                        std::get<CHILD_TIMED_EVENT>(_children).emplace_back(timing);

                        break;
                    }
                    case CHILD_TIMED_EVENT_FILTER:
                    {
                        EventFilterTiming timing;

                        timing.configure(child, factory, _eventSys);

                        _children.emplace<CHILD_TIMED_EVENT_FILTER>().emplace_back(timing);
                        break;
                    }
                }
                return true;
            });
        }
	}

    EventFilter::EventFilter(EventSystem *eventSys)
    :
    _eventSys(eventSys)
    {
        // Do nothing.
    }

    const char *EventFilter::childTypeToString(EventFilter::ChildType value)
    {
        switch (value)
        {
            ENUM_NAME(CHILD_TIMED_EVENT)
            ENUM_NAME(CHILD_TIMED_EVENT_FILTER)
        }

        return "<error>";
    }

    EventFilter::ChildType EventFilter::parseChildType(const char *str)
    {
        TEST_ENUM(CHILD_TIMED_EVENT, str)
        TEST_ENUM(CHILD_TIMED_EVENT_FILTER, str)

        return EventFilter::CHILD_TIMED_EVENT;
    }

    void PassthroughEventFilter::onInput(const Event& event)
	{
        if (_eventSys)
        {
            _eventSys->onOutput(event);
        }
	}

    PassthroughEventFilter::PassthroughEventFilter(EventSystem *eventSys)
    :
    EventFilter(eventSys)
    {
        // Do nothing.
    }

    void EventSystem::configure(dagbase::ConfigurationElement& config)
	{
        EventFilterFactory factory;

		if (auto element = config.findElement("filters"); element)
		{
			element->eachChild([this, &factory](dagbase::ConfigurationElement& child) {

				EventFilter* filter = factory.createEventFilter(child, this);

				if (filter)
				{
                    _filters.emplace_back(filter);
				}

				return true;
            });
		}
	}

	void EventSystem::onInput(const Event& inputEvent)
	{
        for (auto itFilter = _filters.begin(); itFilter != _filters.end(); ++itFilter)
        {
            if ((*itFilter)->types() & (1<<inputEvent.type()))
            {
                (*itFilter)->onInput(inputEvent);
            }
            (*itFilter)->step();
        }
	}

	void EventSystem::onOutput(const Event& event)
	{
		_outputEvents.emplace_back(event);
	}

    void TimedSequenceEventFilter::configure(dagbase::ConfigurationElement &config, EventFilterFactory& factory)
    {
        EventFilter::configure(config, factory);

        if (auto element = config.findElement("output"); element)
        {
            _output.configure(*element);
        }

        dagbase::ConfigurationElement::readConfig(config, "positionRadius", &_positionRadius);
    }

    void TimedSequenceEventFilter::onInput(const Event &inputEvent)
    {
        switch (_childType)
        {
            case EventFilter::CHILD_TIMED_EVENT:
            {
                auto &sequence = std::get<CHILD_TIMED_EVENT>(_children);
                switch (_state)
                {
                    case STATE_INITIAL:
                    {
                        if (_seqIndex < sequence.size() && inputEvent.type() == sequence[_seqIndex].event.type())
                        {
                            _prevEvent = inputEvent;
                            changeState(STATE_EVENT);
                        }
                        break;
                    }
                    case STATE_EVENT:
                    {
                        double d = distanceBetween(_prevEvent.pos(), inputEvent.pos());
                        if (
                                _seqIndex < sequence.size() &&
                                inputEvent.type() == sequence[_seqIndex].event.type() &&
                                (inputEvent.timestamp() - _stateEntryTick) >= sequence[_seqIndex].interval &&
                                d < _positionRadius)
                        {
                            ++_seqIndex;
                            if (_output.data().index() == inputEvent.data().index())
                            {
                                _output.setTimestamp(inputEvent.timestamp());
                                switch (_output.data().index())
                                {
                                    case 0:
                                    {
                                        const auto &inputData = std::get<0>(inputEvent.data());
                                        PointerEvent &outputData = std::get<0>(_output.data());

                                        outputData = inputData;
                                    }
                                }
                            }
                            if (_seqIndex < sequence.size())
                                changeState(STATE_INITIAL);
                            else
                                changeState(STATE_FINAL);
                        }
                    }
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }
    }

    void TimedSequenceEventFilter::changeState(TimedSequenceEventFilter::State nextState)
    {
        if (nextState != _state)
        {
            switch (_state)
            {
                case STATE_EVENT:
                    break;
                default:
                    break;
            }
            _state = nextState;
            switch (_state)
            {
                case STATE_EVENT:
                    ++_seqIndex;
                    break;
                case STATE_FINAL:
                    if (_eventSys)
                    {
                        _eventSys->onOutput(_output);
                    }
                    break;
            }
            if (_eventSys)
                _stateEntryTick = _eventSys->eventTime();
        }
    }

    void TimedSequenceEventFilter::step()
    {
        // Do nothing.
    }

    TimedSequenceEventFilter::TimedSequenceEventFilter(EventSystem *eventSys)
    :
    EventFilter(eventSys)
    {
        // Do nothing.
    }

    double TimedSequenceEventFilter::distanceBetween(const std::int32_t op1[2], const std::int32_t op2[2])
    {
        return std::hypot(op1[0]-op2[0], op1[1]-op2[1]);
    }

    void TimedSequenceEventFilter::EventTiming::configure(dagbase::ConfigurationElement &config)
    {
//        dagbase::ConfigurationElement::readConfig<Event::Type>(config, "type", Event::parseType, &type);
        dagbase::ConfigurationElement::readConfig(config, "event", &event);
        dagbase::ConfigurationElement::readConfig(config, "interval", &interval);
    }

    const char *eventSourceToString(EventSource value)
    {
        switch (value)
        {
            ENUM_NAME(EVENT_SOURCE_UNKNOWN)
            ENUM_NAME(EVENT_SOURCE_POINTING)
            ENUM_NAME(EVENT_SOURCE_KEYBOARD)
        }
        return "<error>";
    }

    EventSource parseEventSource(const char *str)
    {
        TEST_ENUM(EVENT_SOURCE_UNKNOWN, str)
        TEST_ENUM(EVENT_SOURCE_POINTING, str)
        TEST_ENUM(EVENT_SOURCE_KEYBOARD, str)

        return EVENT_SOURCE_UNKNOWN;
    }

    ChordEventFilter::ChordEventFilter(EventSystem *eventSys)
    :
    EventFilter(eventSys)
    {
        // Do nothing.
    }

    void ChordEventFilter::configure(dagbase::ConfigurationElement &config, EventFilterFactory &factory)
    {
        EventFilter::configure(config, factory);
        dagbase::ConfigurationElement::readConfig(config, "output", &_output);
    }

    void ChordEventFilter::onInput(const Event &inputEvent)
    {
        switch (_childType)
        {
            case EventFilter::CHILD_TIMED_EVENT:
            {
                auto& sequence = std::get<CHILD_TIMED_EVENT>(_children);

                std::for_each(sequence.begin(), sequence.end(), [this,&inputEvent](EventTiming& event)
                {
                    if (inputEvent.matches(event.event) )
                    {
                        ++_count;
                    }
                });
                if (_count == sequence.size())
                {
                    if (_eventSys)
                    {
                        _output.setTimestamp(inputEvent.timestamp());
                        _eventSys->onOutput(_output);
                    }
                }
                break;
            }
        }
    }

    void ChordEventFilter::step()
    {
        // Do nothing.
    }

    void EventFilter::EventFilterTiming::configure(dagbase::ConfigurationElement &config, EventFilterFactory& factory, EventSystem* eventSys)
    {
        filter = factory.createEventFilter(config, eventSys);
    }
}
