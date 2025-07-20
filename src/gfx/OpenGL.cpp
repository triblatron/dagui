//
// Created by Tony Horrobin on 28/01/2025.
//
#include "config/config.h"

#include "gfx/OpenGL.h"
#include "gfx/AttributeArray.h"
#include "gfx/IndexArray.h"

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

        void VertexBuffer::draw(GLenum mode, GLint first, GLsizei count)
        {
            bind();
            setPointers();
            if (_a)
                glDrawArrays(mode, first, count);
            }

        void VertexBuffer::setPointers()
        {
            if (_a)
            {
                std::size_t elementSize = _a->elementSize();
                for (auto i = 0; i < _a->desciptor().attributes.size(); ++i)
                {
                    const AttributeLayout &layout = _a->desciptor().attributes[i];
                    switch (layout.attr.usage)
                    {
                        case AttributeDescriptor::USAGE_POSITION:
                            glVertexPointer(layout.attr.numComponents, dataTypeToGL(layout.attr.dataType), elementSize,
                                            (void *) layout.offset);
                            break;
                        case AttributeDescriptor::USAGE_COLOUR:
                            glColorPointer(layout.attr.numComponents, dataTypeToGL(layout.attr.dataType), elementSize,
                                           (void *) layout.offset);
                            break;
                    }
                }
            }
        }
        //std::cout << glGetError() << std::endl;

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
            case dagui::AttributeDescriptor::TYPE_UINT16:
                return GL_UNSIGNED_SHORT;
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

        void IndexBuffer::submit()
        {
            bind();
            if (_a)
            {
                auto size = _a->elementSize()*_a->size();
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, _a->data(), GL_STATIC_DRAW);
            }
        }

        IndexBuffer::IndexBuffer(const IndexArray *a)
        :
        _a(a)
        {
            // Do nothing.
        }

        void IndexBuffer::draw(GLenum mode)
        {
            if (_a)
            {
                glDrawElements(mode, _a->size(), dataTypeToGL(_a->descriptor().dataType), nullptr);
            }
        }
    }
}