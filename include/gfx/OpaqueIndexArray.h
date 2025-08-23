//
// Created by Tony Horrobin on 12/07/2025.
//

#pragma once

#include "config/Export.h"

#include "gfx/IndexArray.h"

namespace dagui
{
    class DAGUI_API OpaqueIndexArray : public IndexArray
    {
    public:
        using IndexArray = std::vector<char>;
        using value_type = IndexArray;
    public:
        //! \return The number of indices in the array.
        std::size_t size() const override;

        std::size_t elementSize() const override
        {
            return descriptor().size();
        }

        //! Add one vertex to the array.
        //! \note Type safety is provided by the descriptor.
        //! \note There must be at last bufferSize bytes available in buffer,
        void addIndex(const void* buffer, std::size_t bufferSize) override;

        //! Query one vertex from the array
        //! \pre index<size()
        //! Copies at most bufferSize bytes into buffer.
        void getIndex(std::size_t index, void* buffer, std::size_t bufferSize) override;

        //! Get a pointer to the internal array.
        //! \note Useful for passing to glBufferData() et al.
        const void* data() const override;

        bool operator==(const OpaqueIndexArray& other) const;
    private:
        IndexArray _array;
    };
}