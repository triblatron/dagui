#pragma once

#include "config/Export.h"

#include <cstdint>

namespace nfe
{
	class Image;
	class ImageSource;
	
	class NFE_API TextureAtlas
	{
	public:
		enum Error
		{
			ERR_OK,
			ERR_NON_POWER_OF_TWO_DIMS,
			ERR_UNKNOWN
		};
	public:
		TextureAtlas(std::size_t width, std::size_t height, std::size_t numComponents);
		
		Error error() const
		{
			return _errod;
		}
		
		void setImageSource(ImageSource* source)
		{
			_source = source;
		}
		
		void pack();
		
		static const char* errorToString(Error error);
		
		static Error parseError(const char* str);
		
		static bool isPowerOfTwo(std::size_t value)
		{
			return (value & (value-1))==0;
		}
	private:
		Error _errod{ERR_UNKNOWN};
		Image* _binImage{nullptr};
		ImageSource* _source{nullptr};
	};
}
