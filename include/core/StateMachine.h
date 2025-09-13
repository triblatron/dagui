//
// Created by Tony Horrobin on 10/09/2025.
//

#pragma once

#include "config/Export.h"

#include "core/ConfigurationElement.h"
#include "core/Variant.h"
#include "util/Searchable.h"

#include <set>
#include <cstdint>
#include <vector>
#include <functional>

namespace dagui
{
    template<typename State, typename Transition, typename Input>
    class StateMachine
    {
    public:
    public:
        void configure(dagbase::ConfigurationElement& config)
        {
            if (auto element=config.findElement("states"); element)
            {
                element->eachChild([this](dagbase::ConfigurationElement& child) {
                    State state;

                    state.configure(child);

                    _states.emplace(state);

                    return true;
                });
            }
        }

        void onInput(Input input)
        {

        }

        void step()
        {

        }

        dagbase::Variant find(std::string_view path) const
        {
            dagbase::Variant retval;

            retval = dagbase::findEndpoint(path, "numStates", std::uint32_t(_states.size()));
            if (retval.has_value())
                return retval;

            return {};
        }
    private:
        using StateSet = std::set<State>;
        StateSet _states;
        using TransitionFunction = std::map<std::pair<State,Input>,State>;
        TransitionFunction _transitionFunction;
        using EntryExitActions = std::vector<std::function<void(State)>>;
        EntryExitActions _entryActions;
        EntryExitActions _exitActions;
        using TransitionActions = std::vector<std::function<void(State,State)>>;
        TransitionActions _transitionActions;
    };
}
