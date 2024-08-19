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
            BORDER_BIT = 1 << 4
        };

        enum Status : std::uint32_t
        {
            VISIBLE_BIT = 1 << 0,
            MINIMISED_BIT = 1 << 1,
            MAXIMISED_BIT = 1 << 2,
        };
    public:
        Window(Widget* parent);
    private:
        Features _features{ 0x0 };
        Status _status{ 0x0 };
    };
}