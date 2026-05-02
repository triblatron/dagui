//
// Created by Tony Horrobin on 25/04/2026.
//

#ifndef DAGUI_TUI_H
#define DAGUI_TUI_H

#include "config/Export.h"

#include "core/TypeRegistry.h"
#include "core/Atom.h"
#include "util/VectorMap.h"

namespace dagbase
{
    struct Type;
}

namespace dagui
{
    class Editor;

    class DAGUI_API EditorRegistry
    {
    public:
        ~EditorRegistry() = default;

        void registerEditor(dagbase::Atom typeName, dagui::Editor* editor) // NOLINT(*-unnecessary-value-param)
        {
            _editors.emplace(typeName, editor);
        }

        //! Use the Prototype pattern to create an Editor tree for the given type
        //! \retval nullptr if the Editor could not be created.
        //! \note Lazily creates an Editor for properties whose types have not been seen yet.
        Editor* findOrCreateEditor(const dagbase::Type& type);

        Editor* findEditor(dagbase::Atom typeName);

        // ReSharper disable once CppPassValueParameterByConstReference
        void unregisterEditor(dagbase::Atom typeName) // NOLINT(*-unnecessary-value-param)
        {
            if (auto it=_editors.find(typeName); it!=_editors.end())
            {
                _editors.erase(it);
            }
        }
    private:
        using TypeToEditorMap = dagbase::VectorMap<dagbase::Atom, dagui::Editor*>;
        TypeToEditorMap _editors;
    };
}
#endif //DAGUI_TUI_H
