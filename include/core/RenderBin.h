//
// Created by Tony Horrobin on 25/06/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Shape.h"
#include "core/Variant.h"

#include <cstdint>
#include <string_view>
#include "core/RenderBinKey.h"

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class Blending;
    class TextureAtlas;

    class DAGUI_API RenderBin
    {
    public:
        void setMesh(ShapeMesh* mesh)
        {
            _mesh = mesh;
        }

        ShapeMesh * mesh()
        {
            return _mesh;
        }

        void setAtlas(TextureAtlas* atlas)
        {
            _atlas = atlas;
        }

        TextureAtlas* atlas()
        {
            return _atlas;
        }

        void setBlending(Blending* blending)
        {
            _blending = blending;
        }

        Blending* blending()
        {
            return _blending;
        }

        dagbase::Variant find(std::string_view path) const;

        void configure(dagbase::ConfigurationElement& config);

        void draw();
    private:
        ShapeMesh* _mesh{nullptr};
        TextureAtlas* _atlas{nullptr};
        Blending* _blending{nullptr};
    };
}
