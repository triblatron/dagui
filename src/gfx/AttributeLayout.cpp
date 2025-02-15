//
// Created by Tony Horrobin on 15/02/2025.
//

#include "config/config.h"

#include "gfx/AttributeLayout.h"

#include "core/ConfigurationElement.h"
#include "gfx/AttributeDescriptor.h"

namespace dagui
{
    void AttributeLayout::configure(dagbase::ConfigurationElement& config)
    {
        if (auto element=config.findElement("attribute"); element)
            attr.configure(*element);

    }
}
