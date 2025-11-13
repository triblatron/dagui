#include "config/config.h"

#include "core/EventSystem.h"
#include "core/ConfigurationElement.h"

#include <string>
#include <cmath>

namespace dagui
{
	void EventFilter::configure(dagbase::ConfigurationElement& config)
	{
		std::string eventTypes;

		dagbase::ConfigurationElement::readConfig(config, "eventTypes", &eventTypes);
		_types = Event::parseTypeMask(eventTypes);
	}

    EventFilter::EventFilter(EventSystem *eventSys)
    :
    _eventSys(eventSys)
    {
        // Do nothing.
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
		if (auto element = config.findElement("filters"); element)
		{
			element->eachChild([this](dagbase::ConfigurationElement& child) {
				EventFilter* filter = nullptr;

				std::string className;

				dagbase::ConfigurationElement::readConfig(child, "class", &className);

				if (className == "PassthroughEventFilter")
				{
					filter = new PassthroughEventFilter(this);
				}
                else if (className == "TimedSequenceEventFilter")
                {
                    filter = new TimedSequenceEventFilter(this);
                }

				if (filter)
				{
					filter->configure(child);
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

    void TimedSequenceEventFilter::configure(dagbase::ConfigurationElement &config)
    {
        EventFilter::configure(config);

        if (auto element = config.findElement("sequence"); element)
        {
            element->eachChild([this](dagbase::ConfigurationElement& child) {
                EventTiming timing;

                timing.configure(child);

                _sequence.emplace_back(timing);

                return true;
            });
        }

        if (auto element = config.findElement("output"); element)
        {
            _output.configure(*element);
        }

        dagbase::ConfigurationElement::readConfig(config, "positionRadius", &_positionRadius);
    }

    void TimedSequenceEventFilter::onInput(const Event &inputEvent)
    {
        switch (_state)
        {
            case STATE_INITIAL:
                if (_seqIndex<_sequence.size() && inputEvent.type() == _sequence[_seqIndex].type)
                {
                    _prevEvent = inputEvent;
                    changeState(STATE_EVENT);
                }
                break;
            case STATE_EVENT:
            {
                double d = distanceBetween(_prevEvent.pos(), inputEvent.pos());
                if (
                        _seqIndex < _sequence.size() &&
                        inputEvent.type() == _sequence[_seqIndex].type &&
                        (inputEvent.timestamp() - _stateEntryTick) >= _sequence[_seqIndex].interval &&
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
                                const auto& inputData = std::get<0>(inputEvent.data());
                                PointerEvent& outputData = std::get<0>(_output.data());

                                outputData = inputData;
                            }
                        }
                    }
                    if (_seqIndex<_sequence.size())
                        changeState(STATE_INITIAL);
                    else
                        changeState(STATE_FINAL);
                }
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
        dagbase::ConfigurationElement::readConfig<Event::Type>(config, "type", Event::parseType, &type);
        dagbase::ConfigurationElement::readConfig(config, "interval", &interval);
    }
}
