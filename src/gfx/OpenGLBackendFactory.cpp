//
// Created by Tony Horrobin on 19/07/2025.
//


#include "config/config.h"

#include "gfx/OpenGLBackendFactory.h"
#include "gfx/OpenGLMesh.h"

namespace dagui
{
    MeshBackend *OpenGLBackendFactory::createMesh(Mesh* mesh)
    {
        return new OpenGLMesh(mesh);
    }
}