//
// Created by Tony Horrobin on 11/10/2026.
//

#pragma once

#include "config/DaguiExport.h"

#include "core/Script.h"
#include "core/Variant.h"

#include <string>
#include <cstdint>
#include <vector>

namespace dagbase
{
    class ConfigurationElement;
    class Coroutine;
    class Lua;
    class ScriptProtocol;
}

namespace dag
{
    class NodeEditorLive;
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
        LuaScript(dagbase::Lua* lua, dagbase::ScriptProtocol* proto);

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
        dagbase::ScriptProtocol* _proto{nullptr};
        State _state{STATE_INITIAL};
        std::string _mainName;
        std::vector<dagbase::Variant> _results;
        void setState(State newState);
    };
}