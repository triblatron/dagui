//
// Created by Tony Horrobin on 23/04/2025.
//

#pragma once

#include "config/Export.h"

#include "core/DynamicVisitor.h"
#include "core/Variant.h"
#include "core/IdentifierGenerator.h"

namespace dagui
{
    class SceneNode;
    using SceneNodeVisitor=dagbase::DynamicVisitor<SceneNode>;
    class Widget;

    class DAGUI_API SceneNode
    {
    public:
        SceneNode(const dagbase::Atom &className, dagbase::IdentifierGenerator::Identifier id, Widget* widget);

        virtual ~SceneNode() = default;

        dagbase::Atom typeName() const
        {
            return _className;
        }

        dagbase::IdentifierGenerator::Identifier id() const
        {
            return _id;
        }

        virtual void eachChild(std::function<bool(SceneNode*)> f) = 0;

        virtual void accept(SceneNodeVisitor& visitor) = 0;

        virtual dagbase::Variant find(std::string_view path) const;
    private:
        dagbase::Atom _className;
        dagbase::IdentifierGenerator::Identifier _id{0};
        Widget* _widget{nullptr};
    };
}
