//
// Created by tony on 30/09/24.
//

#include "config/config.h"

#include "gfx/PackingStrategy.h"
#include "util/enums.h"

#include <cstring>

namespace dagui
{
    const char* PackingStrategy::errorToString(Error err)
    {
        switch (err)
        {
            ENUM_NAME(ERR_OK)
            ENUM_NAME(ERR_FAILED_TO_PACK)
            ENUM_NAME(ERR_UNKNOWN)
        }

        return "<error>";
    }

    PackingStrategy::Error PackingStrategy::parseError(const char* str)
    {
        TEST_ENUM(ERR_OK, str);
        TEST_ENUM(ERR_FAILED_TO_PACK, str);

        return ERR_UNKNOWN;
    }
}
