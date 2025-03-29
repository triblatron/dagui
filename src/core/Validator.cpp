//
// Created by Tony Horrobin on 29/03/2025.
//

#include "config/config.h"

#include "core/Validator.h"
#include "util/enums.h"

namespace dagui
{
    const char* Validator::statusString(Status status)
    {
        switch (status)
        {
            ENUM_NAME(STATUS_OK)
            ENUM_NAME(STATUS_ERR_EMPTY)
            ENUM_NAME(STATUS_ERR_SIGN)
            ENUM_NAME(STATUS_ERR_INTEGER)
            ENUM_NAME(STATUS_ERR_POINT)
            ENUM_NAME(STATUS_ERR_EXPONENT_SIGN)
            ENUM_NAME(STATUS_ERR_EXPONENT)
            ENUM_NAME(STATUS_ERR_TOO_LOW)
            ENUM_NAME(STATUS_ERR_TOO_HIGH)
            ENUM_NAME(STATUS_UNKNOWN)
        }
        return "<error>";
    }

    Validator::Status Validator::parseStatus(const char* str)
    {
        TEST_ENUM(STATUS_OK, str);
        TEST_ENUM(STATUS_ERR_EMPTY, str);
        TEST_ENUM(STATUS_ERR_SIGN, str);
        TEST_ENUM(STATUS_ERR_INTEGER, str);
        TEST_ENUM(STATUS_ERR_POINT, str);
        TEST_ENUM(STATUS_ERR_EXPONENT_SIGN, str);
        TEST_ENUM(STATUS_ERR_EXPONENT, str);
        TEST_ENUM(STATUS_ERR_TOO_LOW, str);
        TEST_ENUM(STATUS_ERR_TOO_HIGH, str);

        return STATUS_UNKNOWN;
    }
}