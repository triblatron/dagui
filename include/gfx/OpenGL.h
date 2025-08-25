

//
// Created by Tony Horrobin on 28/01/2025.
//

#pragma once

#include "config/Export.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#if defined(_WIN32) || defined(__linux__)
#include <GL/glew.h>
#elif defined(__APPLE__)
#include <OpenGL/gl.h>
#endif
#include <cstdint>

#include "AttributeDescriptor.h"

namespace dagui
{
    class AttributeArray;
    class IndexArray;

    namespace gl
    {
        struct DAGUI_API Layout
        {
            GLuint numComponentsPerAttr{0};
            GLenum dataType{0};
            GLuint stride{0};
        };

        class DAGUI_API Buffer
        {
        public:
            void setLayout(const Layout& layout)
            {
                _layout = layout;
            }

            void setData(const void* data, unsigned int size)
            {
                _data = data;
                _size = size;
            }

            unsigned int numElements() const
            {
                return _size;
            }

            void allocate();

            static GLenum dataTypeToGL(AttributeDescriptor::DataType dataType);
        protected:
            std::uint32_t _handle{0};
        private:
            Layout _layout;
            const void* _data{nullptr};
            unsigned int _size{0};
        };

        class DAGUI_API VertexBuffer : public Buffer
        {
        public:
            VertexBuffer() = default;
            explicit VertexBuffer(const AttributeArray* a);
            void setArray(const AttributeArray* a)
            {
                _a = a;
            }
            void bind();
            void unbind();
            void submit();
            void setPointers();
            void draw(GLenum mode, GLint first, GLsizei count);
        private:
            const AttributeArray* _a{nullptr};
        };

        class DAGUI_API IndexBuffer : public Buffer
        {
        public:
            IndexBuffer() = default;
            explicit IndexBuffer(const IndexArray* a);

            void setArray(const IndexArray* a)
            {
                _a = a;
            }
            void bind();
            void unbind();
            void submit();
            void draw(GLenum mode);
        private:
            const IndexArray* _a{nullptr};
        };

        class DAGUI_API OpenGL
        {
        public:
            virtual ~OpenGL() = default;

            virtual void drawArray(const VertexBuffer& vertices) = 0;
            virtual void drawElements(const VertexBuffer& vertices, const IndexBuffer& indices) = 0;
        };
    }
}
