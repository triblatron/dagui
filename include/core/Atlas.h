#pragma once

#include "config/Export.h"

#include <cstdlib>
#include <cstdint>

namespace dagui
{
	class ImageDef;

	class DAGUI_API Atlas
	{
	public:
		virtual ~Atlas() noexcept = default;

		virtual std::size_t width() const = 0;

		virtual std::size_t height() const = 0;

		virtual void allocateImage(unsigned long id, ImageDef* inputImage) = 0;
        
        virtual const ImageDef* imageForGlyphIndex(unsigned long glyphIndex) const = 0;
	};
}
