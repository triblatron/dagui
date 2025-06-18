//
// Created by Tony Horrobin on 23/04/2025.
//

#pragma once

#include "config/Export.h"

#include "core/DynamicVisitor.h"
#include "core/Variant.h"
#include "core/IdentifierGenerator.h"

#include <glm/glm.hpp>

#include <cstdint>

namespace dagui
{
    class SceneNode;
    using SceneNodeVisitor=dagbase::DynamicVisitor<SceneNode>;
    class Widget;

    class DAGUI_API SceneNode
    {
    public:
        SceneNode(const dagbase::Atom &className, dagbase::IdentifierGenerator::Identifier id, Widget* widget, SceneNode* parent=nullptr);

        virtual ~SceneNode() = default;

        dagbase::Atom typeName() const
        {
            return _className;
        }

        dagbase::IdentifierGenerator::Identifier id() const
        {
            return _id;
        }

        void setParent(SceneNode* parent)
        {
            _parent = parent;
        }

        SceneNode* parent()
        {
            return _parent;
        }

        virtual void eachChild(std::function<bool(SceneNode*)> f) = 0;

        virtual void accept(SceneNodeVisitor& visitor) = 0;

        virtual dagbase::Variant find(std::string_view path) const;
    private:
        dagbase::Atom _className;
        dagbase::IdentifierGenerator::Identifier _id{dagbase::IdentifierGenerator::INVALID_ID};
        Widget* _widget{nullptr};
        SceneNode* _parent{nullptr};
        std::int32_t _zOrder{0};
    };
}
