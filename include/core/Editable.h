//
// Created by Tony Horrobin on 15/06/2026.
//

#pragma once

#include "config/Export.h"

struct ImGuiContext;

namespace dagui
{
    class DAGUI_API Editable
    {
    public:
        virtual ~Editable() = default;

        virtual void edit(ImGuiContext* context) = 0;
    };
}