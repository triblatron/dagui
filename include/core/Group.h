//
// Created by Tony Horrobin on 18/04/2025.
//

#pragma once

#include "config/Export.h"

#include "core/SceneNode.h"

#include "util/SearchableArray.h"

#include <vector>

namespace dagui
{
    class DAGUI_API Group : public SceneNode
    {
    public:
        Group(Widget* widget);

        void eachChild(std::function<bool(SceneNode*)> f) override;

        void accept(SceneNodeVisitor& visitor) override
        {
            visitor.visit(*this);
            for (auto child : _children.a)
            {
                child->accept(visitor);
            }
        }

        void addChild(SceneNode* child)
        {
            if (child)
            {
                _children.a.push_back(child);
            }
        }

        dagbase::Variant find(std::string_view path) const override;
    private:
        using ChildArray = dagbase::SearchableArray<std::vector<SceneNode*>>;
        ChildArray _children;
    };
}
