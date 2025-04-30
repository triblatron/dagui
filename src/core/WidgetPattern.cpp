//
// Created by Tony Horrobin on 29/04/2025.
//

#include "config/config.h"

#include "core/WidgetPattern.h"
#include "core/Widget.h"

namespace dagui
{

    SceneNode *WidgetPattern::match(Widget& widget)
    {
        if (widget.typeName() == dagbase::Atom::intern(_matchClass))
        {
            return _sceneTemplate.instantiate(widget);
        }

        return nullptr;
    }

    void WidgetPattern::configure(dagbase::ConfigurationElement& config)
    {
        if (auto element = config.findElement("matchClass"); element)
        {
            _matchClass = element->asString();
        }

        if (auto element=config.findElement("sceneTemplate"); element)
        {
            _sceneTemplate.configure(*element);
        }
    }

    dagbase::ConfigurationElement::ValueType WidgetPattern::find(std::string_view path) const
    {
        return dagbase::ConfigurationElement::ValueType();
    }
}