//
// Created by tony on 30/07/24.
//

#pragma once

#include "config/Export.h"

#include <string>

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
    private:
        std::string _id;
        Widget* _parent{nullptr};
        Shape* _shape{nullptr};
    };
}
