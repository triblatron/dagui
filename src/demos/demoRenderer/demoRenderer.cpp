//
// Created by Tony Horrobin on 02/02/2025.
//
#include "config/config.h"

#if defined(__APPLE__)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#elif defined(__linux__) || defined(_WIN32)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif // __APPLE__
#include <iostream>
#include <ostream>

#include "core/LuaInterface.h"
#include "gfx/GenericMesh.h"
#include "gfx/OpenGLMesh2D.h"
#include "gfx/OpenGLRenderer.h"
#include "util/glmOutput.h"
#include "core/Rectangle.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

GLFWwindow* window = nullptr;
dagui::gl::VertexBuffer vertexBuffer;
dagui::gl::VertexBuffer vertexBuffer2;
dagui::gl::IndexBuffer indexBuffer;

struct Vertex
{
    float x{0.0f};
    float y{0.0f};
    float r{0.0f};
    float g{0.0f};
    float b{0.0f};
    float a{0.0f};

    void configure(dagbase::ConfigurationElement& config)
    {
        dagbase::ConfigurationElement::readConfig(config, "x", &x);
        dagbase::ConfigurationElement::readConfig(config, "y", &y);
        dagbase::ConfigurationElement::readConfig(config, "r", &r);
        dagbase::ConfigurationElement::readConfig(config, "g", &g);
        dagbase::ConfigurationElement::readConfig(config, "b", &b);
        dagbase::ConfigurationElement::readConfig(config, "a", &a);
    }
};
dagui::ShapeMesh    mesh;

dagui::GenericAttributeArray<Vertex>* a = new dagui::GenericAttributeArray<Vertex>();
GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,  // Bottom-left
     0.5f, -0.5f, 0.0f,  // Bottom-right
     0.0f,  0.5f, 0.0f   // Top
};

void onReshape(int width, int height)
{
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display(dagui::Renderer& renderer)
{
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 0.0f);
    dagui::Rectangle rect;
    rect.setSize(1.0f,0.5f);
    renderer.drawRect(rect);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f);
    glEnd();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    vertexBuffer.draw(GL_TRIANGLES, 0, a->size());
    glColor3f(1.0f, 0.0f, 1.0f);
    vertexBuffer2.bind();
    vertexBuffer2.setPointers();
    //vertexBuffer2.draw(GL_TRIANGLES, 0, mesh.attributeArray(0)->size());
    //std::cout << glGetError() << std::endl;
//    mesh.unbind();
    indexBuffer.draw(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glfwSwapBuffers(window);
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char** argv)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << '\n';

        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    // glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH/* | GLUT_3_2_CORE_PROFILE*/);
    window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!window)
    {
        // Window or OpenGL context creation failed
        std::cerr << "Failed to open GLFW window." << '\n';

        glfwTerminate();
        return -1;
    }
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    GLenum err = glewInit();

    if (GLEW_OK != err)
    {
        std::cerr << "Failed to init GLEW with error: " << glewGetErrorString(err) << ", bailing\n";

        return -1;
    }
    dagui::ArrayDescriptor descriptor;
    {
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
    }
    {
        dagbase::Lua lua;
        auto config = dagbase::ConfigurationElement::fromFile(lua, "data/tests/demoRenderer/Mesh.lua");
        if (!config)
        {
            std::cerr << "Failed to load mesh config, bailing\n";

            return -1;
        }
        mesh.configure(*config);
        vertexBuffer2.setArray(mesh.attributeArray(0));
        vertexBuffer2.allocate();
        vertexBuffer2.submit();
        indexBuffer.setArray(mesh.indexArray());
        indexBuffer.allocate();
        indexBuffer.submit();
    }
    std::cout << "sizeof(vertices): " << sizeof(vertices) << std::endl;
    //std::cout << glGetError() << std::endl;
    glm::mat4 model = glm::perspective(glm::radians(45.0),16.0/9.0, 0.1, 1000.0);
    std::cout << "model: " << model << std::endl;
    dagui::OpenGLRenderer renderer;
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float) height;

        glViewport(0, 0, width, height);

        display(renderer);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
