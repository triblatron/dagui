//
// Created by Tony Horrobin on 10/06/2026.
//

#pragma once

#include "config/Export.h"

#include "core/EditorRegistry.h"

#include <map>

namespace dagui
{
    class Editor;

    class DAGUI_API EditorRegistryImGui : public EditorRegistry
    {
    public:
        EditorRegistryImGui() = default;

        ~EditorRegistryImGui() override = default;

        void registerEditor(const EditorParameters& params) override;

        void registerCompoundEditor(dagbase::Atom typeName, CompoundEditor *editor) override;

        Editor* findEditor(dagbase::Atom typeName) override;

        // ReSharper disable once CppPassValueParameterByConstReference
        void unregisterEditor(dagbase::Atom typeName) override;// NOLINT(*-unnecessary-value-param)
    private:
        using TypeToEditorMap = std::map<dagbase::Atom, dagui::Editor*>;
        TypeToEditorMap _registry;
    };
}