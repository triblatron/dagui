#include "node_editor.h"
#include "imnodes.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "core/TestEditable.h"
#if defined(__APPLE__)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#elif defined(__linux__) || defined(_WIN32)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif // __APPLE__

#include "core/TypeRegistry.h"
#include "core/LuaInterface.h"
#include "imgui/EditorRegistryImGui.h"
#include "core/Editor.h"

#include <cstdio>
#include <iostream>
#include <vector>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error:%d(%s)\n", error, description);
}

namespace propertyeditor
{
    class PropertyEditorDemo
    {
    public:
        PropertyEditorDemo()
        {
            dagbase::Lua lua;
            _config = dagbase::ConfigurationElement::fromFile(lua, "data/tests/EditorRegistry/std_editors.lua");
            if (!_config)
            {
                std::cerr << "Failed to load config for EditorRegistry, bailing\n";

                ImGui::End();
                return;
            }
            registry.configure(*_config);
            _obj = new dagui::TestEditable();
        }

        ~PropertyEditorDemo()
        {
            delete _config;
            delete _obj;
        }

        void show()
        {
            bool open = true;
            // Main body of the Demo window starts here.
            if (!ImGui::Begin("Bool Editor Demo", &open, 0))
            {
                // Early out if the window is collapsed, as an optimization.
                ImGui::End();
                return;
            }

            // dagbase::Type* type = dagbase::TypeRegistry::getTypeRegistry().findType(dagbase::Atom::intern("TestEditable"));
            //
            // if (type)
            // {
            //     auto editor = registry.findOrCreateEditor(*type);
            //     if (editor)
            //     {
            //         editor->setContext(ImGui::GetCurrentContext());
            //         editor->setObject(_obj);
            //         editor->makeItSo();
            //     }
            // }
            _obj->edit(ImGui::GetCurrentContext());
            ImGui::End();
        }
    private:
        dagbase::ConfigurationElement* _config{nullptr};
        dagui::EditorRegistryImGui registry;
        dagui::TestEditable* _obj{nullptr};
    };
}

int main()
{
    IMGUI_CHECKVERSION();
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << '\n';

        return -1;
    }
    ImGui::CreateContext();
    ImNodes::CreateContext();
    example::NodeEditorInitialize();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    auto window = glfwCreateWindow(640, 480, "ImGUI demo", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    // Setup Dear ImGui context

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    propertyeditor::PropertyEditorDemo propertyEditor;   
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        example::NodeEditorShow();
        propertyEditor.show();
        ImGui::ShowDemoWindow();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    std::cerr << "Destroying GLFW window\n";
    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(window);
    example::NodeEditorShutdown();
    ImNodes::DestroyContext();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}
