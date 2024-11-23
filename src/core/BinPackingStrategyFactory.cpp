#include "config/config.h"

#include "core/BinPackingStrategyFactory.h"
#include "core/ShelfBinPackingStrategy.h"
#include "core/MaxRectsBinPackingStrategy.h"

namespace dagui
{
	BinPackingStrategy* BinPackingStrategyFactory::createStrategy(std::string_view className)
	{
		if (className == "Shelf")
		{
			return new ShelfBinPackingStrategy();
		}
		else if (className == "MaxRects")
		{
			return new MaxRectsBinPackingStrategy();
		}

		return nullptr;
	}
}