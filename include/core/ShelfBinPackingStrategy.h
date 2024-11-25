#pragma once

#include "config/Export.h"

#include "core/BinPackingStrategy.h"

namespace dagui
{
	class ImageDef;

	class DAGUI_API ShelfBinPackingStrategy : public BinPackingStrategy
	{
	public:
		void pack(ImageSource& imageSource, Atlas& atlas) override;
	private:
		void allocateImage(Atlas& atlas, ImageDef* inputImage, size_t* maxHeightInThisShelf, size_t* nextX, size_t* nextY);
	};
}
