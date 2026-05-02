//
// Created by Tony Horrobin on 02/05/2026.
//

#include "config/config.h"

#include "core/EditorRegistry.h"

#include "core/CompoundEditor.h"
#include "core/BoolEditor.h"

namespace dagui
{
    Editor * EditorRegistry::findOrCreateEditor(const dagbase::Type &type)
    {
        auto existingEditor = findEditor(type.name);
        if (existingEditor)
            return existingEditor->clone();

        if (type.members.empty())
        {
            // Primitive type, no properties
            if (type.name == "Boolean")
            {
                existingEditor = new BoolEditor(type.name);
            }

            if (existingEditor)
            {
                registerEditor(type.name, existingEditor);

                return existingEditor->clone();
            }
        }

        // Traverse all properties of type
        auto* root = new CompoundEditor;
        auto props = type.enumerate(dagbase::TypeData::MEMBER_PROPERTY);
        for (auto itProp=props.first; itProp!=props.second; ++itProp)
        {
            auto propEditor = findOrCreateEditor(*std::get<dagbase::Property>(itProp->data.value).type);
            if (propEditor)
            {
                root->addChild(propEditor);
            }
        }

        registerEditor(type.name, root);

        return root->clone();
    }

    Editor * EditorRegistry::findEditor(dagbase::Atom typeName)
    {
        if (auto it=_editors.find(typeName); it!=_editors.end())
            return it->second;

        return nullptr;
    }
}
