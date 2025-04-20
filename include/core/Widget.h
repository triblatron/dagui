//
// Created by tony on 30/07/24.
//

#pragma once

#include "config/Export.h"

#include "core/ConfigurationElement.h"
#include "core/DynamicVisitor.h"

#include <string>
#include <string_view>

namespace dagui
{
    class WidgetFactory;
}

namespace dagui
{
    class Shape;
    class Widget;
    using WidgetVisitor = dagbase::DynamicVisitor<Widget>;

    class DAGUI_API Widget
    {
    public:
        explicit Widget(dagbase::Atom typeName, Widget* parent= nullptr);

        virtual ~Widget() = default;

        void setId(const std::string& id)
        {
            _id = id;
        }

        const std::string& id() const
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

        virtual void configure(dagbase::ConfigurationElement& config, WidgetFactory& factory);

        virtual Widget* root();

        void addChild(Widget* child);

        virtual void addIdentified(Widget* widget)
        {
            // Do nothing.
        }

        virtual Widget* lookupWidget(std::string name);

        virtual dagbase::ConfigurationElement::ValueType find(std::string_view path) const;

        virtual void accept(WidgetVisitor& visitor);

        dagbase::Atom typeName()
        {
            return _typeName;
        }

        void eachChild(std::function<bool(Widget&)>);
    private:
        dagbase::Atom _typeName;
        std::string _id;
        Widget* _parent{nullptr};
        Shape* _shape{nullptr};
        using ChildArray = std::vector<Widget*>;
        ChildArray _children;
    };
}
