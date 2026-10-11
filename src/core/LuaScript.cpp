//
// Created by Tony Horrobin on 11/10/2026.
//

#include "config/config.h"

#include "core/LuaScript.h"
#include "core/ConfigurationElement.h"
#include "core/LuaInterface.h"
#include "core/Scripting.h"
#include "lua_runtime.h"
#include "util/enums.h"

#include <string>

#include "MemoryNodeLibrary.h"
#include "core/Node.h"
#include "core/PortDirection.h"

namespace dagui
{
    LuaScript::LuaScript(dagbase::Lua *lua)
        :
    _lua(lua)
    {
        // Do nothing.
    }

    void LuaScript::configure(dagbase::ConfigurationElement &config)
    {
        dagbase::ConfigurationElement::readConfig(config, "main", &_mainName);
    }

    void LuaScript::step(dagbase::Node *self)
    {
        State prevState = _state;
        do
        {
            prevState = _state;
            switch (_state)
            {
                case STATE_INITIAL:
                    if (!_mainName.empty())
                    {
                        _thread = dagbase::Coroutine::fromNamedFunction(_lua->get(), _mainName.c_str());
                    }
                    setState(STATE_THREAD_INITED);
                    break;
                case STATE_THREAD_INITED:
                {
                    if (_thread)
                    {
                        auto type = SWIG_TypeQuery(_thread->get(), "_p_dagbase__Node");
                        if (type)
                        {
                            SWIG_NewPointerObj(_thread->get(), self, type, 0);
                        }
                        int numResults = 0;
                        auto reason = _thread->resume(1, &numResults);
                        switch (reason)
                        {
                            case LUA_OK:
                                setState(STATE_THREAD_FINISHED);
                                break;
                            case LUA_YIELD:
                            {
                                int nspace = _thread->integer(1);
                                int action = _thread->integer(2);
                                switch (action)
                                {
                                    case ACTION_ADD_PORT:
                                    {
                                        const char* name = lua_tostring(_thread->get(), 3);
                                        auto dir = dagbase::PortDirection::Direction(_thread->integer(4));
                                        auto flags = dagbase::Node::NodeFlags(_thread->integer(5));
                                        auto value = _thread->number(6);//_thread->get(), 4));
                                        dag::MemoryNodeLibrary nodeLib;
                                        self->addDynamicPort(new dagbase::Port(nodeLib.nextPortID(), name, dir, dagbase::Port::FLAGS_NONE, dagbase::Value(value)), dagbase::MetaPort::FLAGS_OWN_BIT);
                                        lua_pop(_thread->get(), 4);
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                    }

                    break;
                }
            }
        } while (_state!=prevState);
    }

    const char * LuaScript::stateName(State state)
    {
        switch (state)
        {
            ENUM_NAME(STATE_INITIAL)
            ENUM_NAME(STATE_THREAD_INITED)
            ENUM_NAME(STATE_THREAD_FINISHED)
        }

        return "<error>";
    }

    void LuaScript::setState(State newState)
    {
        std::cout << "LuaScript:" << this << ":Changing state from " << stateName(_state) << " to " << stateName(newState) << std::endl;
        _state = newState;
    }
}
