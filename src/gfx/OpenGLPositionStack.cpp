//
// Created by Tony Horrobin on 26/08/2025.
//

#include "config/config.h"

#include "gfx/OpenGLPositionStack.h"

#include <GL/glew.h>

namespace dagui
{
    void OpenGLPositionStack::push()
    {
        glPushMatrix();
    }

    void OpenGLPositionStack::translate(float x, float y)
    {
        glTranslatef(x,y,0.0f);
    }

    void OpenGLPositionStack::pop()
    {
        glPopMatrix();
    }

    glm::vec2 OpenGLPositionStack::top() const
    {
        return _translation;
    }
}