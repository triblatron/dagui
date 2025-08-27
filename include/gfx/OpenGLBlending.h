//
// Created by Tony Horrobin on 27/08/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Blending.h"

namespace dagui
{
    class DAGUI_API OpenGLBlending : public Blending
    {
    public:
        void enable() override;

        void makeItSo() override;

        void disable() override;
    private:
        static std::int32_t mapFactorToGL(Factor factor);
    };
}