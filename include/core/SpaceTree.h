//
// Created by tony on 02/10/24.
//

#pragma once

#include "config/Export.h"

#include <functional>
#include <array>
#include <string_view>

#include "ConfigurationElement.h"

namespace nfe
{
    class ConfigurationElement;
    class SpaceTree;

    class NFE_API Children
    {
    public:
        using ChildArray = std::array<SpaceTree*,2>;
    public:
        ChildArray::iterator begin()
        {
            return _a.begin();
        }

        ChildArray::iterator end()
        {
            return _a.end();
        }

        std::size_t size() const
        {
            return _nextIndex;
        }

        void addChild(SpaceTree* child)
        {
            if (child)
            {
                if (_nextIndex < _a.max_size())
                {
                    _a[_nextIndex++] = child;
                }
            }
        }

        SpaceTree* operator[](std::size_t index)
        {
            return _a[index];
        }

        ConfigurationElement::ValueType find(std::string_view) const;
    private:
        ChildArray _a{nullptr};
        std::size_t _nextIndex{0};
    };

    class NFE_API SpaceTree
    {
    public:
        enum Result
        {
            RESULT_OK,
            RESULT_UNKNOWN,
            RESULT_FAILED_TO_INSERT,
            RESULT_FAILED_TO_SPLIT
        };

        enum Type
        {
            TYPE_UNKNOWN,
            TYPE_INTERNAL,
            TYPE_FREE,
            TYPE_FULL
        };

        enum Split
        {
            SPLIT_UNKNOWN,
            SPLIT_HORIZONTAL,
            SPLIT_VERTICAL
        };

        enum Heuristic
        {
            FIT_UNKNOWN,
            FIT_NEXT,
            FIT_BEST_SHORT_SIDE
        };

        SpaceTree(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, Type type, Split split);

        void setParent(SpaceTree* parent)
        {
            _parent = parent;
        }

        void addChild(SpaceTree* child)
        {
            if (child != nullptr)
            {
                child->setParent(this);
                _children.addChild(child);
            }
        }

        SpaceTree* child(std::size_t index)
        {
            if (index<_children.size())
            {
                return _children[index];
            }

            return nullptr;
        }

        void traversal(const std::function<bool(SpaceTree*)>& callback);

        static SpaceTree* createNode(ConfigurationElement& config);

        static SpaceTree* fromConfig(nfe::ConfigurationElement& config);

        Result split(std::int32_t width, std::int32_t height, Split split);

        Result insert(std::int32_t width, std::int32_t height, Heuristic heuristic);

        ConfigurationElement::ValueType find(std::string_view path) const;

        static const char* typeToString(Type type);

        static Type parseType(const char* str);

        static const char* splitToString(Split split);

        static Split parseSplit(const char* str);

        static const char* resultToString(Result result);

        static Result parseResult(const char* str);

        static const char* heuristicToString(Heuristic value);

        static Heuristic parseHeuristic(const char* str);
    private:
        SpaceTree* _parent{nullptr};
        std::int32_t _x{0};
        std::int32_t _y{0};
        std::int32_t _width{0};
        std::int32_t _height{0};
        Type  _type{TYPE_UNKNOWN};
        Split _split{SPLIT_UNKNOWN};
        Children _children;
    };
}
