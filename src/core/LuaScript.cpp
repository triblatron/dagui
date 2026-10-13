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
#include "NodeEditorLive.h"
#include "core/Node.h"
#include "core/PortDirection.h"
#include "core/ScriptProtocol.h"

namespace dagui
{
    LuaScript::LuaScript(dagbase::Lua *lua, dag::NodeEditorLive* nodeEditor, dagbase::ScriptProtocol* proto)
        :
    _lua(lua),
    _nodeEditor(nodeEditor),
    _proto(proto)
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
                        if (!_thread)
                            _thread = dagbase::Coroutine::fromNamedFunction(_lua->get(), _mainName.c_str());
                        else
                        {
                            _thread->setMainFunction(_mainName.c_str());
                        }
                        if (_thread)
                        {
                            auto type = SWIG_TypeQuery(_thread->get(), "_p_dagbase__Node");
                            if (type)
                            {
                                SWIG_NewPointerObj(_thread->get(), self, type, 0);
                            }
                            setState(STATE_THREAD_INITED);
                        }
                    }
                    break;
                case STATE_THREAD_INITED:
                {
                    if (_thread)
                    {
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
                                if (_proto)
                                {
                                    std::array<std::byte, sizeof(dagbase::Variant)*4> buffer;
                                    std::pmr::monotonic_buffer_resource pool{buffer.data(), buffer.size()};
                                    std::pmr::vector<dagbase::Variant> args{&pool};
                                    // Copy args into buffer
                                    for (int i=3; i<=_thread->top(); ++i)
                                    {
                                        dagbase::Variant arg;
                                        arg.fromLua(*_thread, i);
                                    }
                                    _proto->request(action, args);
                                }
                                switch (action)
                                {
                                    case ACTION_ADD_PORT:
                                    {
                                        if (lua_gettop(_thread->get())>=6)
                                        {
                                            const char* name = lua_tostring(_thread->get(), 3);
                                            auto dir = dagbase::PortDirection::Direction(_thread->integer(4));
                                            auto flags = dagbase::Node::NodeFlags(_thread->integer(5));
                                            auto value = _thread->number(6);
                                            if (_nodeEditor)
                                            {
                                                auto status = _nodeEditor->addPort(self->id(), name, dir, dagbase::Port::FLAGS_NONE, dagbase::Value(value));
                                                auto port = _nodeEditor->activeGraph()->port(std::get<dagbase::PortID>(status.result.value()));
                                                auto portType = SWIG_TypeQuery(_thread->get(), "_p_dagbase__Port");
                                                // Push results of yield
                                                if (portType)
                                                {
                                                    SWIG_NewPointerObj(_thread->get(), port, portType, 0);
                                                }
                                            }
                                            //auto port = new dagbase::Port(nodeLib.nextPortID(), name, dir, dagbase::Port::FLAGS_NONE, dagbase::Value(value));
                                            //self->addDynamicPort(port, dagbase::MetaPort::FLAGS_OWN_BIT);
                                            // Pop results of resume
                                            lua_pop(_thread->get(), 6);
                                        }
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

    void LuaScript::reset()
    {
        if (_thread)
        {
            _thread->reset();
            setState(STATE_INITIAL);
        }
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
