#pragma once

#include "config/Export.h"

namespace nfe
{
	class Image;
	
	class NFE_API ImageSource
	{
	public:
		virtual ~ImageSource();
		
		virtual bool hasMore() const = 0;
		virtual Image* item() = 0;
		virtual void nextItem() = 0;
	};
}
