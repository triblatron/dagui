//
// Created by tony on 07/08/24.
//

#include "config/Export.h"

#include "core/Shape.h"

#include <vector>

namespace nfe
{
    class NFE_API CompositeShape : Shape
    {
    public:
        void accept(ShapeVisitor& visitor) override;

        bool isInside(double x, double y) override;

        void addShape(Shape* shape);
    private:
        using ShapeArray = std::vector<Shape*>;
        ShapeArray _shapes;
    };
}