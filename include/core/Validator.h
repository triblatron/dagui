//
// Created by Tony Horrobin on 29/03/2025.
//

#pragma once

#include "config/Export.h"

#include <cstdint>

namespace dagui
{
    class DAGUI_API Validator
    {
    public:
        enum Status : std::uint32_t
        {
            STATUS_OK,
            STATUS_ERR_EMPTY,
            STATUS_ERR_SIGN,
            STATUS_ERR_INTEGER,
            STATUS_ERR_POINT,
            STATUS_ERR_EXPONENT_SIGN,
            STATUS_ERR_EXPONENT,
            STATUS_ERR_TOO_LOW,
            STATUS_ERR_TOO_HIGH,
            STATUS_UNKNOWN
        };
    public:
        Validator() = default;

        virtual ~Validator() = default;

        static const char* statusString(Status status);

        static Status parseStatus(const char* str);
    };
}