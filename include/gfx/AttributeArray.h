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

        //! \return The number of vertices in the array.
        virtual std::size_t size() const = 0;

        //! \return The size of one element of the array in bytes.
        virtual std::size_t elementSize() const = 0;

        //! Add one vertex to the array.
        //! \note Type safety is provided by the descriptor.
        //! \note There must be at last bufferSize bytes available in buffer,
        virtual void addVertex(const void* buffer, std::size_t bufferSize) = 0;

        //! Query one vertex from the array
        //! \pre index<size()
        //! Copies at most bufferSize bytes into buffer.
        virtual void getVertex(std::size_t index, void* buffer, std::size_t bufferSize) = 0;

        //! Get a pointer to the internal array.
        //! \note Useful for passing to glBufferData() et al.
        virtual const void* data() const = 0;
    protected:
        ArrayDescriptor _descriptor;
    };
}