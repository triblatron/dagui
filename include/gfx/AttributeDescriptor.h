//
// Created by Tony Horrobin on 12/02/2025.
//

#pragma once

#include "config/Export.h"

#include <cstdint>
#include <string>

namespace dagbase
{
    class ConfigurationElement;
}

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
            //! 16bit unsigned integer
            TYPE_UINT16,
            // 32bit signed integer
            TYPE_INT32,
            //! 32bit unsigned integer
            TYPE_UINT32,
            //! IEEE single precision float
            TYPE_FLOAT,
            //! IEEE double precision float
            TYPE_DOUBLE
        };
        //! Fixed-pipeline usages
        //! Use indices according to convention.
        enum Usage
        {
            USAGE_POSITION,
            USAGE_NORMAL,
            USAGE_TEXCOORD,
            USAGE_TANGENT,
            USAGE_BINORMAL,
            USAGE_COLOUR,
            USAGE_UNKNOWN
        };
        //! The name of the attribute
        //! \note Not passed to the graphics library.
        std::string name;
        //! The type of each component, initially unknown and will typically cause an error if used.
        DataType dataType{TYPE_UNKNOWN};
        //! The number of components of each instance of this attribute
        //! \note The initial value will typically cause an error if used.
        //! \note Valid values are 1,2,3,4
        std::uint32_t numComponents{0};
        Usage usage{USAGE_UNKNOWN};

        std::size_t size() const;

        void configure(dagbase::ConfigurationElement& config);

        static const char* dataTypeToString(DataType type);

        static DataType parseDataType(const char* str);

        static const char* usageToString(Usage usage);

        static Usage parseUsage(const char* str);
    };
}