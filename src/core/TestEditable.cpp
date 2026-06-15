//
// Created by Tony Horrobin on 15/06/2026.
//

#include "config/config.h"

#include "core/TestEditable.h"
#include "imgui.h"

namespace dagui
{
    void TestEditable::edit(void* context)
    {
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(context));
        ImGui::Checkbox("foo", &_foo);
    }
}