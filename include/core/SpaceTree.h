//
// Created by tony on 02/10/24.
//

#pragma once

#include "config/Export.h"

#include <cstdlib>
#include <functional>
#include <vector>

namespace nfe
{
    class ConfigurationElement;

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

        SpaceTree(SpaceTree* parent, std::size_t width, std::size_t height, Type type);

        void setParent(SpaceTree* parent)
        {
            _parent = parent;
        }

        void addChild(SpaceTree* child)
        {
            if (child != nullptr)
            {
                child->setParent(this);
                _children.push_back(child);
            }
        }

        void traversal(const std::function<void(SpaceTree*)>& callback);

        static SpaceTree* fromConfig(nfe::ConfigurationElement& config);

        Result insert(std::size_t width, std::size_t height);
    private:
        SpaceTree* _parent{nullptr};
        std::size_t _width{0};
        std::size_t _height{0};
        Type  _type{TYPE_UNKNOWN};
        using ChildArray = std::vector<SpaceTree*>;
        ChildArray _children;
    };
}
