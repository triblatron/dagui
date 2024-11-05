//
// Created by tony on 30/09/24.
//

#pragma once

#include "config/Export.h"

#include "gfx/PackingStrategy.h"

#include <vector>
#include <cstdlib>

namespace nfe
{
    class NFE_API ShelfPackingStrategy : public PackingStrategy
    {
    public:
        ShelfPackingStrategy() = default;

        void setBinRectangle(ImageDef* bin) override
        {
            if (bin != nullptr)
            {
                _binImageDef = bin;
            }
        }

        void setInputSource(ImageSource* source) override
        {
            if (source!=nullptr)
            {
                _source = source;
            }
        }

        void makeItSo() override;

        Error error() const override
        {
            return _errod;
        }

        void allocateImage(ImageDef* inputImage, size_t* maxHeightInThisShelf, size_t* nextX, size_t* nextY);
        bool ok() const
        {
            return _errod == ERR_OK;
        }

        std::size_t numAllocations() const
        {
            return _numAllocations;
        }
    private:
        Error _errod{ERR_UNKNOWN};
        ImageDef* _binImageDef{nullptr};
        ImageSource* _source{nullptr};
        using ImageDefArray = std::vector<ImageDef*>;
        ImageDefArray _inputs;
        std::size_t _numAllocations{0};
    };
}
