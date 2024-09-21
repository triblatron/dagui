#include "config/config.h"

#include "gfx/TextureAtlas.h"
#include "util/enums.h"
#include "gfx/Image.h"
#include "gfx/ImageSource.h"

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
		
		_binImage = new Image(width, height, numComponents);
		
		_errod = ERR_OK;
	}
	
	TextureAtlas::~TextureAtlas()
	{
		delete _binImage;
	}
	
	void TextureAtlas::pack()
	{
		if (_source != nullptr)
		{
			size_t nextX{0};
			size_t nextY{0};
			size_t maxHeightInThisShelf{0};
			
			while (_source->hasMore())
			{
				nfe::Image* inputImage = _source->item();
				
				if (_binImage!=nullptr && inputImage!=nullptr & nextX + inputImage->width() < _binImage->width() && nextY + inputImage->height() < _binImage->height())
				{
					// Update max height for current shelf
					if (inputImage->height() > maxHeightInThisShelf)
					{
						maxHeightInThisShelf = inputImage->height();
					}
					// Copy input image
					_binImage->copyFrom(nextY, nextX, inputImage);
					// Update free space
					nextX += inputImage->width();
					if (nextX == _binImage->width())
					{
						nextX = 0u;
						nextY += maxHeightInThisShelf;
						maxHeightInThisShelf = 0u;
					}
				}
							
				_source->nextItem();
			}
		}
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
