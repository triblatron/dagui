//
// Created by Tony Horrobin on 12/05/2025.
//

#ifndef DAGUI_LAYOUTPROPERTIES_H
#define DAGUI_LAYOUTPROPERTIES_H

#include "config/Export.h"

#include <cstdint>
#include <optional>

namespace dagui
{
    struct DAGUI_API LayoutProperties
    {
        enum HorizontalAlignment : std::uint32_t
        {
            ALIGN_LEFT,
            ALIGN_CENTRE,
            ALIGN_RIGHT
        };

        float spacing{0.0f};
        HorizontalAlignment horizontalAlignment{ALIGN_LEFT};
        std::optional<float> maxHeight;
        enum VerticalSizePolicy : std::uint32_t
        {
            SIZE_EXPAND
        };
        VerticalSizePolicy verticalSizePolicy{SIZE_EXPAND};
    };
}

#endif //DAGUI_LAYOUTPROPERTIES_H
