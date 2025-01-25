//
// Created by Tony Horrobin on 25/01/2025.
//
#include "config/config.h"

#include "gfx/RendererFactory.h"
#include "gfx/OpenGLRenderer.h"

namespace dagui
{
    Renderer* RendererFactory::createRenderer(std::string_view apiName, const APIVersion& version)
    {
        if (apiName == "OpenGL")
        {
            return new OpenGLRenderer();
        }
        return nullptr;
    }

}