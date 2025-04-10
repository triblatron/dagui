//
// Created by Tony Horrobin on 09/04/2025.
//

#pragma once

#include "Widget.h"
#include "config/Export.h"
#include "util/Searchable.h"
#include <unordered_map>

namespace dagui
{
    class DAGUI_API RootWidget : public dagui::Widget
    {
    public:
        RootWidget() = default;

    private:
        using WidgetLookup = std::unordered_map<std::string, std::shared_ptr<Widget>>;

    };
}