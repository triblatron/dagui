//
// Created by tony on 31/07/24.
//

#pragma once

#include "config/Export.h"

#include "core/Shape.h"

namespace nfe
{
    class NFE_API Circle : public Shape
    {
    public:
        void setPos(double x, double y)
        {
            _x = x;
            _y = y;
        }

        void setRadius(double radius)
        {
            _radius = radius;
        }

        bool isInside(double x, double y) override;

        void accept(ShapeVisitor &visitor) override;
    private:
        double _x{0};
        double _y{0};
        double _radius{0};
    };
}