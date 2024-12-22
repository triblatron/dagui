#pragma once

#include "config/Export.h"

#include "core/Atlas.h"
#include "gfx/BinImageDef.h"

#include <cstdint>
#include <vector>
#include <cstdlib>
#include "core/VectorMap.h"

namespace dagui
{
    class BinPackingStrategy;
	class Image;
	class ImageDef;
	class ImageSource;
	
	struct ImageBounds
	{
		std::size_t x{0};
		std::size_t y{0};
		std::size_t w{0};
		std::size_t h{0};
	};

	class DAGUI_API TextureAtlas : public Atlas
	{
	public:
		enum Error
		{
			ERR_OK,
			ERR_NON_POWER_OF_TWO_DIMS,
			ERR_FAILED_TO_ALLOCATE,
			ERR_UNKNOWN
		};
	public:
		TextureAtlas(std::size_t width, std::size_t height, std::size_t numComponents);
		
		~TextureAtlas() override;
		
		std::size_t width() const override
		{
			return _binImageDef != nullptr ? _binImageDef->width : std::uint32_t{ 0 };
		}

		std::size_t height() const override
		{
			return _binImageDef != nullptr ? _binImageDef->height : std::uint32_t{ 0 };
		}

        const ImageDef * imageForGlyphIndex(unsigned long glyphIndex) const override;
        
		Error error() const
		{
			return _errod;
		}

		bool ok() const
		{
			return _errod == ERR_OK;
		}

		void setImageSource(ImageSource* source)
		{
			_source = source;
		}
		
		dagui::BinImageDef* image()
		{
			return _binImageDef;
		}
		
		void pack(BinPackingStrategy& strategy);
		
		std::size_t numAllocations() const
		{
			return _numAllocations;
		}

		void allocateImage(unsigned long id, ImageDef* inputImage) override;

		static const char* errorToString(Error error);
		
		static Error parseError(const char* str);
		
		static bool isPowerOfTwo(std::size_t value)
		{
			return (value & (value-1))==0;
		}

	private:
		Error _errod{ERR_UNKNOWN};
		std::size_t _numComponents{0};
		BinImageDef* _binImageDef{nullptr};
		Image* _binImage{nullptr};
		ImageSource* _source{nullptr};
		using ImageBoundsMap = std::vector<std::pair<Image*, ImageBounds>>;
		ImageBoundsMap _imageBounds;
		std::size_t _numAllocations{0};
        using ImageMap = VectorMap<unsigned long, ImageDef*>;
        ImageMap _images;
	};
}
