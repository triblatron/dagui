#pragma once

#include "config/Export.h"

#include <cstdint>
#include <vector>

namespace nfe
{
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
	
	class NFE_API TextureAtlas
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
		
		~TextureAtlas();
		
		Error error() const
		{
			return _errod;
		}
		
		void setImageSource(ImageSource* source)
		{
			_source = source;
		}
		
		nfe::ImageDef* image()
		{
			return _binImage;
		}
		
		void pack();
		
		std::size_t numAllocations() const
		{
			return _numAllocations;
		}
		
		static const char* errorToString(Error error);
		
		static Error parseError(const char* str);
		
		static bool isPowerOfTwo(std::size_t value)
		{
			return (value & (value-1))==0;
		}
	private:
		Error _errod{ERR_UNKNOWN};
		std::size_t _numComponents{0};
		ImageDef* _binImage{nullptr};
		ImageSource* _source{nullptr};
		using ImageBoundsMap = std::vector<std::pair<Image*, ImageBounds>>;
		ImageBoundsMap _imageBounds;
		void allocateImage(ImageDef* inputImage, size_t* maxHeightInThisShelf, size_t* nextX, size_t* nextY);
		std::size_t _numAllocations{0};
	};
}
