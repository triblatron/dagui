//
// Created by Tony Horrobin on 27/08/2025.
//

#pragma once

#include "config/Export.h"

#include <cstdint>

namespace dagui
{
    class DAGUI_API Blending
    {
    public:
        enum Factor : std::uint32_t
        {
            FACTOR_UNKNOWN,
            FACTOR_SRC_ALPHA,
            FACTOR_ONE_MINUS_SRC_ALPHA
        };
    public:
        virtual ~Blending() = default;

        virtual void enable() = 0;

        void setFunc(Factor srcFactor, Factor destFactor)
        {
            _srcFactor = srcFactor;
            _destFactor = destFactor;
        }

        virtual void makeItSo() = 0;

        virtual void disable() = 0;

    protected:
        Factor _srcFactor{FACTOR_UNKNOWN};
        Factor _destFactor{FACTOR_UNKNOWN};
    };
}