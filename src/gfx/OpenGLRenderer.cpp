//
// Created by Tony Horrobin on 25/12/2024.
//

#include "config/config.h"

#include "gfx/OpenGLRenderer.h"

#include "core/Vec2f.h"
#include "gfx/BinImageDef.h"
#include "gfx/ImageDef.h"
#include "gfx/Mesh2D.h"
#include "gfx/TextureAtlas.h"
#include "gfx/OpenGL.h"
#include "gfx/DrawingCommand.h"
#include "core/DrawCommandBuffer.h"
#include "core/DrawCommand.h"
#include "core/Rectangle.h"
#include "core/Mesh.h"

namespace dagui
{
    void OpenGLRenderer::drawTexturedQuad(float x, float y, const ImageDef* imageDef)
    {
        glTexCoord2f(imageDef->p0.x, imageDef->p0.y);
        glVertex2f(x,y);
        glTexCoord2f(imageDef->p1.x, imageDef->p1.y);
        glVertex2f(x+imageDef->width,y);
        glTexCoord2f(imageDef->p2.x, imageDef->p2.y);
        glVertex2f(x+imageDef->width,y+imageDef->height);

        glTexCoord2f(imageDef->p2.x, imageDef->p2.y);
        glVertex2f(x+imageDef->width,y+imageDef->height);
        glTexCoord2f(imageDef->p3.x, imageDef->p3.y);
        glVertex2f(x,y+imageDef->height);
        glTexCoord2f(imageDef->p0.x, imageDef->p0.y);
        glVertex2f(x,y);
    }

    OpenGLRenderer::OpenGLRenderer()
    {
        _version.major = 1;
        _version.minor = 1;
        _version.patch = 0;
    }

    void OpenGLRenderer::configure(dagbase::ConfigurationElement& config)
    {
        if (auto element = config.findElement("commandClass"); element != nullptr)
        {
            auto commandClass = element->asString();
            if (commandClass == "DrawArrays")
            {
                _command = new gl::DrawArrays();
                _command->configure(*element);
            }
        }
    }

    dagbase::ConfigurationElement::ValueType OpenGLRenderer::find(std::string_view path) const
    {
        dagbase::ConfigurationElement::ValueType retval;

        retval = dagbase::findEndpoint(path, "numCommands", _command!=nullptr?std::int64_t(1):std::int64_t(0));
        if (retval.has_value())
            return retval;

        return {};
    }

    void OpenGLRenderer::drawText(FT_FaceRec_* face, TextureAtlas& atlas, std::string_view text)
    {
        float x = 0, y = 0;
        glBegin(GL_TRIANGLES);
        // For each glyph in the string
        for (std::size_t i = 0; i < text.size(); i++)
        {
            FT_UInt glyphIndex = FT_Get_Char_Index(face, text[i]);
            ImageDef* imageDef = atlas.imageForGlyphIndex(glyphIndex);
            if (imageDef)
            {
                generateTextureCoordinates(*imageDef, *atlas.binImageDef());
                drawTexturedQuad(x, y, imageDef);
                x+=imageDef->advance();
            }
        }
        // Get the texture coordinates for the glyph
        glEnd();
    }

    void OpenGLRenderer::drawMesh2D(const Mesh2D& mesh)
    {
        switch (mesh.primitiveType())
        {
        case Mesh2D::PRIMITIVE_TRIANGLE:
            if (_gl)
            {
                gl::Layout vertexLayout;
                vertexLayout.stride=0;
                vertexLayout.dataType = GL_FLOAT;
                vertexLayout.numComponentsPerAttr = 3;
                gl::VertexBuffer vertices;
                vertices.setLayout(vertexLayout);
                vertices.setData(mesh.vertices(), mesh.numVertices());
                _gl->drawArray(vertices);
            }
            break;
        }
    }

    void OpenGLRenderer::generateTextureCoordinates(ImageDef& imageDef, BinImageDef& binImageDef)
    {
        imageDef.p0.x = float(imageDef.x)/float(binImageDef.width);
        imageDef.p0.y = float(imageDef.y)/float(binImageDef.height);
        imageDef.p1.x = float(imageDef.x+imageDef.width)/float(binImageDef.width);
        imageDef.p1.y = float(imageDef.y)/float(binImageDef.height);
        imageDef.p2.x = float(imageDef.x+imageDef.width)/float(binImageDef.width);
        imageDef.p2.y = float(imageDef.y+imageDef.height)/float(binImageDef.height);
        imageDef.p3.x = float(imageDef.x)/float(binImageDef.width);
        imageDef.p3.y = float(imageDef.y+imageDef.height)/float(binImageDef.height);
    }

    void OpenGLRenderer::makeItSo(DrawCommandBuffer &buffer)
    {
        buffer.eachCommand([this](DrawCommand* cmd) {
            cmd->makeItSo(*this);
        });
    }

    void OpenGLRenderer::drawRect(const Rectangle &rect)
    {
        glBegin(GL_QUADS);
        glVertex2f(rect.x(), rect.y());
        glVertex2f( rect.x() + rect.width(), rect.y());
        glVertex2f(rect.x() + rect.width(), rect.y() + rect.height());
        glVertex2f(rect.x(), rect.y() + rect.height());
        glEnd();
    }

    void OpenGLRenderer::drawMesh(Mesh &mesh)
    {
        mesh.backend()->draw();
    }
}
