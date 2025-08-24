//
// Created by tony on 24/08/25.
//

#include "config/config.h"

#include "gfx/OpenGLTextureAtlas.h"

namespace dagui
{
    void OpenGLTextureAtlas::allocate()
    {
        glGenTextures(1, &_handle);
    }

    void OpenGLTextureAtlas::bind()
    {
        glBindTexture(GL_TEXTURE_2D, _handle);
    }

    void OpenGLTextureAtlas::setParameters()
    {
    }

    void OpenGLTextureAtlas::upload(Image& image)
    {
    }
}
