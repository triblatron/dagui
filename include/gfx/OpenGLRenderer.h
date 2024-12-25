//
// Created by Tony Horrobin on 25/12/2024.
//

#pragma once

#include "gfx/Renderer.h"

namespace dagui
{
    class DAGUI_API OpenGLRenderer : public Renderer
    {
    public:
        void generateTextureCoordinates(ImageDef& imageDef, BinImageDef& binImageDef) override;

    };
}