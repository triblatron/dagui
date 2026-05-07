//
// Created by Tony Horrobin on 06/05/2026.
//

#include "config/config.h"

#include "tui/EditorRegistryTUI.h"

#include "core/BoolEditor.h"
#include "core/CompoundEditor.h"
#include "core/ConfigurationElement.h"

namespace dagui
{
    void EditorRegistryTUI::registerEditor(const EditorParameters &params)
    {
        Editor* editor = nullptr;

        if (params.className == "BoolEditor")
        {
            editor = new BoolEditor(params);
        }

        if (editor)
        {
            _registry.emplace(params.typeName, editor);
        }
    }

    void EditorRegistryTUI::registerCompoundEditor(dagbase::Atom typeName, CompoundEditor *editor)
    {
        _registry.emplace(typeName, editor);
    }

    dagui::Editor * EditorRegistryTUI::findEditor(dagbase::Atom typeName)
    {
        if (auto it=_registry.find(typeName); it != _registry.end())
            return it->second;

        return nullptr;
    }

    void EditorRegistryTUI::unregisterEditor(dagbase::Atom typeName)
    {
        if (auto it=_registry.find(typeName); it != _registry.end())
            _registry.erase(it);
    }
}
