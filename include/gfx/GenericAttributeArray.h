//
// Created by Tony Horrobin on 15/02/2025.
//

#pragma once

#include "AttributeDescriptor.h"
#include "config/Export.h"
#include "gfx/AttributeArray.h"

#include <cstring>

namespace dagui
{
    template<typename Vertex>
    class GenericAttributeArray : public AttributeArray
    {
    public:
        GenericAttributeArray() = default;

        template<typename ComponentType>
        void addAttribute(std::size_t attrIndex, const ComponentType* attr, std::size_t size)
        {
            Vertex vertex;
            _data.push_back(vertex);
            //std::copy(attr, attr + size, static_cast<char*>(&_data.back()) + _descriptor.attributes[attrIndex].offset);;
            std::memcpy((char*)&_data.back() + _descriptor.attributes[attrIndex].offset, (const char*)attr, size);
        }

        template<typename ComponentType>
        void get(std::size_t attrIndex, std::size_t index, ComponentType* buf, std::size_t bufSize)
        {
            //std::copy(&_data[index] + _descriptor.attributes[attrIndex].offset, &_data[index] + _descriptor.attributes[attrIndex].offset + bufSize, buf);
            std::memcpy((char*)buf, (const char*)&_data[index] + _descriptor.attributes[attrIndex].offset, bufSize);
        }

        void addVertex(const Vertex& vertex)
        {
            _data.push_back(vertex);
        }

        Vertex vertex(std::size_t index) const
        {
            if (index<_data.size())
                return _data[index];

            return Vertex();
        }

        std::size_t size() const override
        {
            return _data.size();
        }

        std::size_t elementSize() const override
        {
            return sizeof(Vertex);
        }

        const void* data() const override
        {
            return _data.data();
        }
    private:
        using VertexArray = std::vector<Vertex>;
        VertexArray _data;
    };
}