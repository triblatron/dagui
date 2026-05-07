//
// Created by Tony Horrobin on 25/04/2026.
//

#ifndef DAGUI_TUI_H
#define DAGUI_TUI_H

#include "config/Export.h"

#include "core/TypeRegistry.h"
#include "core/Atom.h"
#include "core/EditorParameters.h"
#include "util/VectorMap.h"

namespace dagbase
{
    struct Type;
}

namespace dagui
{
    class CompoundEditor;
    class Editor;

    class DAGUI_API EditorRegistry
    {
    public:
        virtual ~EditorRegistry() = default;

        void configure(dagbase::ConfigurationElement& config);

        virtual void registerEditor(const EditorParameters& params) = 0;

        virtual void registerCompoundEditor(dagbase::Atom typeName, CompoundEditor* editor) = 0;

        //! Use the Prototype pattern to create an Editor tree for the given type
        //! \retval nullptr if the Editor could not be created.
        //! \note Lazily creates an Editor for properties whose types have not been seen yet.
        Editor* findOrCreateEditor(const dagbase::Type& type);

        virtual Editor* findEditor(dagbase::Atom typeName) = 0;

        // ReSharper disable once CppPassValueParameterByConstReference
        virtual void unregisterEditor(dagbase::Atom typeName) = 0;// NOLINT(*-unnecessary-value-param)
    };
}
#endif //DAGUI_TUI_H
