//
// Created by Tony Horrobin on 16/10/2026.
//

#include "config/config.h"

#include "core/NodeEditorScriptProtocol.h"
#include "core/Scripting.h"
#include "lua_runtime.h"

namespace dagui
{
    NodeEditorScriptProtocol::NodeEditorScriptProtocol(dag::NodeEditorLive *editor)
        :
    _nodeEditor(editor)
    {
        // Do nothing.
    }

    void NodeEditorScriptProtocol::request(dagbase::Node* self, dagbase::Lua& lua, std::uint32_t opCode, const std::pmr::vector<dagbase::Variant> &args,
                                           std::vector<dagbase::Variant> *results)
    {
        switch (opCode)
        {
            case ACTION_ADD_PORT:
            {
                if (lua_gettop(lua.get())>=6)
                {
                    const char* name = lua_tostring(lua.get(), 3);
                    auto dir = dagbase::PortDirection::Direction(lua.integer(4));
                    auto flags = dagbase::Node::NodeFlags(lua.integer(5));
                    auto value = lua.number(6);
                    if (_nodeEditor)
                    {
                        auto status = _nodeEditor->addPort(self->id(), name, dir, dagbase::Port::FLAGS_NONE, dagbase::Value(value));
                        auto port = _nodeEditor->activeGraph()->port(std::get<dagbase::PortID>(status.result.value()));
                        auto portType = lua.typeQuery("_p_dagbase__Port");
                        // Push results of yield
                        if (portType)
                        {
                            lua.pushObj(port, portType, 0);
                        }
                    }
                    //auto port = new dagbase::Port(nodeLib.nextPortID(), name, dir, dagbase::Port::FLAGS_NONE, dagbase::Value(value));
                    //self->addDynamicPort(port, dagbase::MetaPort::FLAGS_OWN_BIT);
                    // Pop results of resume
                    lua_pop(lua.get(), 6);
                }
                break;
            }
        }

    }
}
