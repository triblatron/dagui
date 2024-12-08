#pragma once

#include "config/Export.h"
#include <cstdlib>

namespace dagui
{
	class ImageDef;
	
	class DAGUI_API ImageSource
	{
	public:
		virtual ~ImageSource();

        virtual std::size_t estimateCount() const = 0;
		virtual bool hasMore() const = 0;
		virtual ImageDef* item() = 0;
		virtual void nextItem() = 0;
	};
}
