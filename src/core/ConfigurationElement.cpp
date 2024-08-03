//
// Created by tony on 03/08/24.
//

#include "config/config.h"

#include "core/ConfigurationElement.h"
#include "core/LuaInterface.h"

#include <stack>

namespace nfe
{
    ConfigurationElement::ConfigurationElement(std::string name)
            :
            _name(name)
    {
        // Do nothing.
    }

    ConfigurationElement::ConfigurationElement(std::string name, bool value)
            :
            _name(name),
            _value(value)
    {
        // Do nothing.
    }

    ConfigurationElement *ConfigurationElement::fromString(const char *str)
    {
        nfe::Lua lua;

        if (str != nullptr)
        {
            lua.eval(str);
        }

        return buildTree(lua);
    }

    ConfigurationElement *ConfigurationElement::findElement(std::string path)
    {
        if (path.find("$.") == 0)
        {
            auto root = this;
            while (root->parent() != nullptr)
            {
                root = root->parent();
            }

            path = path.substr(2);

            return root->findInChildren(path);
        }
        else
        {
            return findInChildren(path);
        }
        return nullptr;
    }

    ConfigurationElement *ConfigurationElement::buildTree(Lua& lua)
    {
        using ParentStack = std::stack<ConfigurationElement*>;
        ParentStack parentStack;
        auto parent = new ConfigurationElement("root");
        parentStack.push(parent);

        if (lua.tableExists("root"))
        {
            auto rootTable = lua.tableForName("root");

            TableTraversal trav(lua.get());

            trav([&parentStack](lua_State* lua, size_t level) {
                ConfigurationElement* child = nullptr;
                if (lua_isstring(lua, -2))
                {
                    std::string name = lua_tostring(lua, -2);
                    if (lua_isinteger(lua, -1))
                    {
                        child = new ConfigurationElement(name, lua_tointeger(lua, -1));
                        parentStack.top()->addChild(child);
                    }
                    else if (lua_isnumber(lua, -1))
                    {
                        child = new ConfigurationElement(name, lua_tonumber(lua, -1));
                        parentStack.top()->addChild(child);
                    }
                    else if (lua_isboolean(lua, -1))
                    {
                        child = new ConfigurationElement(name, lua_toboolean(lua, -1));
                        parentStack.top()->addChild(child);
                    }
                    else if (lua_isstring(lua, -1))
                    {
                        child = new ConfigurationElement(name, lua_tostring(lua, -1));
                        parentStack.top()->addChild(child);
                    }
                    else if (lua_istable(lua, -1))
                    {
                        child = new ConfigurationElement(name);
                        parentStack.top()->addChild(child);
                        parentStack.push(child);
                    }
                }
                return 0;
            });
        }
        return parent;
    }

    ConfigurationElement *ConfigurationElement::findInChildren(std::string path)
    {
        auto dotPos = path.find('.');
        if (dotPos != std::string::npos)
        {
            std::string first = path.substr(0,dotPos);
            if (path.length()>dotPos)
            {
                std::string rest = path.substr(dotPos+1);
                for (auto child : _children)
                {
                    if (child->_name == first)
                    {
                        return child->findInChildren(rest);
                    }
                }
            }
        }
        else
        {
            for (auto child : _children)
            {
                if (child->_name == path)
                {
                    return child;
                }
            }
        }

        return nullptr;
    }
}
