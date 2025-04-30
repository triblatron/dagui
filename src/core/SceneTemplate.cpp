//
// Created by Tony Horrobin on 29/04/2025.
//

#include "config/config.h"

#include "core/SceneTemplate.h"
#include "core/ConfigurationElement.h"
#include "core/Group.h"

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
                _props.m.insert(PropertyMap::value_type (prop,value));
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
            return new Group(&widget);
        }

        return nullptr;
    }
}