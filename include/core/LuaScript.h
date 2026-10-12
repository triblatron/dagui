//
// Created by Tony Horrobin on 11/10/2026.
//

#pragma once

#include "config/Export.h"

#include "core/Script.h"

#include <string>
#include <cstdint>

namespace dagbase
{
    class ConfigurationElement;
    class Coroutine;
    class Lua;
}

namespace dagui
{

    class DAGUI_API LuaScript : public dagbase::Script
    {
    public:
        enum State : std::uint32_t
        {
            STATE_INITIAL,
            STATE_THREAD_INITED,
            STATE_THREAD_FINISHED
        };
    public:
        LuaScript(dagbase::Lua* lua);

        void configure(dagbase::ConfigurationElement& config);

        void setMainName(const std::string& name)
        {
            _mainName = name;
        }

        void step(dagbase::Node* self) override;

        bool isFinished() const override
        {
            return _state == STATE_THREAD_FINISHED;
        }

        void reset() override;

        static const char* stateName(State state);
    private:
        dagbase::Lua* _lua{nullptr};
        dagbase::Coroutine* _thread{nullptr};
        State _state{STATE_INITIAL};
        std::string _mainName;
        void setState(State newState);
    };
}