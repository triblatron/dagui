#include "config/config.h"

#include "gfx/TextureAtlas.h"
#include "util/enums.h"

#include <cstring>

namespace nfe
{
	TextureAtlas::TextureAtlas(std::size_t width, std::size_t height, std::size_t numComponents)
	:
	_errod(ERR_UNKNOWN)
	{
		if (!isPowerOfTwo(width) || !isPowerOfTwo(height))
		{
			_errod = ERR_NON_POWER_OF_TWO_DIMS;
			return;
		}
		
		_errod = ERR_OK;
	}
	
	void TextureAtlas::pack()
	{
	}
	
	const char* TextureAtlas::errorToString(Error error)
	{
		switch (error)
		{
			ENUM_NAME(ERR_UNKNOWN)
			ENUM_NAME(ERR_OK)
			ENUM_NAME(ERR_NON_POWER_OF_TWO_DIMS)
		}
		
		return "<error>";
	};
	
	TextureAtlas::Error TextureAtlas::parseError(const char* str)
	{
		TEST_ENUM(ERR_UNKNOWN, str)
		TEST_ENUM(ERR_OK, str)
		TEST_ENUM(ERR_NON_POWER_OF_TWO_DIMS, str)
		
		return ERR_UNKNOWN;
	}
}
