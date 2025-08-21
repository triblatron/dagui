//
// Created by Tony Horrobin on 12/07/2025.
//

#include "config/config.h"

#include "gfx/OpaqueIndexArray.h"

#include <algorithm>
#include <iterator>

namespace dagui
{

    std::size_t OpaqueIndexArray::size() const
    {
        return _array.size() / descriptor().size();
    }

    void OpaqueIndexArray::addIndex(const void *buffer, std::size_t bufferSize)
    {
        std::copy_n(static_cast<const char*>(buffer), bufferSize, std::back_inserter(_array));
    }

    void OpaqueIndexArray::getIndex(std::size_t index, void *buffer, std::size_t bufferSize)
    {
        if (buffer && bufferSize>=descriptor().size() && _array.size()>=(index+1)*descriptor().size())
        {
            std::copy_n(static_cast<const char*>(data()) + index * descriptor().size(),bufferSize,static_cast<char*>(buffer));
        }
    }

    const void *OpaqueIndexArray::data() const
    {
        if (!_array.empty())
            return _array.data();
        else
            return nullptr;
    }
}