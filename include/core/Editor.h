//
// Created by Tony Horrobin on 02/05/2026.
//

#pragma once

#include "config/Export.h"

#include "core/Variant.h"

#include <string_view>

namespace dagui
{
    class DAGUI_API Editor
    {
    public:
        virtual ~Editor() = default;

        virtual Editor* clone() = 0;

        virtual dagbase::Variant find(std::string_view path) const = 0;
    };
}