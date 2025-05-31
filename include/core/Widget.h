//
// Created by tony on 30/07/24.
//

#pragma once

#include "config/Export.h"

#include "core/ConfigurationElement.h"
#include "core/DynamicVisitor.h"
#include "util/SearchableMap.h"
#include "core/Constraint.h"
#include "util/SearchableArray.h"
#include "core/Reference.h"
#include "core/Atom.h"

#include <glm/vec2.hpp>

#include <string>
#include <string_view>

namespace dagui
{
    class SceneNode;
    class Shape;
    class Widget;
    class WidgetFactory;

    using WidgetVisitor = dagbase::DynamicVisitor<Widget>;

    class DAGUI_API Widget
    {
    public:
        explicit Widget(dagbase::Atom typeName, Widget* parent= nullptr);

        virtual ~Widget() = default;

        void setId(const std::string& id)
        {
            _id = dagbase::Atom::intern(id);
        }

        const dagbase::Atom& id() const
        {
            return _id;
        }

        void setShape(Shape* shape)
        {
            _shape = shape;
        }

        Shape* shape()
        {
            return _shape;
        }

        const Shape* shape() const
        {
            return _shape;
        }

        void setX(int x)
        {
            _pos[0] = x;
        }

        void setY(int y)
        {
            _pos[1] = y;
        }

        virtual void configure(dagbase::ConfigurationElement& config, WidgetFactory& factory);

        virtual Widget* root();

        void addChild(Widget* child);

        virtual void addIdentified(Widget* widget)
        {
            // Do nothing.
        }

        virtual Widget* lookup(dagbase::Atom name);

        virtual dagbase::Variant find(std::string_view path) const;

        virtual void accept(WidgetVisitor& visitor);

        dagbase::Atom typeName()
        {
            return _typeName;
        }

        void eachChild(std::function<bool(Widget&)> f);

        void eachConstraint(std::function<bool(Constraint&)> f);
    private:
        glm::ivec2 _pos;
        using Properties=dagbase::SearchableMap<std::unordered_map<dagbase::Atom, dagbase::Variant>>;
        Properties _props;
        dagbase::Atom _id;
        using ChildArray = std::vector<Widget*>;
        ChildArray _children;
        dagbase::Atom _typeName;
        Widget* _parent{nullptr};
        SceneNode* _sceneNode{nullptr};
        Shape* _shape{nullptr};
        using ConstraintArray = dagbase::SearchableArray<std::vector<Constraint>>;
        ConstraintArray _constraints;
    };
}
