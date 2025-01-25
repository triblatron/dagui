//
// Created by Tony Horrobin on 25/01/2025.
//

#include "config/config.h"

#include "util/APIVersion.h"

namespace dagui
{
    int APIVersion::compare(const APIVersion& a, const APIVersion& b)
    {
        if (a.major > b.major)
            return 1;
        else if (a.major < b.major)
            return -1;
        else if (a.minor > b.minor)
            return 1;
        else if (a.minor < b.minor)
            return -1;
        else if (a.patch > b.patch)
            return 1;
        else if (a.patch < b.patch)
            return -1;

        return 0;
    }

}