//
// Created by tony on 19/08/24.
//

#pragma once

#include "config/Export.h"

#include "core/Widget.h"

#include <cstdint>

namespace nfe
{
    class NFE_API Window : public Widget
    {
    public:
        enum Features : std::uint32_t
        {
            TITLE_BIT = 1 << 0,
            MINIMISE_BIT = 1 << 1,
            MAXIMISE_BIT = 1 << 2,
            CLOSE_BIT = 1 << 3,
            BORDER_BIT = 1 << 4,
            RESIZEABLE_BIT = 1 << 5,
            DEFAULT_FEATURES = TITLE_BIT | MINIMISE_BIT | MAXIMISE_BIT | CLOSE_BIT | BORDER_BIT | RESIZEABLE_BIT
        };

        enum Status : std::uint32_t
        {
            VISIBLE_BIT = 1 << 0,
            MINIMISED_BIT = 1 << 1,
            MAXIMISED_BIT = 1 << 2,
        };
    public:
        explicit Window(Widget* parent);
    private:
        Features _features{ DEFAULT_FEATURES };
        Status _status{ 0x0 };
    };
}