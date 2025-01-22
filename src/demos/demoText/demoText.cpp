//
// Created by tony on 24/08/24.
//
#include "config/config.h"
#include <cstdlib>
#if defined(HAVE_WINDOWS_H)
#include <Windows.h>
#endif
#if defined(__linux__) || defined(_WIN32)
#include <GL/glut.h>
#else
#include <glut.h>
#endif // __linux__ || _WIN32
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <cstdint>

#include "../../../ThirdParty/dagbase/include/core/ConfigurationElement.h"
#include "core/BinPackingStrategy.h"
#include "core/BinPackingStrategyFactory.h"
#include "gfx/Image.h"
#include "gfx/FontImageSource.h"
#include "gfx/TextureAtlas.h"
#include "gfx/OpenGLRenderer.h"
#include "core/LuaInterface.h"
#include "core/ConfigurationElement.h"
static dagui::Image* texImage = new dagui::Image(512,512,4);
static GLuint texName = 0;
static FT_Library  library{nullptr};
static dagui::FontImageSource* source{nullptr};
static FT_Face     face{nullptr};      /* handle to face object */
static dagui::TextureAtlas atlas;//(512, 512, 1);
static dagui::OpenGLRenderer renderer;
static std::string text;

void copyGlyphToImage(FT_GlyphSlot glyph, dagui::Image* image)
{
    for (auto row = 0U; row<glyph->bitmap.rows; ++row)
    {
        for (auto col=0U; col<glyph->bitmap.width; ++col)
        {
            auto intensity = glyph->bitmap.buffer[row*glyph->bitmap.width+col];

            image->set(glyph->bitmap.rows - 1 - row, col, intensity, intensity, intensity);
        }
    }
}

void createImage(dagui::Image* image)
{
    for (size_t row=0; row<image->height(); ++row)
    {
        for (size_t col=0; col<image->width(); ++col)
        {
            // Should be yellow
            image->set(row,col,255,255,0);
        }
    }
    FT_GlyphSlot slot = face->glyph;
    copyGlyphToImage(slot, image);
}


void init()
{
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    // createImage(texImage);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glGenTextures(1,&texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texImage->width(), texImage->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, texImage->data());
}

void onReshape(int width, int height)
{
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void onDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, texName);

    glMatrixMode(GL_MODELVIEW);
    renderer.drawText(face, atlas, text);
    // glBegin(GL_QUADS);
    // glColor3f(1.0f,1.0f,1.0f);
    // glTexCoord2d(0.0, 0.0);
    // glVertex2d(0.0, 0.0);
    // glTexCoord2d(1.0, 0.0);
    // glVertex2d(512.0,0.0);
    // glTexCoord2d(1.0,1.0);
    // glVertex2d(512.0,512.0);
    // glTexCoord2d(0.0,1.0);
    // glVertex2d(0.0,512.0);
    // glEnd();
    glutSwapBuffers();
    glDisable(GL_TEXTURE_2D);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <fontFilename> <text>\n";

        return -1;
    }
    text = argv[2];
    int error = FT_Init_FreeType( &library );
    if ( error )
    {
        std::cerr << "Error initialising FreeType library, bailing\n";
        return -1;

    }
    source = new dagui::FontImageSource(library);
    dagbase::Lua lua;
    auto sourceConfig = dagbase::ConfigurationElement::fromFile(lua, argv[1]);
    if (!sourceConfig)
    {
        std::cerr << "Failed to configure font image source\n";

        return -1;
    }
    source->configure(*sourceConfig);
    face = source->face();
    dagui::BinPackingStrategyFactory factory;

    auto binPacking = factory.createStrategy("MaxRects");
    auto atlasConfigStr = "root = { width=512, height=512, numComponents=3 }";
    auto atlasConfig = dagbase::ConfigurationElement::fromString(lua, atlasConfigStr);
    atlas.configure(*atlasConfig);
    atlas.setImageSource(source);
    atlas.pack(*binPacking);
    texImage = atlas.binImage();
    //binPacking->pack(*source, atlas);
    // if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
    // {
    //     std::cout << "Rendering glyph\n";
    //
    //     error = FT_Render_Glyph( face->glyph,   /* glyph slot  */
    //                              FT_RENDER_MODE_NORMAL ); /* render mode */
    //     if (error)
    //     {
    //         std::cerr << "Failed to render glyph, bailing\n";
    //
    //         return -6;
    //     }
    // }
    glutInit(&argc, argv);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(512,512);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
    glutCreateWindow("demoText");
    init();
    glutReshapeFunc(onReshape);
    glutDisplayFunc(onDisplay);
    glutMainLoop();
}
