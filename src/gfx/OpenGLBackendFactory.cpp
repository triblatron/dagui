//
// Created by Tony Horrobin on 19/07/2025.
//


#include "config/config.h"

#include "gfx/OpenGLBackendFactory.h"
#include "gfx/OpenGLMesh.h"
#include "gfx/OpenGLTextureAtlas.h"
#include "gfx/OpenGLPositionStack.h"
#include "gfx/OpenGLBlending.h"

namespace dagui
{
    MeshBackend *OpenGLBackendFactory::createMesh(Mesh* mesh)
    {
        return new OpenGLMesh();
    }

    TextureAtlasBackend *OpenGLBackendFactory::createTextureAtlas(TextureAtlas *atlas)
    {
        return new OpenGLTextureAtlas();
    }

    PositionStack *OpenGLBackendFactory::createPositionStack()
    {
        return new OpenGLPositionStack();
    }

    Blending *OpenGLBackendFactory::createBlending()
    {
        return new OpenGLBlending();
    }
}