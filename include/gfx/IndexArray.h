//
// Created by Tony Horrobin on 12/07/2025.
//

#pragma once

#include "config/Export.h"

#include "ArrayDescriptor.h"

#include <cstdlib>

namespace dagui
{
    //! \class IndexArray
    //! An array of indices into an AttributeArray
    //! Used to efficiently render triangles while sharing vertices.
    class DAGUI_API IndexArray
    {
    public:
        IndexArray() = default;

        virtual ~IndexArray() = default;

        void setDescriptor(const IndexArrayDescriptor& descriptor)
        {
            _descriptor = descriptor;
        }

        const IndexArrayDescriptor& descriptor() const
        {
            return _descriptor;
        }

        //! \return The number of indices in the array.
        virtual std::size_t size() const = 0;

        virtual std::size_t elementSize() const = 0;

        //! Add one vertex to the array.
        //! \note Type safety is provided by the descriptor.
        //! \note There must be at last bufferSize bytes available in buffer,
        virtual void addIndex(const void* buffer, std::size_t bufferSize) = 0;

        //! Query one vertex from the array
        //! \pre index<size()
        //! Copies at most bufferSize bytes into buffer.
        virtual void getIndex(std::size_t index, void* buffer, std::size_t bufferSize) = 0;

        //! Get a pointer to the internal array.
        //! \note Useful for passing to glBufferData() et al.
        virtual const void* data() const = 0;
    private:
        IndexArrayDescriptor _descriptor;
    };
}