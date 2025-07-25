//
// Created by Tony Horrobin on 12/02/2025.
//

#pragma once

#include "config/Export.h"

#include "gfx/AttributeLayout.h"

#include <vector>

namespace dagui
{
    //! Description of the attributes in a generic attribute array
    struct DAGUI_API ArrayDescriptor
    {
        std::uint32_t offsetSoFar{0};
        using AttributeLayoutArray = std::vector<AttributeLayout>;
        AttributeLayoutArray attributes;

        void configure(dagbase::ConfigurationElement& config);

        std::size_t size() const;
    };

    struct DAGUI_API IndexArrayDescriptor
    {
        AttributeDescriptor::DataType dataType{AttributeDescriptor::TYPE_UNKNOWN};

        void configure(dagbase::ConfigurationElement& config);

        std::size_t size() const;
    };
}