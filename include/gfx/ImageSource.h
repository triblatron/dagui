#pragma once

#include "config/Export.h"

namespace nfe
{
	class ImageDef;
	
	class NFE_API ImageSource
	{
	public:
		virtual ~ImageSource();
		
		virtual bool hasMore() const = 0;
		virtual ImageDef* item() = 0;
		virtual void nextItem() = 0;
	};
}
