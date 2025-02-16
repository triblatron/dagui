//
// Created by Tony Horrobin on 15/02/2025.
//

#include "config/config.h"

#include "gfx/AttributeDescriptor.h"

#include "core/ConfigurationElement.h"
#include "util/enums.h"

namespace dagui
{
    std::size_t AttributeDescriptor::size() const
    {
        std::size_t size = 0;
        switch (dataType)
        {
        case TYPE_UNKNOWN:
            size = 0;
            break;
        case TYPE_BYTE:
            size = 1;
            break;
        case TYPE_INT32:
            size = sizeof(std::int32_t);
            break;
        case TYPE_UINT32:
            size = sizeof(std::uint32_t);
            break;
        case TYPE_FLOAT:
            size = sizeof(float);
            break;
        case TYPE_DOUBLE:
            size = sizeof(double);
            break;
        }

        return numComponents * size;
    }

    void AttributeDescriptor::configure(dagbase::ConfigurationElement& config)
    {
        if (auto element=config.findElement("name"); element)
            name = element->asString();

        if (auto element=config.findElement("dataType"); element)
            dataType = parseDataType(element->asString().c_str());

        if (auto element=config.findElement("numComponents"); element)
            numComponents = element->asInteger();

        if (auto element=config.findElement("usage"); element)
            usage = parseUsage(element->asString().c_str());
    }

    const char* AttributeDescriptor::dataTypeToString(DataType type)
    {
        switch (type)
        {
            ENUM_NAME(TYPE_UNKNOWN)
            ENUM_NAME(TYPE_BYTE)
            ENUM_NAME(TYPE_INT32)
            ENUM_NAME(TYPE_UINT32)
            ENUM_NAME(TYPE_FLOAT)
            ENUM_NAME(TYPE_DOUBLE)
        }

        return "<error>";
    }

    AttributeDescriptor::DataType AttributeDescriptor::parseDataType(const char* str)
    {
        TEST_ENUM(TYPE_UNKNOWN, str);
        TEST_ENUM(TYPE_BYTE, str);
        TEST_ENUM(TYPE_INT32, str);
        TEST_ENUM(TYPE_UINT32, str);
        TEST_ENUM(TYPE_FLOAT, str);
        TEST_ENUM(TYPE_DOUBLE, str);

        return TYPE_UNKNOWN;
    }

    const char* AttributeDescriptor::usageToString(Usage usage)
    {
        switch (usage)
        {
            ENUM_NAME(USAGE_UNKNOWN)
            ENUM_NAME(USAGE_POSITION)
            ENUM_NAME(USAGE_COLOUR)
        }
    }

    AttributeDescriptor::Usage AttributeDescriptor::parseUsage(const char* str)
    {
        TEST_ENUM(USAGE_UNKNOWN, str);
        TEST_ENUM(USAGE_POSITION, str);
        TEST_ENUM(USAGE_COLOUR, str);
    }
}
