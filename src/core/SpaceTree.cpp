//
// Created by tony on 02/10/24.
//

#include "config/config.h"

#include "core/SpaceTree.h"
#include "core/ConfigurationElement.h"
#include "util/enums.h"

#include <cstring>
#include <queue>

#include "util/Searchable.h"

namespace nfe
{
    ConfigurationElement::ValueType Children::find(std::string_view path) const
    {
        ConfigurationElement::ValueType retval = findArray(path, _a);

        if (retval.has_value())
        {
            return retval;
        }

        return {};
    }

    SpaceTree::SpaceTree(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, Type type, Split split)
        :
    _parent(nullptr),
    _x(x),
    _y(y),
    _width(width),
    _height(height),
    _type(type),
    _split(split)
    {
        // Do nothing.
    }

    void SpaceTree::traversal(const std::function<bool(SpaceTree*)>& callback)
    {
        if (!std::invoke(callback, this))
        {
            return;
        }

        for (auto child : _children)
        {
            if (child)
                child->traversal(callback);
        }
    }

    void SpaceTree::traversal(const std::function<bool(const SpaceTree*)>& callback) const
    {
        if (!std::invoke(callback, this))
        {
            return;
        }

        for (const auto* child : _children)
        {
            if (child)
                child->traversal(callback);
        }
    }

    SpaceTree* SpaceTree::createNode(ConfigurationElement& config)
    {
        std::int32_t x=0, y=0, width=0, height=0;
        if (auto xConfig = config.findElement("x"); xConfig)
        {
            x = static_cast<std::int32_t>(xConfig->asInteger());
        }
        if (auto yConfig = config.findElement("y"); yConfig)
        {
            y = static_cast<std::int32_t>(yConfig->asInteger());
        }
        if (auto widthConfig = config.findElement("width"); widthConfig)
        {
            width = static_cast<std::int32_t>(widthConfig->asInteger());
        }
        if (auto heightConfig = config.findElement("height"); heightConfig)
        {
            height = static_cast<std::int32_t>(heightConfig->asInteger());
        }
        auto type = TYPE_UNKNOWN;
        if (auto typeConfig = config.findElement("nodeType"); typeConfig)
        {
            type = parseType(typeConfig->asString().c_str());
        }
        auto split = SPLIT_UNKNOWN;
        if (auto splitConfig = config.findElement("split"); splitConfig)
        {
            split = parseSplit(splitConfig->asString().c_str());
        }
        return new SpaceTree(x, y, width, height, type, split);
    }

    SpaceTree* SpaceTree::fromConfig(nfe::ConfigurationElement& config)
    {
        auto root = createNode(config);

        using ConfigQueue = std::queue<ConfigurationElement*>;
        ConfigQueue configQueue;
        using ParentQueue = std::queue<SpaceTree*>;
        ParentQueue parentQueue;
        
        configQueue.push(&config);
        parentQueue.push(root);
        while (!configQueue.empty())
        {
            auto topConfig = configQueue.front();
            configQueue.pop();
            SpaceTree* parent = parentQueue.front();
            parentQueue.pop();
            if (auto childrenConfig=topConfig->findElement("children"); childrenConfig)
            {
                childrenConfig->eachChild([&configQueue, &parentQueue, &parent](nfe::ConfigurationElement& childConfig)
                {
                    auto child = createNode(childConfig);

                    parent->addChild(child);
                    configQueue.push(&childConfig);
                    parentQueue.push(child);
                    return true;
                });
            }
        }
        return root;
    }

    SpaceTree::Result SpaceTree::split(std::int32_t width, std::int32_t height, Split split)
    {
        switch (_type)
        {
        case TYPE_FREE:
            {
                _type = TYPE_INTERNAL;
                _split = split;
                switch (split)
                {
                case SPLIT_HORIZONTAL:
                    {
                        auto leftChild = new SpaceTree(_x, _y, width, _height, TYPE_FREE, SPLIT_UNKNOWN);
                        addChild(leftChild);
                        auto rightChild = new SpaceTree(_x+width, _y, _width-width, _height, TYPE_FREE, SPLIT_UNKNOWN);
                        addChild(rightChild);
                        break;
                    }
                case SPLIT_VERTICAL:
                    {
                        auto leftChild = new SpaceTree(_x, _y, width, height, TYPE_FREE, SPLIT_UNKNOWN);
                        addChild(leftChild);
                        auto rightChild = new SpaceTree(_x, _y+height, width, _height - height, TYPE_FREE, SPLIT_UNKNOWN);
                        addChild(rightChild);
                        break;
                    }
                default:
                    return RESULT_FAILED_TO_SPLIT;
                }
                break;
            }
        default:
            return RESULT_FAILED_TO_SPLIT;
        }
        return RESULT_OK;
    }

    SpaceTree::Result SpaceTree::insert(std::int32_t width, std::int32_t height, Heuristic heuristic)
    {
        // Try to fit according to the heuristic
        switch (heuristic)
        {
        case FIT_NEXT:
            // Find the next available large enough free space.
            switch (_type)
            {
                case TYPE_UNKNOWN:
                    break;
                case TYPE_FREE:
                    {
                        if (width > _width || height > _height)
                        {
                            return RESULT_FAILED_TO_INSERT;
                        }
                        split(width, height, SPLIT_HORIZONTAL);
                        child(0)->split(width, height, SPLIT_VERTICAL);
                        return RESULT_OK;
                    }
                case TYPE_FULL:
                    return RESULT_FAILED_TO_INSERT;
                case TYPE_INTERNAL:
                    // Recursively try to insert.
                    break;
            }
            break;
        case FIT_BEST_SHORT_SIDE:
            switch (_type)
            {
                case TYPE_UNKNOWN:
                    break;
                case TYPE_FREE:
                    {
                        std::size_t shortSide {0};

                        if (_width < _height)
                        {
                            shortSide = std::abs(_width - width);
                        }
                        else
                        {
                            shortSide = std::abs(_height - height);
                        }

                        break;
                    }
                case TYPE_FULL:
                    return RESULT_FAILED_TO_INSERT;
                case TYPE_INTERNAL:

                    break;
            }
            break;
        default:
            return RESULT_FAILED_TO_INSERT;
        }
        return RESULT_OK;
    }

    ConfigurationElement::ValueType SpaceTree::find(std::string_view path) const
    {
        ConfigurationElement::ValueType retval {};

        retval = findInternal(path, "children", _children);
        if (retval.has_value())
            return retval;

        retval = findEndpoint(path, "nodeType", typeToString(_type));
        if (retval.has_value())
            return retval;

        retval = findEndpoint(path, "x", static_cast<std::int64_t>(_x));
        if (retval.has_value())
            return retval;

        retval = findEndpoint(path, "y", static_cast<std::int64_t>(_y));
        if (retval.has_value())
            return retval;

        retval = findEndpoint(path, "width", static_cast<std::int64_t>(_width));
        if (retval.has_value())
            return retval;

        retval = findEndpoint(path, "height", static_cast<std::int64_t>(_height));
        if (retval.has_value())
            return retval;

        retval = findEndpoint(path, "split", splitToString(_split));
        if (retval.has_value())
            return retval;

        return {};
    }

    SpaceTree* SpaceTree::findSpace(std::int32_t width, std::int32_t height, Heuristic heuristic)
    {
        SpaceTree* freeNode{nullptr};

        switch (heuristic)
        {
            case FIT_NEXT:
                {

                    traversal([width, height, &freeNode] (SpaceTree* node)
                    {
                        switch(node->_type)
                        {
                        case TYPE_FREE:
                            if (node->_width >= width && node->_height >= height)
                            {
                                freeNode = node;
                                return false;
                            }
                            break;
                        case TYPE_FULL:
                            return true;
                        case TYPE_INTERNAL:
                            if (node->_width >= width && node->_height >= height)
                            {
                                return true;
                            }
                            else
                            {
                                return false;
                            }
                        default:
                            return true;
                        };

                        return true;
                    });
                }
        default:
            break;
        }
        return freeNode;
    }

    const char* SpaceTree::typeToString(Type type)
    {
        switch (type)
        {
            ENUM_NAME(TYPE_UNKNOWN)
            ENUM_NAME(TYPE_INTERNAL)
            ENUM_NAME(TYPE_FREE)
            ENUM_NAME(TYPE_FULL)
        }

        return "<error>";
    }

    SpaceTree::Type SpaceTree::parseType(const char* str)
    {
        TEST_ENUM(TYPE_UNKNOWN, str);
        TEST_ENUM(TYPE_INTERNAL, str);
        TEST_ENUM(TYPE_FREE, str);
        TEST_ENUM(TYPE_FULL, str);

        return TYPE_UNKNOWN;
    }

    const char* SpaceTree::splitToString(Split split)
    {
        switch (split)
        {
            ENUM_NAME(SPLIT_UNKNOWN)
            ENUM_NAME(SPLIT_HORIZONTAL)
            ENUM_NAME(SPLIT_VERTICAL)
        }

        return "<error>";
    }

    SpaceTree::Split SpaceTree::parseSplit(const char* str)
    {
        TEST_ENUM(SPLIT_UNKNOWN, str);
        TEST_ENUM(SPLIT_HORIZONTAL, str);
        TEST_ENUM(SPLIT_VERTICAL, str);

        return SPLIT_UNKNOWN;
    }

    const char* SpaceTree::resultToString(Result result)
    {
        switch (result)
        {
            ENUM_NAME(RESULT_OK)
            ENUM_NAME(RESULT_UNKNOWN)
            ENUM_NAME(RESULT_FAILED_TO_INSERT)
            ENUM_NAME(RESULT_FAILED_TO_SPLIT)
        }

        return "<error>";
    }

    SpaceTree::Result SpaceTree::parseResult(const char* str)
    {
        TEST_ENUM(RESULT_OK, str);
        TEST_ENUM(RESULT_UNKNOWN, str);
        TEST_ENUM(RESULT_FAILED_TO_INSERT, str);
        TEST_ENUM(RESULT_FAILED_TO_SPLIT, str);

        return RESULT_UNKNOWN;
    }

    const char* SpaceTree::heuristicToString(Heuristic value)
    {
        switch (value)
        {
            ENUM_NAME(FIT_UNKNOWN)
            ENUM_NAME(FIT_NEXT)
            ENUM_NAME(FIT_BEST_SHORT_SIDE)
        }

        return "<error>";
    }

    SpaceTree::Heuristic SpaceTree::parseHeuristic(const char* str)
    {
        TEST_ENUM(FIT_UNKNOWN, str);
        TEST_ENUM(FIT_NEXT, str);
        TEST_ENUM(FIT_BEST_SHORT_SIDE, str);

        return FIT_UNKNOWN;
    }
}
