//
// Created by tony on 24/08/24.
//
#include "config/config.h"

#include <GL/glut.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <cstdint>


class Image
{
public:
    Image(size_t width, size_t height, size_t numComponents)
    :
    _width(width),
    _height(height),
    _numComponents(numComponents)
    {
        _buffer = new GLubyte[height*width*numComponents];
    }

    ~Image()
    {
        delete [] _buffer;
    }

    size_t width() const
    {
        return _width;
    }

    size_t height() const
    {
        return _height;
    }

    void set(size_t row, size_t col, std::uint8_t red, std::uint8_t green, std::uint8_t blue)
    {
        if (row < _height && col < _width)
        {
            _buffer[row*_width*_numComponents+col*_numComponents+0] = red;
            _buffer[row*_width*_numComponents+col*_numComponents+1] = green;
            _buffer[row*_width*_numComponents+col*_numComponents+2] = blue;
            _buffer[row*_width*_numComponents+col*_numComponents+3] = 255;
        }
    }

    GLubyte* data()
    {
        return _buffer;
    }
private:
    size_t _width{0};
    size_t _height{0};
    size_t _numComponents{0};
    GLubyte* _buffer{nullptr};
};

static Image* texImage = new Image(512,512,4);

void createImage(Image* image)
{
    for (size_t row=0; row<image->height(); ++row)
    {
        for (size_t col=0; col<image->width(); ++col)
        {
            // Should be yellow
            image->set(row,col,255,255,0);
        }
    }
}

static GLuint texName;

void init()
{
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    createImage(texImage);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glGenTextures(1,&texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImage->width(), texImage->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage->data());
}

void onReshape(int width, int height)
{
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
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
    glBegin(GL_QUADS);
    glColor3f(1.0f,1.0f,1.0f);
    glTexCoord2d(0.0, 0.0);
    glVertex2d(-0.5, -0.5);
    glTexCoord2d(1.0, 0.0);
    glVertex2d(0.5,-0.5);
    glTexCoord2d(1.0,1.0);
    glVertex2d(0.5,0.5);
    glTexCoord2d(0.0,1.0);
    glVertex2d(-0.5,0.5);
    glEnd();
    glutSwapBuffers();
    glDisable(GL_TEXTURE_2D);
}

int main(int argc, char *argv[])
{
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
