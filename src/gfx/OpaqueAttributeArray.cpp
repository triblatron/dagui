//
// Created by Tony Horrobin on 23/02/2025.
//

#include "config/config.h"

#include "gfx/OpaqueAttributeArray.h"

namespace dagui
{
    void OpaqueAttributeArray::addVertex(const void* buffer, std::size_t bufferSize)
    {
        if (bufferSize >= elementSize())
            for (auto a : _descriptor.attributes)
                std::copy_n(static_cast<const char*>(buffer)+a.offset, elementSize(), std::back_inserter(_data));
    }

    void OpaqueAttributeArray::getVertex(std::size_t index, void* buffer, std::size_t bufferSize)
    {
        if (bufferSize>=elementSize())
            for (auto a : _descriptor.attributes)
                std::copy_n(static_cast<const char*>(data())+elementSize()*index, elementSize(), static_cast<char*>(buffer)+a.offset);
    }
}
