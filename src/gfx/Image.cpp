#include "config/config.h"

#include "gfx/Image.h"

#include <cstring>

namespace dagui
{
	Image::Image(std::size_t width, std::size_t height, std::size_t numComponents, unsigned char* buffer)
	:
	_width(width),
	_height(height),
	_numComponents(numComponents),
	_buffer(buffer),
	_own(false)
	{
		// Do nothing.
	}
	
	bool Image::find(std::uint8_t red, std::uint8_t green, std::uint8_t blue) const
	{
		for (auto row=0; row<_height; ++row)
		{
			for (auto col=0; col<_width; ++col)
			{
				std::uint8_t actualRed{0},actualGreen{0},actualBlue{0};
				
				get(row, col, &actualRed, &actualGreen, &actualBlue);
				
				if (red == actualRed && green == actualGreen && blue == actualBlue)
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	void Image::copyFrom(std::size_t destOriginRow, std::size_t destOriginCol, const dagui::Image* source)
	{
		if (_buffer != nullptr && source != nullptr)
		{
			size_t destRow{destOriginRow}, destCol{destOriginCol};
			
			switch (_numComponents)
			{
			case 3:
				switch (source->numComponents())
				{
				case 1:
					for (size_t row=0; row<source->height(); ++row)
					{
						for (size_t col=0; col<source->width(); ++col)
						{
							std::uint8_t red{0},green{0},blue{0};
							
							source->get(row, col, &red, &green, &blue);
							set(destRow, destCol, red, green, blue);
							destCol ++;
							if (destCol - destOriginCol == source->width())
							{
								destCol = destOriginCol;
								++destRow;
							}
						}
					}
					
					break;
				}
				
				break;
			}
		}
	}
}
