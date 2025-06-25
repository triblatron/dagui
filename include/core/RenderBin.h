//
// Created by Tony Horrobin on 25/06/2025.
//

#pragma once

#include "config/Export.h"

#include <cstdint>

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

        void configure(dagbase::ConfigurationElement& config);

        bool operator<(const RenderBin& other) const;
    private:
        std::int32_t _material{-1};
        std::int32_t _texture{-1};
        std::int32_t _layer{0};
    };
}