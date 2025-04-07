//
// Created by tony on 30/07/24.
//

#pragma once

#include "config/Export.h"

#include "core/ConfigurationElement.h"

#include <string>
#include <string_view>

namespace dagui
{
    class WidgetFactory;
}

namespace dagui
{
    class Shape;

    class DAGUI_API Widget
    {
    public:
        explicit Widget(Widget* parent= nullptr);

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

        virtual dagbase::ConfigurationElement::ValueType find(std::string_view path);
    private:
        std::string _id;
        Widget* _parent{nullptr};
        Shape* _shape{nullptr};
    };
}
