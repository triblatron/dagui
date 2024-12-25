//
// Created by Tony Horrobin on 25/12/2024.
//

#pragma once

#include "config/Export.h"

namespace dagui
{
    class BinImageDef;
    class ImageDef;

    class DAGUI_API Renderer
    {
    public:
        virtual ~Renderer() = default;

        virtual void generateTextureCoordinates(ImageDef& imageDef, BinImageDef& binImageDef) = 0;
    };
}