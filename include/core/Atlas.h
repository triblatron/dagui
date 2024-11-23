#pragma once

#include "config/Export.h"

#include <cstdlib>

namespace dagui
{
	class ImageDef;

	class DAGUI_API Atlas
	{
	public:
		virtual ~Atlas() noexcept = default;

		virtual std::size_t width() const = 0;

		virtual std::size_t height() const = 0;

		virtual void allocateImage(ImageDef* inputImage, size_t* maxHeightInThisShelf, size_t* nextX, size_t* nextY) = 0;
	};
}