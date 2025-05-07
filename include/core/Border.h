//
// Created by Tony Horrobin on 14/04/2025.
//

#pragma once

#include "config/Export.h"

#include "core/SceneNode.h"

namespace dagui
{
    class Widget;

    class DAGUI_API Border : public SceneNode
    {
    public:
        Border(dagbase::IdentifierGenerator::Identifier id, Widget* widget);

        void accept(SceneNodeVisitor & visitor) override;

        void eachChild(std::function<bool(SceneNode*)> f) override
        {
            // Do nothing.
        }

        dagbase::Variant find(std::string_view path) const override;
    };

}
