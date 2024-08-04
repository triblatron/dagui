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
        enum Type
        {
            TYPE_BOOL,
            TYPE_INTEGER,
            TYPE_DOUBLE,
            TYPE_STRING
        };
        using ValueType = std::optional<std::variant<bool, std::int64_t, double, std::string>>;
    public:
        explicit ConfigurationElement(std::string name);

        ConfigurationElement(std::string name, ValueType::value_type value)
        :
        _name(name),
        _value(value)
        {
            // Do nothing.
        }

        ConfigurationElement(std::int64_t index, ValueType::value_type value)
        :
        _index(index),
        _value(value)
        {
            // Do nothing.
        }

        ~ConfigurationElement();

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

        std::int64_t asInteger(int64_t defaultValue=0) const
        {
            if (_value.has_value() && _value->index() == TYPE_INTEGER)
            {
                return std::get<TYPE_INTEGER>(_value.value());
            }
            else
            {
                return defaultValue;
            }
        }

        double asDouble(double defaultValue=0.0) const
        {
            if (_value.has_value() && _value->index()==TYPE_DOUBLE)
            {
                return std::get<TYPE_DOUBLE>(_value.value());
            }
            else
            {
                return defaultValue;
            }
        }

        bool asBool(bool defaultValue=false) const
        {
            if (_value.has_value() && _value->index()==TYPE_BOOL)
            {
                return std::get<TYPE_BOOL>(_value.value());
            }
            else
            {
                return defaultValue;
            }
        }

        std::string asString(std::string defaultValue="") const
        {
            if (_value.has_value() && _value->index()==TYPE_STRING)
            {
                return std::get<TYPE_STRING>(_value.value());
            }
            else
            {
                return defaultValue;
            }
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