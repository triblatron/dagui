#include "node_editor.h"
#include "imnodes.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#if defined(__APPLE__)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#elif defined(__linux__) || defined(_WIN32)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif // __APPLE__
#include <cstdio>
#include <iostream>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error:%d(%s)\n", error, description);
}

namespace example
{
    namespace
    {
        class HelloWorldNodeEditor
        {
        public:
            void show()
            {
                ImGui::Begin("simple node editor");

                ImNodes::BeginNodeEditor();
                ImNodes::BeginNode(1);

                ImNodes::BeginNodeTitleBar();
                ImGui::TextUnformatted("simple node :)");
                ImNodes::EndNodeTitleBar();

                ImNodes::BeginInputAttribute(2);
                ImGui::Text("input");
                ImNodes::EndInputAttribute();

                ImNodes::BeginOutputAttribute(3);
                ImGui::Indent(40);
                ImGui::Text("output");
                ImNodes::EndOutputAttribute();

                ImNodes::EndNode();
                ImNodes::EndNodeEditor();

                ImGui::End();
            }
        };

        static HelloWorldNodeEditor editor;
    } // namespace

    void NodeEditorInitialize() { ImNodes::SetNodeGridSpacePos(1, ImVec2(200.0f, 200.0f)); }

    void NodeEditorShow() { editor.show(); }

    void NodeEditorShutdown() {}

} // namespace example
int main()
{
    IMGUI_CHECKVERSION();
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << '\n';

        return -1;
    }
    auto context = ImGui::CreateContext();
    ImNodes::CreateContext();
    example::NodeEditorInitialize();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    auto window = glfwCreateWindow(640, 480, "ImGUI demo", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    // Setup Dear ImGui context

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        example::NodeEditorShow();
        //ImGui::ShowDemoWindow();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    example::NodeEditorShutdown();
    ImNodes::DestroyContext();
    ImGui::DestroyContext();
    return 0;
}