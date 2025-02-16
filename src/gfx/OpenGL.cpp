//
// Created by Tony Horrobin on 28/01/2025.
//
#include "config/config.h"

#include "gfx/OpenGL.h"
#include "gfx/AttributeArray.h"

#include <iostream>

namespace dagui
{
    namespace gl
    {
        VertexBuffer::VertexBuffer(const AttributeArray* a)
            :
        _a(a)
        {
            // Do nothing.
        }

        void VertexBuffer::bind()
        {
            glBindBuffer(GL_ARRAY_BUFFER,_handle);
        }

        void VertexBuffer::unbind()
        {
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }

        void VertexBuffer::submit()
        {
            bind();
            if (_a)
                glBufferData(GL_ARRAY_BUFFER, _a->elementSize()*_a->size(), _a->data(), GL_STATIC_DRAW);
        }

        void VertexBuffer::draw()
        {
            bind();
            if (_a)
            {
                glVertexPointer(_a->desciptor().attributes[0].attr.numComponents, dataTypeToGL(_a->desciptor().attributes[0].attr.dataType), _a->elementSize(), (void*)_a->desciptor().attributes[0].offset);
                glColorPointer(_a->desciptor().attributes[1].attr.numComponents, dataTypeToGL(_a->desciptor().attributes[1].attr.dataType), _a->elementSize(), (void*)_a->desciptor().attributes[1].offset);
                glDrawArrays(GL_TRIANGLES, 0, _a->size());
            }
            std::cout << glGetError() << std::endl;
        }

        void Buffer::allocate()
        {
            glGenBuffers(1, &_handle);
        }

        GLenum Buffer::dataTypeToGL(AttributeDescriptor::DataType dataType)
        {
            switch (dataType)
            {
            case dagui::AttributeDescriptor::TYPE_UNKNOWN:
                return GL_NONE;
            case dagui::AttributeDescriptor::TYPE_BYTE:
                return GL_UNSIGNED_BYTE;
            case dagui::AttributeDescriptor::TYPE_INT32:
                return GL_INT;
            case dagui::AttributeDescriptor::TYPE_UINT32:
                return GL_UNSIGNED_INT;
            case dagui::AttributeDescriptor::TYPE_FLOAT:
                return GL_FLOAT;
            case dagui::AttributeDescriptor::TYPE_DOUBLE:
                return GL_DOUBLE;
            }

            return GL_NONE;
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