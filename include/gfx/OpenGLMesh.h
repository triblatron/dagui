//
// Created by Tony Horrobin on 16/07/2025.
//

#include "config/Export.h"

#include "gfx/MeshBackend.h"
#include "gfx/OpenGL.h"

namespace dagui
{
    class Mesh;

    class DAGUI_API OpenGLMesh : public MeshBackend
    {
    public:
        explicit OpenGLMesh(Mesh* mesh);

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
        Mesh* _mesh{nullptr};
        gl::VertexBuffer _vertices;
        gl::IndexBuffer _indices;
    };
}