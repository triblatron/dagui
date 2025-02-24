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
        {
            // Special case of copying a whole vertex
            if (!_descriptor.attributes.empty() && _descriptor.attributes[0].offset == 0 && bufferSize == elementSize())
            {
                std::copy_n(static_cast<const char*>(buffer)+_descriptor.attributes[0].offset, _descriptor.size(), std::back_insert_iterator(_data));
            }
            else
            {
                for (const auto& a : _descriptor.attributes)
                    std::copy_n(static_cast<const char*>(buffer)+a.offset, a.attr.size(), std::back_inserter(_data));
            }

            ++_numVertices;
        }
    }

    void OpaqueAttributeArray::getVertex(std::size_t index, void* buffer, std::size_t bufferSize)
    {
        if (bufferSize>=elementSize())
        {
            // Special case of copying a whole vertex
            if (!_descriptor.attributes.empty() && _descriptor.attributes[0].offset == 0 && bufferSize == elementSize())
            {
                std::copy_n(static_cast<const char*>(data())+elementSize()*index+_descriptor.attributes[0].elementOffset, elementSize(), static_cast<char*>(buffer));
            }
            else
            {
                for (const auto& a : _descriptor.attributes)
                    std::copy_n(static_cast<const char*>(data())+elementSize()*index+a.elementOffset, a.attr.size(), static_cast<char*>(buffer)+a.offset);
            }
        }
    }
}
