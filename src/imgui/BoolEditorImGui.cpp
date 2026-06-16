//
// Created by Tony Horrobin on 10/06/2026.
//

#include "imgui/BoolEditorImGui.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace dagui
{
    BoolEditorImGui::BoolEditorImGui(const EditorParameters &params)
        :
    BoolEditor(params)
    {
        // Do nothing.
    }

    BoolEditorImGui::BoolEditorImGui(const BoolEditorImGui &other)
        :
    BoolEditor(other)
    {
        // Do nothing.
    }

    void BoolEditorImGui::makeItSo()
    {
        ImGui::SetCurrentContext((ImGuiContext*)context());
        get(&_value);
        ImGui::Checkbox("foo", &_value);
        set(_value);
    }

    BoolEditorImGui * BoolEditorImGui::clone()
    {
        return new BoolEditorImGui(*this);
    }

    dagbase::Variant BoolEditorImGui::find(std::string_view path) const
    {
        return BoolEditor::find(path);
    }
}
