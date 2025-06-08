//
// Created by tony on 07/08/24.
//

#include "config/Export.h"

#include "core/Shape.h"

#include <vector>

namespace dagui
{
    class DAGUI_API CompositeShape : Shape
    {
    public:
        CompositeShape();

        void accept(ShapeVisitor& visitor) override;

        bool isInside(float x, float y) override;

        void addShape(Shape* shape);

        void configure(dagbase::ConfigurationElement &config, WidgetFactory &factory) override;

        dagbase::Variant find(std::string_view path) const override;
    private:
        using ShapeArray = std::vector<Shape*>;
        ShapeArray _shapes;
    };
}