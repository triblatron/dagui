//
// Created by Tony Horrobin on 10/05/2025.
//

#include "config/config.h"

#include "core/WidgetToSceneNodeConverter.h"
#include "core/ConfigurationElement.h"
#include "core/WidgetPattern.h"
#include "core/Widget.h"
#include "core/Group.h"

namespace dagui
{

    void WidgetToSceneNodeConverter::configure(dagbase::ConfigurationElement &config)
    {
        config.eachChild([this](dagbase::ConfigurationElement& child) {
            auto pattern = new WidgetPattern();
            pattern->configure(child);
            _widgetPatterns.insert(NameToWidgetPatternMap::value_type(dagbase::Atom::intern(pattern->matchClass()), pattern));
            return true;
        });
    }

    SceneNode *WidgetToSceneNodeConverter::convert(SceneNodeFactory &factory, Widget &widgetTree)
    {
        auto typeName = widgetTree.typeName();

        if (auto it=_widgetPatterns.find(typeName); it!=_widgetPatterns.end())
        {
            auto sceneNode = it->second->match(factory, widgetTree);

            if (sceneNode)
            {
                if (auto group = dynamic_cast<Group*>(sceneNode); group)
                {
                    widgetTree.eachChild([this,&group,&factory](Widget& child) {
                        auto childSceneNode = convert(factory, child);
                        if (childSceneNode)
                        {
                            group->addChild(childSceneNode);
                        }
                        return true;
                    });

                }
            }
            return sceneNode;
        }

        return nullptr;
    }
}
