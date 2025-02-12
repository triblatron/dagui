//
// Created by Tony Horrobin on 12/02/2025.
//

#pragma once

#include "config/Export.h"

#include <cstdint>

namespace dagui
{
    //! Describes an attribute
    struct DAGUI_API AttributeDescriptor
    {
        //! The type of each component
        enum DataType
        {
            //! No type, the initial value
            TYPE_UNKNOWN,
            //! 8bit unsigned integer
            TYPE_BYTE,
            // 32bit signed integer
            TYPE_INT32,
            //! 32bit unsigned integer
            TYPE_UINT32,
            //! IEEE single precision float
            TYPE_FLOAT,
            //! IEEE double precision float
            TYPE_DOUBLE
        };
        //! The name of the attribute
        //! \note Not passed to the graphics library.
        const char* name{nullptr};
        //! The type of each component, initially unknown and will typically cause an error if used.
        DataType dataType{TYPE_UNKNOWN};
        //! The number of components of each instance of this attribute
        //! \note The initial value will typically cause an error if used.
        //! \note Valid values are 1,2,3,4
        std::uint32_t numComponents{0};
    };
}