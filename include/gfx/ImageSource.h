#pragma once

#include "config/Export.h"

namespace dagui
{
	class ImageDef;
	
	class DAGUI_API ImageSource
	{
	public:
		virtual ~ImageSource();
		
		virtual bool hasMore() const = 0;
		virtual ImageDef* item() = 0;
		virtual void nextItem() = 0;
	};
}
