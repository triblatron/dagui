//
// Created by Tony Horrobin on 12/05/2025.
//

#ifndef DAGUI_LAYOUTPROPERTIES_H
#define DAGUI_LAYOUTPROPERTIES_H

#include "config/Export.h"

#include "core/Variant.h"
#include "util/SearchableArray.h"

#include <cstdint>
#include <optional>
#include <vector>
#include <string_view>
#include <array>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    struct DAGUI_API LayoutProperties
    {
        enum HorizontalAlignment : std::uint32_t
        {
            HORIZ_ALIGN_UNKNOWN,
            HORIZ_ALIGN_LEFT,
            HORIZ_ALIGN_CENTRE,
            HORIZ_ALIGN_RIGHT
        };

        float spacing{0.0f};
        dagbase::SearchablePrimitiveArray<std::array<float,4>> padding;

        HorizontalAlignment horizontalAlignment{HORIZ_ALIGN_LEFT};

        enum VerticalAlignment : std::uint32_t
        {
            VERTICAL_ALIGN_UNKNOWN,
            VERTICAL_ALIGN_TOP,
            VERTICAL_ALIGN_CENTRE,
            VERTICAL_ALIGN_BOTTOM
        };

        VerticalAlignment verticalAlignment{VERTICAL_ALIGN_CENTRE};

        std::optional<float> minHeight{0.0f};
        std::optional<float> maxHeight{0.0f};

        enum VerticalSizePolicy : std::uint32_t
        {
            SIZE_EXPAND
        };
        VerticalSizePolicy verticalSizePolicy{SIZE_EXPAND};
        enum Overflow : std::uint32_t
        {
            OVERFLOW_SCROLL,
            OVERFLOW_CLIP,
            OVERFLOW_VISIBLE
        };
        Overflow overflow{OVERFLOW_CLIP};
        struct GrowthFactor
        {
            float width{0.0f};
            float height{0.0};
        };
        int growthWidth{0};
        int growthHeight{0};

        void configure(dagbase::ConfigurationElement& config);

        dagbase::Variant find(std::string_view path) const;

        static HorizontalAlignment parseHorizontalAlignment(const char* str);

        static VerticalAlignment parseVerticalAlignment(const char* str);
    };
}

#endif //DAGUI_LAYOUTPROPERTIES_H
