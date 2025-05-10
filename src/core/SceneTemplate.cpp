//
// Created by Tony Horrobin on 29/04/2025.
//

#include "config/config.h"

#include "core/SceneTemplate.h"
#include "core/ConfigurationElement.h"
#include "core/Group.h"
#include "core/Text.h"
#include "core/ClipGroup.h"
#include "core/Widget.h"
#include "core/SceneNodeFactory.h"

namespace dagui
{
    void SceneTemplate::configure(dagbase::ConfigurationElement &config)
    {
        if (auto element=config.findElement("class"); element)
        {
            _sceneClass = element->asString();
        }

        config.eachChild([this](dagbase::ConfigurationElement& child) {
            auto prop = child.name();
            auto value = child.value();
            if (value.has_value())
            {
                auto snippet = new dagbase::PropertyBinding();
                snippet->setInput(value.toString().c_str());
                _snippets.m.insert(SnippetMap::value_type (prop, snippet));
                _props.m.insert(PropertyMap::value_type(prop, value));
            }
            return true;
        });
        if (auto element = config.findElement("children"); element)
        {
            element->eachChild([this](dagbase::ConfigurationElement& child) {
                auto childTemplate = new SceneTemplate();
                childTemplate->configure(child);
                addChild(childTemplate);
                return true;
            });
        }
    }

    SceneNode *SceneTemplate::instantiate(SceneNodeFactory& factory, Widget &widget)
    {
        auto node = factory.createNode(dagbase::Atom::intern(_sceneClass), &widget);

        if (auto parent = dynamic_cast<Group *>(node); parent)
        {
            for (auto childTemplate: _children.a)
            {
                auto child = childTemplate->instantiate(factory, widget);
                if (child)
                    parent->addChild(child);
            }
        }

        if (auto clipGroup=dynamic_cast<ClipGroup*>(node); clipGroup)
        {
            if (auto it=_snippets.m.find("bounds"); it!=_snippets.m.end())
            {
                auto snippet = it->second;
                clipGroup->setBounds(snippet->interpolate(dagbase::Atom::intern("{{"), dagbase::Atom::intern("}}"), [&widget](std::string name) {
                    return widget.find(name);
                }).asVec2());
            }
        }

        if (auto text = dynamic_cast<Text*>(node); text)
        {
            if (auto it = _snippets.m.find("text"); it!=_snippets.m.end())
            {
                auto textSnippet = it->second;
                text->setText(textSnippet->interpolate(dagbase::Atom::intern("{{"), dagbase::Atom::intern("}}"), [&widget](std::string name) {
                    return widget.find(name);
                }).asString());
            }
        }

        return node;
    }
}