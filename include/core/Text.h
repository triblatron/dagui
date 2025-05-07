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
        explicit Text(dagbase::IdentifierGenerator::Identifier id, Widget* widget);

        void accept(SceneNodeVisitor& visitor) override
        {
            // Do nothing.
        }

        void eachChild(std::function<bool(SceneNode*)> f) override
        {
            // Do nothing.
        }

        void setText(const std::string& text)
        {
            _text = text;
        }

        dagbase::Variant find(std::string_view path) const override;
    private:
        std::string _text;
    };
}