#include "config/config.h"

#include "core/ShelfBinPackingStrategy.h"
#include "gfx/ImageSource.h"
#include "gfx/ImageDef.h"
#include "gfx/TextureAtlas.h"

#include <vector>

namespace dagui
{
	void ShelfBinPackingStrategy::pack(ImageSource& imageSource, Atlas& atlas)
	{
		size_t nextX{ 0 };
		size_t nextY{ 0 };
		size_t maxHeightInThisShelf{ 0 };

		using ImageDefArray = std::vector<ImageDef*>;
		ImageDefArray defs;;
		setResult(RESULT_UNKNOWN);
		while (imageSource.hasMore())
		{
			dagui::ImageDef* inputImage = imageSource.item();

			if (inputImage != nullptr)
			{
				if (nextX + inputImage->width <= atlas.width() && nextY + inputImage->height <= atlas.height())
				{
					allocateImage(atlas, inputImage, &maxHeightInThisShelf, &nextX, &nextY);
				}
				else
				{
					// Try next shelf
					nextX = 0u;
					nextY += maxHeightInThisShelf;
					maxHeightInThisShelf = 0u;
					if (nextX + inputImage->width <= atlas.width() && nextY + inputImage->height <= atlas.height())
					{						
						allocateImage(atlas, inputImage, &maxHeightInThisShelf, &nextX, &nextY);
					}
					else
					{
						setResult(RESULT_FAILED_TO_FIND_SPACE);
						break;
					}
				}
				if (ok())
				{
					defs.emplace_back(inputImage);
				}
			}
			imageSource.nextItem();
		}
		if (result() == RESULT_UNKNOWN)
			setResult(RESULT_OK);
	}

	void ShelfBinPackingStrategy::allocateImage(Atlas& atlas, ImageDef* inputImage, size_t* maxHeightInThisShelf, size_t* nextX, size_t* nextY)
	{
		if (inputImage != nullptr && maxHeightInThisShelf != nullptr && nextX != nullptr && nextY != nullptr)
		{
			// Update max height for current shelf
			if (inputImage->height > *maxHeightInThisShelf)
			{
				*maxHeightInThisShelf = inputImage->height;
			}
			// Copy input image
			//_binImage->copyFrom(*nextY, *nextX, inputImage);
			// Update free space
			inputImage->x = *nextX;
			inputImage->y = *nextY;
            atlas.allocateImage(inputImage->index(), inputImage);
			*nextX += inputImage->width;
			if (*nextX == atlas.width())
			{
				*nextX = 0u;
				*nextY += *maxHeightInThisShelf;
				*maxHeightInThisShelf = 0u;
			}
			++_numAllocations;
		}
	}
}