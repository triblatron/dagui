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
#include "core/ShapeFactory.h"
#include "gfx/OpenGLMesh.h"
#include "gfx/OpenGLBackendFactory.h"
#include "core/WidgetFactory.h"
#include "core/Widget.h"
#include "core/Batcher.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

GLFWwindow* window = nullptr;
dagui::gl::VertexBuffer vertexBuffer;
dagui::gl::VertexBuffer vertexBuffer2;
dagui::gl::IndexBuffer indexBuffer;
dagui::OpenGLRenderer renderer;
dagui::Rectangle rect;
dagui::MeshBackend* backend = nullptr;

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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
    glColor3f(1.0f, 0.0f, 1.0f);
    vertexBuffer.draw(GL_TRIANGLES, 0, a->size());
    backend->draw();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glfwSwapBuffers(window);
}
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error:%d(%s)\n", error, description);
}

int main(int argc, char* argv[])
{
    glfwSetErrorCallback(error_callback);
    if (argc != 2)
    {
        std::cerr << "Usage " << argv[0] << " <meshConfigFilename>\n";

        return -1;
    }
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << '\n';

        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
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
        if (!config)
        {
            std::cerr << "Failed to load vertex config, bailing\n";

            return -1;
        }
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
        auto config = dagbase::ConfigurationElement::fromFile(lua, argv[1]);
        if (!config)
        {
            std::cerr << "Failed to load mesh config, bailing\n";

            return -1;
        }
        mesh.configure(*config);
    }
    dagui::Shape* shape = nullptr;
    {
        dagbase::Lua lua;
        auto rectConfig = dagbase::ConfigurationElement::fromFile(lua, "data/tests/demoRenderer/RoundedRectangle.lua");
        if (!rectConfig)
        {
            std::cerr << "Failed to load rectangle config, bailing\n";

            return -1;
        }
        dagui::ShapeFactory shapeFactory;
        shape = shapeFactory.createShape(*rectConfig);
        if (!shape)
        {
            std::cerr << "Failed to create Shape, bailing\n";

            return -1;
        }
    }
    shape->tessellate(mesh);
    dagui::Widget* widgetTree = nullptr;
    {
        dagbase::Lua lua;
        auto widgetConfig = dagbase::ConfigurationElement::fromFile(lua, "data/tests/WidgetFactory/rootWithLabel.lua");
        if (!widgetConfig)
        {
            std::cerr << "Failed to load widget config, bailing\n";

            return -1;
        }
        dagui::WidgetFactory widgetFactory;
        dagui::ShapeFactory shapeFactory;
        widgetTree = widgetFactory.create(*widgetConfig, shapeFactory);
        if (!widgetTree)
        {
            std::cerr << "Failed to create Widget tree, bailing\n";

            return -1;
        }
    }
    dagui::Batcher batcher;
    {
        dagbase::Lua lua;
        auto batcherConfig = dagbase::ConfigurationElement::fromFile(lua, "data/tests/Batcher/ShapeMesh.lua");
        if (!batcherConfig)
        {
            std::cout << "Failed to load batcher config, bailing\n";

            return -1;
        }
        batcher.configure(*batcherConfig);
    }
    dagui::OpenGLBackendFactory factory;
    widgetTree->draw(batcher, factory);

    backend = factory.createMesh(&mesh);
    mesh.setBackend(backend);
    mesh.allocateBuffers();
    mesh.sendToBackend();
    std::cout << "sizeof(vertices): " << sizeof(vertices) << std::endl;
    //std::cout << glGetError() << std::endl;
    glm::mat4 model = glm::perspective(glm::radians(45.0),16.0/9.0, 0.1, 1000.0);
    std::cout << "model: " << model << std::endl;
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);

        display(renderer);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
