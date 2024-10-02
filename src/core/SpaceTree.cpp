//
// Created by tony on 02/10/24.
//

#include "config/config.h"

#include "core/SpaceTree.h"

#include <queue>

#include "core/ConfigurationElement.h"

#include <stack>

namespace nfe
{
    SpaceTree::SpaceTree(SpaceTree* parent, std::size_t width, std::size_t height, Type type)
        :
    _parent(parent),
    _width(width),
    _height(height),
    _type(type)
    {
        // Do nothing.
    }

    void SpaceTree::traversal(const std::function<void(SpaceTree*)>& callback)
    {
        std::invoke(callback, this);

        for (auto child : _children)
        {
            child->traversal(callback);
        }
    }

    SpaceTree* SpaceTree::fromConfig(nfe::ConfigurationElement& config)
    {
        size_t width=0, height=0;
        if (auto widthConfig = config.findElement("width"); widthConfig)
        {
            width = static_cast<size_t>(widthConfig->asInteger());
        }
        if (auto heightConfig = config.findElement("height"); heightConfig)
        {
            height = static_cast<size_t>(heightConfig->asInteger());
        }
        auto root = new SpaceTree(nullptr, width, height, TYPE_FREE);

        using ConfigQueue = std::queue<ConfigurationElement*>;
        ConfigQueue configQueue;
        configQueue.push(&config);
        using ParentStack = std::stack<SpaceTree*>;
        ParentStack parentStack;
        parentStack.push(root);
        SpaceTree* parent = root;
        while (!configQueue.empty())
        {
            auto topConfig = configQueue.front();
            configQueue.pop();
            if (auto childrenConfig=topConfig->findElement("children"); childrenConfig)
            {
                childrenConfig->eachChild([&configQueue, &parentStack, &parent](nfe::ConfigurationElement& childConfig)
                {
                    std::size_t width=0, height=0;
                    if (auto widthConfig = childConfig.findElement("width"); widthConfig)
                    {
                        width = static_cast<std::size_t>(widthConfig->asInteger());
                    }
                    if (auto heightConfig = childConfig.findElement("height"); heightConfig)
                    {
                        height = static_cast<size_t>(heightConfig->asInteger());
                    }
                    auto child = new SpaceTree(nullptr, width, height, TYPE_FREE);
                    parentStack.top()->addChild(child);
                    configQueue.push(&childConfig);
                    parentStack.push(child);
                    return true;
                });
            }
        }
        return root;
    }

    SpaceTree::Result SpaceTree::insert(std::size_t width, std::size_t height)
    {
        if (width > _width || height > _height)
        {
            return RESULT_FAILED_TO_INSERT;
        }
        return RESULT_OK;
    }
}
