//
// Created by Tony Horrobin on 25/01/2025.
//

#pragma once

#include "config/Export.h"

#include <string_view>

namespace dagui
{
    class Renderer;
    struct APIVersion;

    class DAGUI_API RendererFactory
    {
    public:
        Renderer* createRenderer(std::string_view apiName, const APIVersion& version);
    };
}