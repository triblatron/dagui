//
// Created by Tony Horrobin on 28/01/2025.
//
#include "config/config.h"

#include "gfx/OpenGL.h"

namespace dagui
{
    namespace gl
    {
        void VertexBuffer::bind()
        {
            glBindBuffer(GL_ARRAY_BUFFER,_handle);
        }

        void VertexBuffer::unbind()
        {
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }

        void Buffer::allocate()
        {
            glGenBuffers(1, &_handle);
        }

        void IndexBuffer::bind()
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_handle);
        }

        void IndexBuffer::unbind()
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        }
    }
}