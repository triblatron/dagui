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
				if (nextX + inputImage->width() <= atlas.width() && nextY + inputImage->height() <= atlas.height())
				{
					atlas.allocateImage(inputImage, &maxHeightInThisShelf, &nextX, &nextY);
				}
				else
				{
					// Try next shelf
					nextX = 0u;
					nextY += maxHeightInThisShelf;
					maxHeightInThisShelf = 0u;
					if (nextX + inputImage->width() <= atlas.width() && nextY + inputImage->height() <= atlas.height())
					{
						atlas.allocateImage(inputImage, &maxHeightInThisShelf, &nextX, &nextY);
					}
					else
					{
						setResult(RESULT_FAILED_TO_ALLOCATE);
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
		setResult(RESULT_OK);

	}
}