//
// Created by Tony Horrobin on 02/02/2025.
//
#include "config/config.h"

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#include <glut.h>
#elif defined(__linux__) || defined(_WIN32)
#include <GL/gl3.h>
#include <GL/glut.h>
#endif // __APPLE__
#include <iostream>
#include <ostream>

#include "gfx/OpenGLMesh2D.h"

dagui::gl::OpenGLMesh2D mesh;

GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,  // Bottom-left
     0.5f, -0.5f, 0.0f,  // Bottom-right
     0.0f,  0.5f, 0.0f   // Top
};

void onReshape(int width, int height)
{
    // glViewport(0,0,width,height);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // gluOrtho2D(0.0, width, 0.0, height);
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
}

void display()
{
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f);
    glEnd();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    mesh.draw();
    std::cout << glGetError() << std::endl;
//    mesh.unbind();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH/* | GLUT_3_2_CORE_PROFILE*/);
    glutCreateWindow("OpenGL Renderer");
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    mesh.addVertex({-1.0f, -1.0f});
    mesh.addVertex({0.0f, -1.0f});
    mesh.addVertex({0.0f, 0.0f});
    mesh.addColour(1.0f, 0.0f, 0.0f, 1.0f);
    mesh.addColour(0.0f, 1.0f, 0.0f, 1.0f);
    mesh.addColour(0.0f, 0.0f, 1.0f, 1.0f);
    mesh.allocate();
    mesh.submit();
    std::cout << "sizeof(vertices): " << sizeof(vertices) << std::endl;
    std::cout << glGetError() << std::endl;
    glutReshapeFunc(onReshape);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
