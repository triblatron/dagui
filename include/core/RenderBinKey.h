//
// Created by Tony Horrobin on 22/08/2025.
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
    struct DAGUI_API RenderBinKey
    {
        std::int32_t material{-1};
        std::int32_t texture{-1};
        std::int32_t shader{-1};
        std::int32_t layer{0};

        void configure(dagbase::ConfigurationElement& config);

        bool operator<(const RenderBinKey& other) const;
    };

}