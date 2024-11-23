#pragma once

#include "config/Export.h"

#include <string_view>

namespace dagui
{
	class BinPackingStrategy;

	class DAGUI_API BinPackingStrategyFactory
	{
	public:
		BinPackingStrategy* createStrategy(std::string_view className);
	};
}