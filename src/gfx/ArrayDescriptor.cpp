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
        std::uint32_t elementOffset{0};
        config.eachChild(([this, &offset, &elementOffset](dagbase::ConfigurationElement& child)
        {
            AttributeLayout layout;

            layout.configure(child);
            layout.elementOffset = elementOffset;
            layout.offset = offset;
            offset += layout.attr.size();
            elementOffset = offset;
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
            total += attr.attr.size();
        }

        return total;
    }
}
