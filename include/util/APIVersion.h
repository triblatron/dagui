//
// Created by Tony Horrobin on 25/01/2025.
//

#pragma once

#include "config/Export.h"

namespace dagui
{
    struct DAGUI_API APIVersion
    {
        int major{0};
        int minor{0};
        int patch{0};

        static int compare(const APIVersion& a, const APIVersion& b);
    };
}