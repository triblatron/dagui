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

    void IndexArrayDescriptor::configure(dagbase::ConfigurationElement &config)
    {
        dagbase::ConfigurationElement::readConfig<AttributeDescriptor::DataType>(config, "dataType", &AttributeDescriptor::parseDataType, &dataType);
    }

    std::size_t IndexArrayDescriptor::size() const
    {
        switch (dataType)
        {
            case AttributeDescriptor::TYPE_UINT16:
                return sizeof(std::uint16_t);
            case AttributeDescriptor::TYPE_UINT32:
                return sizeof(std::uint32_t);
            default:
                return 0;
        }
    }
}
