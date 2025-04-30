//
// Created by Tony Horrobin on 30/04/2025.
//

#pragma once

#include "config/Export.h"

#include "core/SceneNode.h"

namespace dagui
{
    class DAGUI_API Text : public SceneNode
    {
    public:
        explicit Text(Widget* widget);

        void accept(SceneNodeVisitor& visitor) override
        {
            // Do nothing.
        }

        void eachChild(std::function<bool(SceneNode*)> f) override
        {
            // Do nothing.
        }

        dagbase::Variant find(std::string_view path) const override;
    };
}