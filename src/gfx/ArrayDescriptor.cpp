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
        std::uint32_t offset{offsetSoFar};

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

    std::size_t ArrayDescriptor::size() const
    {
        std::size_t total{0};

        for (auto& attr : attributes)
        {
            total += attr.stride;
        }

        return total;
    }
}
