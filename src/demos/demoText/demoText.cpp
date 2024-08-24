//
// Created by tony on 24/08/24.
//
#include "config/config.h"

#include <GL/glut.h>

#include <iostream>

void onReshape(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glViewport(0,0,width,height);
}

void onDisplay()
{
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glBegin(GL_QUADS);
    glColor3f(1.0f,1.0f,1.0f);
    glVertex2d(-0.5, -0.5);
    glVertex2d(0.5,-0.5);
    glVertex2d(0.5,0.5);
    glVertex2d(-0.5,0.5);
    glEnd();
    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(512,512);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
    glutCreateWindow("demoText");
    glutReshapeFunc(onReshape);
    glutDisplayFunc(onDisplay);
    glutMainLoop();
}
