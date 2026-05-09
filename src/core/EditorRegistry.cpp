//
// Created by Tony Horrobin on 02/05/2026.
//

#include "config/config.h"

#include "core/EditorRegistry.h"

#include "core/CompoundEditor.h"
#include "core/BoolEditor.h"

namespace dagui
{
    void EditorRegistry::configure(dagbase::ConfigurationElement &config)
    {
        if (auto element=config.findElement("classes"); element)
        {
            element->eachChild([this](dagbase::ConfigurationElement& child) {
                EditorParameters params;

                params.configure(child);

                registerEditor(params);


                return true;
            });
        }
    }

    Editor * EditorRegistry::findOrCreateEditor(const dagbase::Type &type)
    {
        auto existingEditor = findEditor(type.name);
        if (existingEditor)
            return existingEditor->clone();

        if (type.members.empty())
        {
            existingEditor = findEditor(type.name);
            // Primitive type, no properties

            if (existingEditor)
            {
                return existingEditor->clone();
            }
        }

        // Traverse all properties of type
        auto* root = new CompoundEditor;
        auto props = type.enumerate(dagbase::TypeData::MEMBER_PROPERTY);
        for (auto itProp=props.first; itProp!=props.second; ++itProp)
        {
            auto propEditor = findOrCreateEditor(std::get<dagbase::Property>(itProp->data.value));
            if (propEditor)
            {
                root->addChild(propEditor);
            }
        }

        registerCompoundEditor(type.name, root);

        return root->clone();
    }

    Editor * EditorRegistry::findOrCreateEditor(const dagbase::Property &prop)
    {
        auto editor = findEditor(prop.type->name);
        if (editor)
        {
            auto copy = editor->clone();
            copy->setProperty(prop);
            return copy;
        }
\
        // Traverse all properties of type
        auto* root = new CompoundEditor;
        root->setProperty(prop);
        auto props = prop.type->enumerate(dagbase::TypeData::MEMBER_PROPERTY);
        for (auto itProp=props.first; itProp!=props.second; ++itProp)
        {
            auto propEditor = findOrCreateEditor(std::get<dagbase::Property>(itProp->data.value));
            if (propEditor)
            {
                root->addChild(propEditor);
            }
        }

        registerCompoundEditor(prop.type->name, root);

        return root->clone();
    }
}
