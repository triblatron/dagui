#pragma once

#include "config/Export.h"

#include <cstdint>
#include <cstdlib>
#include <config/macos_config.h>
//#if defined(HAVE_WINDOWS_H)
//#include <Windows.h>
//#endif // HAVE_WINDOWS_H
//#include <GL/gl.h>

namespace dagui
{
	class DAGUI_API Image
	{
	public:
		Image(std::uint32_t width, std::uint32_t height, std::uint32_t numComponents)
		:
		_width(width),
		_height(height),
		_numComponents(numComponents)
		{
			_buffer = new std::uint8_t[height*width*numComponents];
		}

		//! Does not own the buffer.
		Image(std::uint32_t width, std::uint32_t height, std::uint32_t numComponents, unsigned char* buffer);
		
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
		
		void set(std::uint32_t row, std::uint32_t col, std::uint8_t red, std::uint8_t green, std::uint8_t blue)
		{
			if (row < _height && col < _width)
			{
				_buffer[row*_width*_numComponents+col*_numComponents+0] = red;
				_buffer[row*_width*_numComponents+col*_numComponents+1] = green;
				_buffer[row*_width*_numComponents+col*_numComponents+2] = blue;
				_buffer[row*_width*_numComponents+col*_numComponents+3] = 255;
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
		std::uint8_t* _buffer{nullptr};
		bool _own{true};
	};
}
