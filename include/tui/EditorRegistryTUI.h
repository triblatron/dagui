//
// Created by Tony Horrobin on 06/05/2026.
//

#pragma once

#include "config/Export.h"

#include "core/EditorRegistry.h"

#include <map>

class TGroup;

namespace dagui
{
    class Editor;

    class DAGUI_API EditorRegistryTUI : public EditorRegistry
    {
    public:
        void setRootWidget(TGroup* root)
        {
            _root = root;
        }

        void registerEditor(const EditorParameters& params) override;

        void registerCompoundEditor(dagbase::Atom typeName, CompoundEditor *editor) override;

        dagui::Editor* findEditor(dagbase::Atom typeName) override;

        // ReSharper disable once CppPassValueParameterByConstReference
        void unregisterEditor(dagbase::Atom typeName) override;// NOLINT(*-unnecessary-value-param)

    private:
        TGroup* _root{nullptr};
        using TypeToEditorMap = std::map<dagbase::Atom, dagui::Editor*>;
        TypeToEditorMap _registry;
    };
}