//
// Created by Tony Horrobin on 12/02/2025.
//

#pragma once

#include "config/Export.h"

#include <cstdint>

#include "gfx/AttributeDescriptor.h"

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    //! Describes placement of an attribute in a generic attribute array
    struct DAGUI_API AttributeLayout
    {
        //! The attribute we are placing in an array
        AttributeDescriptor attr;
        //! byte offset to this attribute within the parent array layout
        //! \note set to zero for a tightly packed array of a single attribute or
        //! the first attribute in a layout
        std::uint32_t offset{0};
        //! The offset of this attribute into an element of the array.
        //! Used when transferring data into and out of the array.
        //! \note This might be different from the offset for separate arrays
        std::uint32_t elementOffset{0};
        //! byte offset between consecutive attributes
        //! \note A zero stride means the attributes are tightly packed
        std::uint32_t stride{0};

        void configure(dagbase::ConfigurationElement& config);
    };
}