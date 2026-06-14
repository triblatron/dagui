//
// Created by Tony Horrobin on 10/06/2026.
//

#include "config/config.h"

#include "ImGui/EditorRegistryImGui.h"

#include "ImGui/BoolEditorImGui.h"
#include "core/CompoundEditor.h"
#include "core/ConfigurationElement.h"

namespace dagui
{
    void EditorRegistryImGui::registerEditor(const EditorParameters &params)
    {
        Editor* editor = nullptr;

        if (params.className == "BoolEditor")
        {
            editor = new BoolEditorImGui(params);
        }

        if (editor)
        {
            _registry.emplace(params.typeName, editor);
        }
    }

    void EditorRegistryImGui::registerCompoundEditor(dagbase::Atom typeName, CompoundEditor *editor)
    {
        _registry.emplace(typeName, editor);
    }

    dagui::Editor * EditorRegistryImGui::findEditor(dagbase::Atom typeName)
    {
        if (auto it=_registry.find(typeName); it != _registry.end())
            return it->second;

        return nullptr;
    }

    void EditorRegistryImGui::unregisterEditor(dagbase::Atom typeName)
    {
        if (auto it=_registry.find(typeName); it != _registry.end())
            _registry.erase(it);
    }
}
