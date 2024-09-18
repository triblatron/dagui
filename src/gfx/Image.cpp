#include "config/config.h"

#include "gfx/Image.h"

namespace nfe
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
}
