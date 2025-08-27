//
// Created by Tony Horrobin on 27/08/2025.
//

#include "config/config.h"

#include "gfx/OpenGLBlending.h"

#include <GL/glew.h>

namespace dagui
{
    void dagui::OpenGLBlending::makeItSo()
    {
        glBlendFunc(mapFactorToGL(_srcFactor), mapFactorToGL(_destFactor));
    }

    std::int32_t OpenGLBlending::mapFactorToGL(Factor factor)
    {
        switch (factor)
        {
            case Blending::FACTOR_UNKNOWN:
                return GL_ONE;
            case Blending::FACTOR_SRC_ALPHA:
                return GL_SRC_ALPHA;
            case Blending::FACTOR_ONE_MINUS_SRC_ALPHA:
                return GL_ONE_MINUS_SRC_ALPHA;
        }

        return GL_ONE;
    }

    void OpenGLBlending::enable()
    {
        glEnable(GL_BLEND);
    }

    void OpenGLBlending::disable()
    {
        glDisable(GL_BLEND);
    }
}