//
// Created by Tony Horrobin on 29/04/2025.
//

#include "config/config.h"

#include "core/SceneTemplate.h"
#include "core/ConfigurationElement.h"
#include "core/Group.h"
#include "core/Text.h"

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
                auto snippet = new WidgetSnippet();
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

    SceneNode *SceneTemplate::instantiate(Widget &widget)
    {
        if (_sceneClass=="Group")
        {
            Group* parent = new Group(&widget);

            for (auto childTemplate : _children.a)
            {
                auto child = childTemplate->instantiate(widget);
                if (child)
                    parent->addChild(child);
            }

            return parent;
        }
        else if (_sceneClass=="Text")
        {
            auto text =  new Text(&widget);
            if (auto it = _snippets.m.find("text"); it!=_snippets.m.end())
            {
                auto textSnippet = it->second;
                textSnippet->setWidget(&widget);
                text->setText(textSnippet->interpolate());
            }
            return text;
        }

        return nullptr;
    }
}