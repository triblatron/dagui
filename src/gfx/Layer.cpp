//
// Created by Tony Horrobin on 19/02/2025.
//

#include "config/config.h"

#include "gfx/Layer.h"
#include "core/ConfigurationElement.h"

namespace dagui
{
    void LayerAttributes::configure(dagbase::ConfigurationElement& config)
    {
        if (auto element=config.findElement("texture"); element)
            texture = element->asInteger();

        if (auto element=config.findElement("shader"); element)
            shader = element->asInteger();

        if (auto element=config.findElement("type"); element)
            type = static_cast<Type>(element->asInteger());

        if (auto element=config.findElement("depth"); element)
            depth = element->asInteger();
    }

    bool LayerAttributes::operator<(const LayerAttributes& rhs) const
    {
        if (texture < rhs.texture)
            return true;

        return false;
    }
}