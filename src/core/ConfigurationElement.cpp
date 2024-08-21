//
// Created by tony on 03/08/24.
//

#include "config/config.h"

#include "core/ConfigurationElement.h"
#include "core/LuaInterface.h"

#include <stack>
#include <cstdlib>

namespace nfe
{
    ConfigurationElement::ConfigurationElement(std::string name)
    :
    _name(std::move(name))
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

    ConfigurationElement* ConfigurationElement::findInArray(size_t index, std::string path)
    {
        for (; index<path.length() && isdigit(path[index]); ++index);
        char* end = nullptr;
        size_t childIndex = std::strtoul(path.c_str(), &end, 10);
        if (childIndex<_children.size() && index<path.size() && path[index] == ']')
        {
            ++index;
            if (index < path.length()-1)
            {
                path = path.substr(index+1);
                return findInChildren(path);
            }
            else
            {
                return _children[childIndex];
            }
        }
        return nullptr;
    }

    ConfigurationElement* ConfigurationElement::findElement(std::string path)
    {
        if (path[0] == '$')
        {
            auto root = this;
            while (root != nullptr && root->parent() != nullptr)
            {
                root = root->parent();
            }

            if (root != nullptr && path.find("$.")!=std::string::npos)
            {
                path = path.substr(2);

                return root->findInChildren(path);
            }
            else if (root != nullptr && path.length()>=4 && path[1] == '[')
            {
                path = path.substr(2);
                return findInArray(0, path);
            }
            else if (path == "$")
            {
                return root;
            }
        }
        else
        {
            return findInChildren(path);
        }
        return nullptr;
    }

    ConfigurationElement *ConfigurationElement::findInChildren(std::string path)
    {
        if (path.empty())
        {
            return this;
        }

        auto dotPos = path.find('.');
        // Find position of subscript.
        auto subPos = path.find('[');
        if (subPos!=std::string::npos && subPos < dotPos)
        {
            if (subPos == 0)
            {
                return findInArray(1, path);
            }
            else
            {
                size_t index=0;
                for (index=0; index<path.length() && path[index] != ']';++index);
                if (index<path.length() && path[index]==']')
                {
                    std::string first = path.substr(0,subPos);
                    std::string rest = path.substr(subPos);

                    for (auto child : _children)
                    {
                        if (child->name() == first)
                        {
                            return child->findInChildren(rest);
                        }

                    }
                }
            }
        }
        else if (dotPos != std::string::npos)
        {
            if (dotPos == 0 && path.length()>1)
            {
                size_t index = 0;
                for (index=0; index < path.length() && path[index] != '.' && path[index] != '['; ++index);
                std::string first = path.substr(1, dotPos);
                std::string rest = path.substr(dotPos, index-dotPos);

                for (auto child : _children)
                {
                    if (child->_name == first)
                    {
                        return child->findInChildren(rest);
                    }
                }
            }
            else
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

                while (parentStack.size() - 1 > level)
                {
                    parentStack.pop();
                }

                if (lua_isinteger(lua, -2))
                {
                    std::int64_t index = lua_tointeger(lua, -2);
                    if (lua_isinteger(lua, -1))
                    {
                        child = new ConfigurationElement(index, lua_tointeger(lua, -1));
                        parentStack.top()->addChild(child);
                    }
                    else if (lua_isnumber(lua, -1))
                    {
                        child = new ConfigurationElement(index, lua_tonumber(lua, -1));
                        parentStack.top()->addChild(child);
                    }
                    else if (lua_isboolean(lua, -1))
                    {
                        child = new ConfigurationElement(index, bool(lua_toboolean(lua, -1)));
                        parentStack.top()->addChild(child);
                    }
                    else if (lua_isstring(lua, -1))
                    {
                        child = new ConfigurationElement(index, std::string(lua_tostring(lua, -1)));
                        parentStack.top()->addChild(child);
                    }
                }
                else if (lua_isstring(lua, -2))
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
                        child = new ConfigurationElement(name, bool(lua_toboolean(lua, -1)));
                        parentStack.top()->addChild(child);
                    }
                    else if (lua_isstring(lua, -1))
                    {
                        child = new ConfigurationElement(name, std::string(lua_tostring(lua, -1)));
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

    ConfigurationElement::~ConfigurationElement()
    {
        for (auto child : _children)
        {
            delete child;
        }
    }
}
