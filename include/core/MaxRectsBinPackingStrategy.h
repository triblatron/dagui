#pragma once

#include "config/Export.h"

#include "core/BinPackingStrategy.h"

namespace dagui
{
	class ImageDef;
	class SpaceTree;

	class DAGUI_API MaxRectsBinPackingStrategy : public BinPackingStrategy
	{
	public:
		void pack(ImageSource& imageSource, Atlas& atlas) override;
	private:
		void allocateImage(Atlas& atlas, ImageDef& image, SpaceTree& freeNode);
	};
}
