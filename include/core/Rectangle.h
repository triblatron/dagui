//
// Created by tony on 30/07/24.
//

#include "config/Export.h"

#include "core/Shape.h"
#include "WidgetFactory.h"

namespace dagui
{
    class DAGUI_API Rectangle : public Shape
    {
    public:
        Rectangle();

        void setPos(double x, double y)
        {
            _x = x;
            _y = y;
        }

        void setSize(double width, double height)
        {
            _width = width;
            _height = height;
        }

        double width() const
        {
            return _width;
        }

        double height() const
        {
            return _height;
        }

        void setCornerRadius(double radius)
        {
            _cornerRadius = radius;
        }
        
        void accept(ShapeVisitor& visitor) override;

        bool isInside(float x, float y) override;

        void configure(dagbase::ConfigurationElement &config, WidgetFactory &factory) override;

        dagbase::Variant find(std::string_view path) const override;
    private:
        float _x{0.0f};
        float _y{0.0f};
        float _width{0.0f};
        float _height{0.0f};
        float _cornerRadius{0.0f};

        bool isInCorner(float x, float y, float d, float d1);
    };
}
