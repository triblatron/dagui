//
// Created by Tony Horrobin on 15/06/2026.
//

#include "config/config.h"

#include "core/TestEditable.h"
#include "imgui.h"

namespace dagui
{
    void TestEditable::edit(ImGuiContext *context)
    {
        ImGui::SetCurrentContext(context);
        ImGui::Checkbox("foo", &_foo);
        ImGui::InputDouble("value", &_bar);
        ImGui::InputInt("qux", &_qux);
    }
}