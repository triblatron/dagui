//
// Created by Tony Horrobin on 17/06/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Variant.h"
#include "util/SearchableArray.h"

#include <string_view>
#include <vector>
#include <functional>

namespace dagui
{
    class DrawCommand;
    class Rectangle;

    class DAGUI_API DrawCommandBuffer
    {
    public:
        void drawRect(const Rectangle& rect);

        void eachCommand(std::function<void(DrawCommand*)> f);

        dagbase::Variant find(std::string_view path) const;
    private:
        using DrawCommands = dagbase::SearchableArray<std::vector<DrawCommand*>>;
        DrawCommands _cmds;
        void addCommand(DrawCommand* cmd)
        {
            if (cmd)
                _cmds.a.emplace_back(cmd);
        }
    };
}
