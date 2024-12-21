#include "config/config.h"

#include "gfx/TextureAtlas.h"
#include "util/enums.h"
#include "gfx/Image.h"
#include "gfx/ImageSource.h"
#include "gfx/BinImageDef.h"

#include <cstring>
#include "core/BinPackingStrategy.h"

namespace dagui {
	TextureAtlas::TextureAtlas(std::size_t width, std::size_t height, std::size_t numComponents)
	:
	_errod(ERR_UNKNOWN),
	_numComponents(numComponents)
	{
		if (!isPowerOfTwo(width) || !isPowerOfTwo(height))
		{
			_errod = ERR_NON_POWER_OF_TWO_DIMS;
			return;
		}

		_binImageDef = new BinImageDef(width, height, numComponents);
		_errod = ERR_OK;
	}
	
	TextureAtlas::~TextureAtlas()
	{
		delete _binImage;
	}
	
	void TextureAtlas::pack(BinPackingStrategy& strategy)
	{
		if (_source != nullptr)
		{
			size_t nextX{0};
			size_t nextY{0};
			size_t maxHeightInThisShelf{0};

			_images.reserve(_source->estimateCount());
			strategy.pack(*_source, *this);

			if (ok())
			{
				_binImage = _binImageDef->createImage();
				for (auto def : _images)
				{
					Image* image = def.second->createImage();
					_binImage->copyFrom(def.second->y(), def.second->x(), image);
				}
			}
		}
	}

	void TextureAtlas::allocateImage(unsigned long id, ImageDef* inputImage)
	{
		if (inputImage!=nullptr)
		{
			_images.insert(ImageMap::value_type(id, inputImage));
			++_numAllocations;
		}
	}

	const char * TextureAtlas::errorToString(Error error)
	{
		switch (error)
		{
			ENUM_NAME(ERR_OK)
			ENUM_NAME(ERR_UNKNOWN)
			ENUM_NAME(ERR_NON_POWER_OF_TWO_DIMS)
			ENUM_NAME(ERR_FAILED_TO_ALLOCATE)
		}
		return "<error>";
	}

	const ImageDef* TextureAtlas::imageForGlyphIndex(unsigned long glyphIndex) const
	{
		auto it = _images.find(glyphIndex);
		if (it!=_images.end())
		{
			return it->second;
		}
		else
		{
			return nullptr;
		}
	}
	
	TextureAtlas::Error TextureAtlas::parseError(const char* str)
	{
		TEST_ENUM(ERR_UNKNOWN, str)
		TEST_ENUM(ERR_OK, str)
		TEST_ENUM(ERR_NON_POWER_OF_TWO_DIMS, str)
		TEST_ENUM(ERR_FAILED_TO_ALLOCATE, str)
		
		return ERR_UNKNOWN;
	}
}
