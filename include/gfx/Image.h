#pragma once

#include "config/Export.h"

#include <cstdint>
#include <cstdlib>
#include <cstdint>

//#if defined(HAVE_WINDOWS_H)
//#include <Windows.h>
//#endif // HAVE_WINDOWS_H
//#include <GL/gl.h>

namespace dagui
{
	class DAGUI_API Image
	{
	public:
		enum Origin :  std::uint32_t
		{
			ORIGIN_UNKNOWN,
			ORIGIN_BOTTOM_LEFT,
			ORIGIN_TOP_LEFT
		};
	public:
		Image(std::uint32_t width, std::uint32_t height, std::uint32_t numComponents, Origin origin = ORIGIN_BOTTOM_LEFT)
		:
		_width(width),
		_height(height),
		_numComponents(numComponents),
		_origin(origin)
		{
			_buffer = new std::uint8_t[height*width*numComponents]{0};
		}

		//! Copies the buffer.
		Image(std::uint32_t width, std::uint32_t height, std::uint32_t numComponents, Origin origin, unsigned char* buffer);
		
		~Image()
		{
			if (_own)
			{
				delete [] _buffer;
			}
		}

		std::uint32_t width() const
		{
			return _width;
		}

		std::uint32_t height() const
		{
			return _height;
		}

		std::uint32_t numComponents() const
		{
			return _numComponents;
		}

		void setOrigin(Origin origin)
		{
			_origin = origin;
		}

		Origin origin() const
		{
			return _origin;
		}

		void set(std::uint32_t row, std::uint32_t col, std::uint8_t red, std::uint8_t green, std::uint8_t blue)
		{
			if (row < _height && col < _width)
			{
				switch (_numComponents)
				{
				case 1:
					_buffer[row*_width*_numComponents+col*_numComponents+0] = red;
					break;
				case 3:
					_buffer[row*_width*_numComponents+col*_numComponents+0] = red;
					_buffer[row*_width*_numComponents+col*_numComponents+1] = green;
					_buffer[row*_width*_numComponents+col*_numComponents+2] = blue;
					break;
				case 4:
					_buffer[row*_width*_numComponents+col*_numComponents+0] = red;
					_buffer[row*_width*_numComponents+col*_numComponents+1] = green;
					_buffer[row*_width*_numComponents+col*_numComponents+2] = blue;
					_buffer[row*_width*_numComponents+col*_numComponents+3] = 255;
					break;
				}
			}
		}

		void get(std::uint32_t row, std::uint32_t col, std::uint8_t* red, std::uint8_t* green, std::uint8_t* blue) const
		{
			if (row < _height && col < _width && red != nullptr && green != nullptr && blue != nullptr)
			{
				switch (_numComponents)
				{
					case 1:
						*red = _buffer[row*_width+col];
						*green = _buffer[row*_width+col];
						*blue = _buffer[row*_width+col];
						break;
					case 3:
						*red = _buffer[row*_width*_numComponents+col*_numComponents+0];
						*green = _buffer[row*_width*_numComponents+col*_numComponents+1];
						*blue = _buffer[row*_width*_numComponents+col*_numComponents+2];
						break;
				}
			}
		}
		
		bool find(std::uint8_t red, std::uint8_t green, std::uint8_t blue) const;

		std::size_t count(std::uint8_t red, std::uint8_t green, std::uint8_t blue) const;

		std::uint8_t* data()
		{
			return _buffer;
		}
		
		void copyFrom(std::uint32_t destRow, std::uint32_t destCol, const dagui::Image* source);
	private:
		std::uint32_t _width{0};
		std::uint32_t _height{0};
		std::uint32_t _numComponents{0};
		Origin _origin{ORIGIN_BOTTOM_LEFT};
		std::uint8_t* _buffer{nullptr};
		bool _own{true};
	};
}
