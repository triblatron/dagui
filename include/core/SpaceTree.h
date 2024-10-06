//
// Created by tony on 02/10/24.
//

#pragma once

#include "config/Export.h"

#include <cstdlib>
#include <functional>
#include <vector>

#include "ConfigurationElement.h"

namespace nfe
{
    class ConfigurationElement;
    class SpaceTree;

    struct NFE_API Children
    {
        using ChildArray = std::vector<SpaceTree*>;
        ChildArray a;

        ConfigurationElement::ValueType find(std::string path) const;
    };

    class NFE_API SpaceTree
    {
    public:
        enum Result
        {
            RESULT_OK,
            RESULT_FAILED_TO_INSERT
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

        SpaceTree(std::size_t width, std::size_t height, Type type, Split split);

        void setParent(SpaceTree* parent)
        {
            _parent = parent;
        }

        void addChild(SpaceTree* child)
        {
            if (child != nullptr)
            {
                child->setParent(this);
                _children.a.push_back(child);
            }
        }

        void traversal(const std::function<void(SpaceTree*)>& callback);

        static SpaceTree* createNode(ConfigurationElement& config);

        static SpaceTree* fromConfig(nfe::ConfigurationElement& config);

        Result insert(std::size_t width, std::size_t height);

        ConfigurationElement::ValueType find(std::string path) const;

        static const char* typeToString(Type type);

        static Type parseType(const char* str);

        static const char* splitToString(Split split);

        static Split parseSplit(const char* str);
    private:
        SpaceTree* _parent{nullptr};
        std::size_t _width{0};
        std::size_t _height{0};
        Type  _type{TYPE_UNKNOWN};
        Split _split{SPLIT_UNKNOWN};
        Children _children;
    };
}
