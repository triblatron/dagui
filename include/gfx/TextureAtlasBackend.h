//
// Created by Tony Horrobin on 23/08/2025.
//

#pragma once

#include "config/Export.h"

namespace dagui
{
    class Image;

    class DAGUI_API TextureAtlasBackend
    {
    public:
        virtual ~TextureAtlasBackend() = default;

        virtual void allocate() = 0;

        virtual void bind() = 0;

        virtual void setParameters() = 0;

        virtual void upload(Image& image) = 0;
    };
}