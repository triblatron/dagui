//
// Created by tony on 30/07/24.
//

#include "config/Export.h"

#include "core/Shape.h"

namespace dagui
{
    class Tessellation;

    class DAGUI_API Rectangle : public Shape
    {
    public:
        Rectangle();

        void setPos(float x, float y)
        {
            _x = x;
            _y = y;
        }

        float x() const
        {
            return _x;
        }

        float y() const
        {
            return _y;
        }

        void setSize(float width, float height)
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

        void configure(dagbase::ConfigurationElement &config, ShapeFactory &factory) override;

        dagbase::Variant find(std::string_view path) const override;

        void render(DrawCommandBuffer& buffer) override;

        void tessellate(ShapeMesh& mesh) override;
    private:
        float _x{0.0f};
        float _y{0.0f};
        float _width{0.0f};
        float _height{0.0f};
        float _cornerRadius{0.0f};
        std::uint32_t _numCornerVertices{0};
        bool isInCorner(float x, float y, float d, float d1);
        void addSubRectangle(float x, float y, float width, float height, Tessellation& tess);
        void addCorner(float x, float y, float xRadius, float yRadius, Tessellation& tess);
    };
}
