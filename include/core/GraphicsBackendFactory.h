//
// Created by Tony Horrobin on 19/07/2025.
//

#pragma once

#include "config/Export.h"

namespace dagui
{
    class Mesh;
    class MeshBackend;

    class DAGUI_API GraphicsBackendFactory
    {
    public:
        virtual ~GraphicsBackendFactory() = default;

        virtual MeshBackend* createMesh(Mesh* mesh) = 0;
    };
}
