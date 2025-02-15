//
// Created by Tony Horrobin on 15/02/2025.
//

#pragma once

#include "AttributeDescriptor.h"
#include "config/Export.h"
#include "gfx/AttributeArray.h"

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
            memcpy((char*)&_data.back() + _descriptor.attributes[attrIndex].offset, (const char*)attr, size);
        }

        template<typename ComponentType>
        void get(std::size_t attrIndex, std::size_t index, ComponentType* buf, std::size_t bufSize)
        {
            //std::copy(&_data[index] + _descriptor.attributes[attrIndex].offset, &_data[index] + _descriptor.attributes[attrIndex].offset + bufSize, buf);
            memcpy((char*)buf, (const char*)&_data[index] + _descriptor.attributes[attrIndex].offset, bufSize);
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
    private:
        using VertexArray = std::vector<Vertex>;
        VertexArray _data;
    };
}