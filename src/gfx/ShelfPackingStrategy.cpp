//
// Created by tony on 30/09/24.
//

#include "config/config.h"

#include "gfx/ShelfPackingStrategy.h"
#include "gfx/BinImageDef.h"
#include "gfx/ImageSource.h"

namespace nfe
{
    size_t maxHeightInThisShelf {0};

    void ShelfPackingStrategy::makeItSo()
    {
        _errod = ERR_OK;
        std::size_t nextX {0};
        std::size_t nextY {0};

        while (_source->hasMore())
        {
            nfe::ImageDef* inputImage = _source->item();

            if (_binImageDef!=nullptr && inputImage!=nullptr)
            {
                if ( nextX + inputImage->width() <= _binImageDef->width() && nextY + inputImage->height() <= _binImageDef->height())
                {
                    allocateImage(inputImage, &maxHeightInThisShelf, &nextX, &nextY);
                }
                else
                {
                    // Try next shelf
                    nextX = 0u;
                    nextY += maxHeightInThisShelf;
                    maxHeightInThisShelf = 0u;
                    if ( nextX + inputImage->width() <= _binImageDef->width() && nextY + inputImage->height() <= _binImageDef->height())
                    {
                        allocateImage(inputImage, &maxHeightInThisShelf, &nextX, &nextY);
                    }
                    else
                    {
                        _errod = ERR_FAILED_TO_PACK;
                        break;
                    }
                }
                if (ok())
                {
                    _inputs.emplace_back(inputImage);
                }
            }
            _source->nextItem();
        }
    }

    void ShelfPackingStrategy::allocateImage(ImageDef* inputImage, size_t* maxHeightInThisShelf, size_t* nextX, size_t* nextY)
    {
        if (inputImage!=nullptr && maxHeightInThisShelf != nullptr && nextX != nullptr && nextY != nullptr)
        {
            // Update max height for current shelf
            if (inputImage->height() > *maxHeightInThisShelf)
            {
                *maxHeightInThisShelf = inputImage->height();
            }
            // Copy input image
            //_binImage->copyFrom(*nextY, *nextX, inputImage);
            // Update free space
            inputImage->setPos(*nextX, *nextY);
            *nextX += inputImage->width();
            if (*nextX == _binImageDef->width())
            {
                *nextX = 0u;
                *nextY += *maxHeightInThisShelf;
                *maxHeightInThisShelf = 0u;
            }
            ++_numAllocations;
        }
    }
}
