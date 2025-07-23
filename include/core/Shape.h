//
// Created by tony on 30/07/24.
//

#pragma once

#include "config/Export.h"

#include "core/Atom.h"
#include "core/Variant.h"
#include "gfx/GenericMesh.h"

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
    class ShapeFactory;

    struct DAGUI_API ShapeVertex
    {
        float x{0.0f};
        float y{0.0f};
        float r{0.0f};
        float g{0.0f};
        float b{0.0f};
        float a{0.0f};

        void configure(dagbase::ConfigurationElement& config);

        dagbase::Variant find(std::string_view path) const;

        bool operator<(const ShapeVertex& other) const;
    };

    using ShapeMesh = GenericMesh<ShapeVertex>;

    class DAGUI_API Shape
    {
    public:
        enum Flags : std::uint32_t
        {
            FLAGS_NONE,
            FLAGS_DIRTY_BIT = 1<<0
        };
    public:
        Shape(dagbase::Atom className);

        virtual ~Shape() = default;

        dagbase::Atom className() const
        {
            return _className;
        }

        void setFlag(Flags mask)
        {
            _flags = static_cast<Flags>(_flags | mask);
        }

        void clearFlag(Flags mask)
        {
            _flags = static_cast<Flags>(_flags & ~mask);
        }

        bool isFlagSet(Flags mask) const
        {
            return (_flags & mask) != FLAGS_NONE;
        }

        virtual void accept(ShapeVisitor& visitor) = 0;

        virtual bool isInside(float x, float y) = 0;

        virtual void configure(dagbase::ConfigurationElement &config, dagui::ShapeFactory &factory) = 0;

        virtual dagbase::Variant find(std::string_view path) const;

        virtual void render(DrawCommandBuffer& buffer) {}

        virtual void tessellate(ShapeMesh& mesh) {};

        static std::string flagsToString(Flags value);

        static Flags parseFlags(const std::string& str);
    private:
        dagbase::Atom _className;
        Flags _flags{FLAGS_NONE};
    };
}
