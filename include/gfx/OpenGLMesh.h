//
// Created by Tony Horrobin on 16/07/2025.
//

#include "config/Export.h"

#include "gfx/MeshBackend.h"
#include "gfx/OpenGL.h"

#include <vector>

namespace dagui
{
    class Mesh;

    class DAGUI_API OpenGLMesh : public MeshBackend
    {
    public:
        explicit OpenGLMesh();

        void addVertexBuffer() override;

        void allocate() override;

        void uploadVertices(AttributeArray& a) override;

        void uploadIndices(IndexArray& a) override;

        void draw() override;

        dagbase::Variant find(std::string_view path) const override;

        std::size_t numVertexBuffers() const
        {
            return 1;
        }
    private:
        using VertexBufferArray = std::vector<gl::VertexBuffer*>;
        VertexBufferArray _vertexBuffers;
        gl::IndexBuffer _indices;
    };
}