//
// Created by tony on 31/07/24.
//

#pragma once

#include "config/Export.h"

#include "core/Shape.h"

namespace dagui
{
    class DAGUI_API Circle : public Shape
    {
    public:
        Circle();

        void setPos(double x, double y)
        {
            _x = x;
            _y = y;
        }

        void setRadius(double radius)
        {
            _radius = radius;
        }

        bool isInside(float x, float y) override;

        void accept(ShapeVisitor &visitor) override;

        void configure(dagbase::ConfigurationElement &config, WidgetFactory &factory) override;

        dagbase::Variant find(std::string_view path) const override;
    private:
        float _x{0};
        float _y{0};
        float _radius{0};
    };
}