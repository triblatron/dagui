#pragma once

#include "config/Export.h"

#include "core/BinPackingStrategy.h"

namespace dagui
{
	class DAGUI_API ShelfBinPackingStrategy : public BinPackingStrategy
	{
	public:
		void pack(ImageSource& imageSource, Atlas& atlas) override;
	};
}
