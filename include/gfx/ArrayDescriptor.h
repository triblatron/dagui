//
// Created by Tony Horrobin on 12/02/2025.
//

#pragma once

#include "config/Export.h"

#include "gfx/AttributeLayout.h"

#include <vector>

namespace dagui
{
    //! Description of the attributes in a generic attribute array
    struct DAGUI_API ArrayDescriptor
    {
        using AttributeLayoutArray = std::vector<std::vector<AttributeLayout>>;
        AttributeLayoutArray attributes;
    };
}