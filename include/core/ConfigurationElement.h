//
// Created by tony on 03/08/24.
//

#pragma once

#include "config/Export.h"

#include <string>
#include <variant>
#include <cstdint>
#include <vector>
#include <optional>

struct lua_State;

namespace nfe
{
    class Lua;

    class NFE_API ConfigurationElement
    {
    public:
        using ValueType = std::optional<std::variant<std::string, std::int64_t, double, bool>>;
    public:
        explicit ConfigurationElement(std::string name);
        ConfigurationElement(std::string name, ValueType::value_type value);
        ConfigurationElement(std::int64_t index, ValueType::value_type value);

        //ConfigurationElement(std::string name, bool value);

        static ConfigurationElement* fromString(const char* str);

        const std::string& name() const
        {
            return _name;
        }

        void addChild(ConfigurationElement* child)
        {
            if (child != nullptr)
            {
                child->setParent(this);
                _children.push_back(child);
            }
        }

        ConfigurationElement* parent()
        {
            return _parent;
        }
        ConfigurationElement* findElement(std::string path);
    private:
        void setParent(ConfigurationElement* parent)
        {
            if (parent!=nullptr)
            {
                _parent = parent;
            }
        }
        ConfigurationElement* findInChildren(std::string path);
        ConfigurationElement* findInArray(size_t index, std::string path);
        ConfigurationElement* _parent{nullptr};
        std::string _name;
        std::int64_t _index{0};
        ValueType _value;
        static ConfigurationElement* buildTree(Lua& lua);
        using Children = std::vector<ConfigurationElement*>;
        Children _children;
    };
}