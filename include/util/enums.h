//
// Created by tony on 20/08/24.
//

#pragma once

#include "config/Export.h"

#define BIT_NAME(mask, bit, str) \
    if (mask & bit) \
    {                            \
        str += #bit;             \
        str += ' ';              \
    }

#define TEST_BIT(needle, haystack, mask) \
    if (haystack.find(#needle)!=std::string::npos) \
    {                              \
        mask = static_cast<typeof(mask)>(mask | needle);            \
    }
