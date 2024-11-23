#include "config/config.h"

#include "core/MaxRectsBinPackingStrategy.h"
#include "core/SpaceTree.h"
#include "core/Atlas.h"
#include "gfx/ImageSource.h"
#include "gfx/ImageDef.h"

namespace dagui
{
	void MaxRectsBinPackingStrategy::pack(ImageSource& imageSource, Atlas& atlas)
	{
		SpaceTree* spaceTree = new SpaceTree(0, 0, std::int32_t(atlas.width()), std::int32_t(atlas.height()), SpaceTree::TYPE_FREE, SpaceTree::SPLIT_UNKNOWN);
		setResult(RESULT_UNKNOWN);
		while (imageSource.hasMore())
		{
			auto image = imageSource.item();

			if (image != nullptr)
			{
				auto freeNode = spaceTree->findSpace(image->width(), image->height(), SpaceTree::FIT_BEST_SHORT_SIDE);

				if (freeNode != nullptr)
				{
					atlas.allocateImage(image, nullptr, nullptr, nullptr);
				}
				else
				{
					setResult(RESULT_FAILED_TO_FIND_SPACE);
				}
			}
			imageSource.nextItem();
		}
		setResult(RESULT_OK);
		delete spaceTree;
	}
}
