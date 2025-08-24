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
#include "core/Style.h"

#include <glm/vec2.hpp>

#include <string>
#include <string_view>

namespace dagui
{
    class Batcher;
    class GraphicsBackendFactory;
    class SceneNode;
    class Shape;
    class ShapeFactory;
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

        void addShape(Shape* shape)
        {
            if (shape)
                _shapes.a.emplace_back(shape);
        }

        void setX(int x)
        {
            _pos[0] = x;
        }

        void setY(int y)
        {
            _pos[1] = y;
        }

        virtual void configure(dagbase::ConfigurationElement &config, WidgetFactory &factory, ShapeFactory &shapeFactory);

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

        void addConstraint(const Constraint& constraint)
        {
            _constraints.a.emplace_back(constraint);
        }

        void eachConstraint(std::function<bool(Constraint&)> f);

        void resolveRefs();

        virtual StyleLookup* styleLookup()
        {
            return nullptr;
        }

        virtual void draw(Batcher& batcher, GraphicsBackendFactory& factory);
    private:
        glm::ivec2 _pos{0,0};
        using Properties=dagbase::SearchableMap<std::unordered_map<dagbase::Atom, dagbase::Variant>>;
        Properties _props;
        dagbase::Atom _id;
        using ChildArray = dagbase::SearchableArray<std::vector<Widget*>>;
        ChildArray _children;
        using ConstraintArray = dagbase::SearchableArray<std::vector<Constraint>>;
        ConstraintArray _constraints;
        dagbase::Atom _typeName;
        dagbase::Atom _styleClass;
        StyleRef _style;
        Widget* _parent{nullptr};
        using ShapeArray = dagbase::SearchableArray<std::vector<Shape*>>;
        ShapeArray _shapes;
    };
}
