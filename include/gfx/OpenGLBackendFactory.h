//
// Created by Tony Horrobin on 19/07/2025.
//

#include "config/Export.h"

#include "core/GraphicsBackendFactory.h"

namespace dagui
{
    class DAGUI_API OpenGLBackendFactory : public GraphicsBackendFactory
    {
    public:
        OpenGLBackendFactory() = default;

        MeshBackend* createMesh(Mesh * mesh) override;

        TextureAtlasBackend* createTextureAtlas(TextureAtlas* atlas) override;
    };
}