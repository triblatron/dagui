//
// Created by Tony Horrobin on 02/02/2025.
//
#include "config/config.h"

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#include <glut.h>
#elif defined(__linux__) || defined(_WIN32)
#if defined(__linux__)
#include <GL/gl3.h>
#endif
#include <GL/glew.h>
#include <GL/glut.h>
#endif // __APPLE__
#include <iostream>
#include <ostream>

#include "core/LuaInterface.h"
#include "gfx/GenericMesh2D.h"
#include "gfx/OpenGLMesh2D.h"
#include "gfx/OpenGLRenderer.h"

dagui::GenericMesh2D mesh;
dagui::gl::VertexBuffer vertexBuffer;
struct Vertex
{
    float x{0.0f};
    float y{0.0f};
    float r{0.0f};
    float g{0.0f};
    float b{0.0f};
    float a{0.0f};
};

dagui::GenericAttributeArray<Vertex>* a = new dagui::GenericAttributeArray<Vertex>();
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

    vertexBuffer.draw(GL_TRIANGLES, 0, a->size());
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
    GLenum err = glewInit();

    if (GLEW_OK != err)
    {
        std::cerr << "Failed to init GLEW with error: " << glewGetErrorString(err) << ", bailing\n";

        return -1;
    }
    dagui::ArrayDescriptor descriptor;
    dagbase::Lua lua;
    auto config = dagbase::ConfigurationElement::fromFile(lua, "data/tests/demoRenderer/Vertex.lua");
    descriptor.configure(*config);
    a->setDescriptor(descriptor);
    a->addVertex({-1.0f, -1.0, 1.0f, 0.0f, 0.0f, 1.0f});
    a->addVertex({0.0f, -1.0, 0.0f, 1.0f, 0.0f, 1.0f});
    a->addVertex({0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f});
    vertexBuffer.setArray(a);
    vertexBuffer.allocate();
    vertexBuffer.submit();
    std::cout << "sizeof(vertices): " << sizeof(vertices) << std::endl;
    std::cout << glGetError() << std::endl;
    glutReshapeFunc(onReshape);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
