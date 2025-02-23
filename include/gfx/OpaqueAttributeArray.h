//
// Created by Tony Horrobin on 23/02/2025.
//

#pragma once

#include "gfx/AttributeArray.h"

#include <algorithm>

namespace dagui
{
    class OpaqueAttributeArray : public dagui::AttributeArray
    {
    public:
        OpaqueAttributeArray() = default;

        ~OpaqueAttributeArray() = default;

        std::size_t size() const override
        {
            return _numVertices;
        }

        std::size_t elementSize() const override
        {
            return _descriptor.size();
        }

        void addVertex(const void* buffer, std::size_t bufferSize) override;

        void getVertex(std::size_t index, void* buffer, std::size_t bufferSize) override;

        const void* data() const override
        {
            return _data.data();
        }
    private:
        using VertexArray = std::vector<char>;
        VertexArray _data;
        std::size_t _numVertices{0};
    };
}