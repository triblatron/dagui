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
#include "MemoryNodeLibrary.h"
#include "NodeEditorLive.h"
#include "core/Node.h"
#include "core/PortDirection.h"
#include "core/ScriptProtocol.h"
#include "util/Searchable.h"

#include <string>
#include <array>

namespace dagui
{
    LuaScript::LuaScript(dagbase::Lua *lua, dagbase::ScriptProtocol* proto)
        :
    _lua(lua),
    _proto(proto)
    {
        // There will typically be one result, so pre-allocate it here.
        _results.reserve(1);
    }

    LuaScript::~LuaScript()
    {
        delete _thread;
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
                        {
                            _thread = dagbase::Coroutine::fromNamedFunction(_lua->get(), _mainName.c_str());
                            if (!_thread)
                            {
                                setState(STATE_THREAD_ERROR);
                            }
                        }
                        else
                        {
                            _thread->setMainFunction(_mainName.c_str());
                            if (!_thread->ok())
                            {
                                setState(STATE_THREAD_ERROR);
                            }
                        }
                        if (_thread && _thread->ok())
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
                                        args.emplace_back(arg);
                                    }
                                    _results.clear();
                                    _proto->request(self, action, args, &_results);
                                    for (auto result : _results)
                                    {
                                        result.toLua(*_thread);
                                    }
                                    lua_pop(_thread->get(), numResults);
                                }
                                break;
                            }
                            default:
                            {
                                setState(STATE_THREAD_ERROR);
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

    dagbase::Variant LuaScript::find(std::string_view path) const
    {
        dagbase::Variant retval;

        retval = dagbase::findEndpoint(path, "state", stateName(_state));
        if (retval.has_value())
            return retval;

        return {};
    }

    const char * LuaScript::stateName(State state)
    {
        switch (state)
        {
            ENUM_NAME(STATE_UNKNOWN)
            ENUM_NAME(STATE_INITIAL)
            ENUM_NAME(STATE_THREAD_INITED)
            ENUM_NAME(STATE_THREAD_FINISHED)
            ENUM_NAME(STATE_THREAD_ERROR)
        }

        return "<error>";
    }

    LuaScript::State LuaScript::parseState(const char *str)
    {
        TEST_ENUM(STATE_UNKNOWN, str)
        TEST_ENUM(STATE_INITIAL, str)
        TEST_ENUM(STATE_THREAD_INITED, str)
        TEST_ENUM(STATE_THREAD_FINISHED, str)
        TEST_ENUM(STATE_THREAD_ERROR, str)

        return STATE_UNKNOWN;
    }

    void LuaScript::setState(State newState)
    {
        std::cout << "LuaScript:" << this << ":Changing state from " << stateName(_state) << " to " << stateName(newState) << std::endl;
        _state = newState;
        switch (_state)
        {
            case STATE_INITIAL:
            {
                if (_thread)
                {
                    _thread->clearError();
                }
                break;
            }
            case STATE_THREAD_ERROR:
            {
                std::cerr << "LuaScript:A Lua error occurred executing the script:" << _mainName << ':' << _thread->errorMessage() << '\n';
                break;
            }
            default:
                break;
        }
    }
}
