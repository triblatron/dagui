//
// Created by tony on 30/07/24.
//

#pragma once

#include "config/Export.h"

namespace nfe
{
    class Shape;

    class NFE_API Widget
    {
    public:
        explicit Widget(Widget* parent= nullptr);

        virtual ~Widget() = default;

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
        Widget* _parent{nullptr};
        Shape* _shape{nullptr};
    };
}
