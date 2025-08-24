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
    class DAGUI_API RenderBin
    {
    public:
//        virtual ~RenderBin() = default;
//
//        virtual void render() = 0;

        void setMesh(ShapeMesh* mesh)
        {
            _mesh = mesh;
        }

        ShapeMesh * mesh()
        {
            return _mesh;
        }

        dagbase::Variant find(std::string_view path) const;

        void configure(dagbase::ConfigurationElement& config);
    private:
        ShapeMesh* _mesh;
    };
}
