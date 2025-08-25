//
// Created by tony on 24/08/25.
//

#include "config/config.h"

#include "gfx/OpenGLTextureAtlas.h"
#include "gfx/Image.h"

#include <GL/glew.h>

namespace dagui
{
    void OpenGLTextureAtlas::allocate()
    {
        if (!_allocated)
        {
            glGenTextures(1, &_handle);
            _allocated = true;
        }
    }

    void OpenGLTextureAtlas::bind()
    {
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBindTexture(GL_TEXTURE_2D, _handle);
    }

    void OpenGLTextureAtlas::setParameters()
    {
        if (!_haveSetParameters)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            _haveSetParameters = true;
        }
    }

    void OpenGLTextureAtlas::upload(Image& texImage)
    {
        if (!_haveUploaded)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texImage.width(), texImage.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, texImage.data());

            _haveUploaded = true;
        }
    }

    void OpenGLTextureAtlas::unbind()
    {
        glDisable(GL_TEXTURE_2D);
    }
}
