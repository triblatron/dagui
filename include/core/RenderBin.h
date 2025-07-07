//
// Created by Tony Horrobin on 25/06/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Shape.h"
#include "core/Variant.h"

#include <cstdint>
#include <string_view>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    struct DAGUI_API RenderBinKey
    {
        std::int32_t material{-1};
        std::int32_t texture{-1};
        std::int32_t shader{-1};
        std::int32_t layer{0};

        void configure(dagbase::ConfigurationElement& config);

        bool operator<(const RenderBinKey& other) const;
    };

    class DAGUI_API RenderBin
    {
    public:
//        virtual ~RenderBin() = default;
//
//        virtual void render() = 0;

        Mesh * mesh()
        {
            return _mesh;
        }

        dagbase::Variant find(std::string_view path) const;

        void configure(dagbase::ConfigurationElement& config);
    private:
        Mesh* _mesh;
    };
}
