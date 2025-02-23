//
// Created by Tony Horrobin on 15/02/2025.
//

#pragma once

#include "config/Export.h"

#include "ArrayDescriptor.h"

namespace dagui
{
    class DAGUI_API AttributeArray
    {
    public:
        AttributeArray() = default;

        virtual ~AttributeArray() = default;

        void setDescriptor(const ArrayDescriptor& descriptor)
        {
            _descriptor = descriptor;
        }

        const ArrayDescriptor& desciptor() const
        {
            return _descriptor;
        }

        virtual std::size_t size() const = 0;

        virtual std::size_t elementSize() const = 0;

        virtual void addVertex(const void* buffer, std::size_t bufferSize) = 0;

        virtual void getVertex(std::size_t index, void* buffer, std::size_t bufferSize) = 0;

        virtual const void* data() const = 0;
    protected:
        ArrayDescriptor _descriptor;
    };
}