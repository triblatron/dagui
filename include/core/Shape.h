//
// Created by tony on 30/07/24.
//

#pragma once

#include "config/Export.h"

#include "core/Atom.h"
#include "core/Variant.h"
#include "WidgetFactory.h"

#include <string_view>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class DrawCommandBuffer;
    class Mesh;
    class ShapeVisitor;

    class DAGUI_API Shape
    {
    public:
        Shape(dagbase::Atom className);

        virtual ~Shape() = default;

        dagbase::Atom className() const
        {
            return _className;
        }

        virtual void accept(ShapeVisitor& visitor) = 0;

        virtual bool isInside(float x, float y) = 0;

        virtual void configure(dagbase::ConfigurationElement &config, dagui::WidgetFactory &factory) = 0;

        virtual dagbase::Variant find(std::string_view path) const;

        virtual void render(DrawCommandBuffer& buffer) {}

        virtual void tessellate(Mesh& mesh) {};
    private:
        dagbase::Atom _className;
    };
}
