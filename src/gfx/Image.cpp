#include "config/config.h"

#include "gfx/Image.h"

#include <cstring>
#include <iostream>

namespace dagui
{
	Image::Image(std::uint32_t width, std::uint32_t height, std::uint32_t numComponents, Origin origin, unsigned char* buffer)
	:
	_width(width),
	_height(height),
	_numComponents(numComponents),
	_origin(origin)
	{
		_buffer = new unsigned char[_width * _height * _numComponents];
		for (std::size_t i=0; i<_width*_height*_numComponents; i++)
		{
			_buffer[i] = buffer[i];
		}
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

	std::size_t Image::count(std::uint8_t red, std::uint8_t green, std::uint8_t blue) const
	{
		std::size_t retval{0};

		for (auto row=0; row<_height; ++row)
		{
			for (auto col=0; col<_width; ++col)
			{
				std::uint8_t actualRed{0},actualGreen{0},actualBlue{0};
				get(row, col, &actualRed, &actualGreen, &actualBlue);
				if (red == actualRed && green == actualGreen && blue == actualBlue)
					++retval;
			}
		}

		return retval;
	}

	void Image::copyFrom(std::uint32_t destOriginRow, std::uint32_t destOriginCol, const dagui::Image* source)
	{
		if (_buffer != nullptr && source != nullptr)
		{
			size_t destRow{destOriginRow}, destCol{destOriginCol};
//			std::cout << "Copying to row " << destOriginRow << " col " << destOriginCol << std::endl;
//			std::cout << "Source image has " << source->count(255,255,255) << " white pixels\n";
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
							if (_origin==source->_origin)
							{
								source->get(row, col, &red, &green, &blue);
							}
							else
							{
								source->get(source->height()-1-row, col, &red, &green, &blue);
							}
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
//		std::cout << "Destination image has " << this->count(255,255,255) << " white pixels\n";
	}
}
