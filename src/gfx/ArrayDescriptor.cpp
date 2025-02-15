//
// Created by Tony Horrobin on 15/02/2025.
//

#include "config/config.h"

#include "gfx/ArrayDescriptor.h"

#include "core/ConfigurationElement.h"

namespace dagui
{
    void ArrayDescriptor::configure(dagbase::ConfigurationElement& config)
    {
        std::uint32_t offset{0};

        config.eachChild(([this, &offset](dagbase::ConfigurationElement& child)
        {
            AttributeLayout layout;

            layout.configure(child);
            layout.offset = offset;
            offset += layout.attr.size();

            attributes.push_back(layout);

            return true;
        }));

        for (auto& attr : attributes)
        {
            attr.stride = offset;;
        }
    }
}
