//
// Created by tony on 24/08/25.
//

#pragma once

#include "config/Export.h"

#include "gfx/TextureAtlasBackend.h"

#include <cstdint>

namespace dagui
{
    class DAGUI_API OpenGLTextureAtlas : public TextureAtlasBackend
    {
    public:
        void allocate() override;

        void bind() override;

        void setParameters() override;

        void upload(Image& image) override;

        void unbind() override;
    private:
        std::uint32_t _handle{0};
        bool _allocated{false};
        bool _haveSetParameters{false};
        bool _haveUploaded{false};
    };
}