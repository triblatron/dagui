//
// Created by Tony Horrobin on 02/02/2025.
//
#include "config/config.h"

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#endif // __APPLE__
#include <glut.h>
#include <iostream>
#include <ostream>

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH/* | GLUT_3_2_CORE_PROFILE*/);
    glutCreateWindow("OpenGL Renderer");
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}