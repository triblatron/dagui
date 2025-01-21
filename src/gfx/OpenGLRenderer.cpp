//
// Created by Tony Horrobin on 25/12/2024.
//

#include "config/config.h"

#include "gfx/OpenGLRenderer.h"

#include "core/Vec2f.h"
#include "gfx/BinImageDef.h"
#include "gfx/ImageDef.h"
#include "gfx/TextureAtlas.h"

#if defined(__linux__) || defined(_WIN32)
#include <GL/glut.h>
#else
#include <glut.h>
#endif // __linux__ || _WIN32

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

    void OpenGLRenderer::drawText(FT_FaceRec_* face, TextureAtlas& atlas, std::string_view text)
    {
        float x = 0, y = 0;
        // TODO:Generate a list of triangles for the given ASCII string.
        glBegin(GL_TRIANGLES);
        // For each glyph in the string
        for (std::size_t i = 0; i < text.size(); i++)
        {
            FT_UInt glyphIndex = FT_Get_Char_Index(face, text[i]);
            ImageDef* imageDef = atlas.imageForGlyphIndex(glyphIndex);
            generateTextureCoordinates(*imageDef, *atlas.binImageDef());
            drawTexturedQuad(x, y, imageDef);
            x+=imageDef->advance();
        }
        // Get the texture coordinates for the glyph
        glEnd();
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
}
