#include "config/config.h"

#include "gfx/FontImageSource.h"
#include "util/enums.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "gfx/ImageDef.h"
#include "gfx/Image.h"

#include <iostream>
#include <cstring>

#include "gfx/GlyphImageDef.h"

#include "core/ConfigurationElement.h"

namespace dagui
{
	FontImageSource::FontImageSource(FT_Library library, const char* filename)
	:
	_errod(ERR_UNKNOWN),
	_face(nullptr)
	{
		int error = 0;
		
		error = FT_New_Face( library,
							 filename,
							 0,
							 &_face );
		if ( error == FT_Err_Unknown_File_Format )
		{
			std::cerr << "Unsupported font format, bailing\n";
			_errod = ERR_UNSUPPORTED_FORMAT;
			
			return;
		}
		else if ( error )
		{
			std::cerr << "Failed to open font file, bailing\n";
			_errod = ERR_FAILED_TO_OPEN_FONT;
			return;
		}
		error = FT_Set_Char_Size(
				_face,    /* handle to face object         */
				0,       /* char_width in 1/64 of points  */
				16*64,   /* char_height in 1/64 of points */
				300,     /* horizontal device resolution  */
				300 );   /* vertical device resolution    */
		if (!_ranges.empty())
		{
			_charcode = _ranges[0].first;
			_glyphIndex = FT_Get_Char_Index( _face, _charcode );
		}
		else
		{
			_charcode = FT_Get_First_Char(_face, &_glyphIndex);
		}
		_errod = ERR_OK;
	}

	void FontImageSource::configure(dagbase::ConfigurationElement& config)
	{
		if (auto element = config.findElement("ranges"); element != nullptr)
		{
			for (auto i=0; i<element->numChildren(); ++i)
			{
				auto child = element->child(i);

				std::uint32_t first=0, last=0;

				if (auto firstConfig = child->findElement("first"); firstConfig != nullptr)
				{
					first = firstConfig->asInteger();
				}

				if (auto lastConfig = child->findElement("last"); lastConfig != nullptr)
				{
					last = lastConfig->asInteger();
				}

				addRange(first, last);
			}
		}
	}

	bool FontImageSource::hasMore() const
	{
		return _glyphIndex!=0;
	}
	
	void copyGlyphToImage(FT_GlyphSlot glyph, dagui::Image* image)
	{
		for (auto row = 0; row<glyph->bitmap.rows; ++row)
		{
			for (auto col=0; col<glyph->bitmap.width; ++col)
			{
				auto intensity = glyph->bitmap.buffer[row*glyph->bitmap.width+col];

				image->set(glyph->bitmap.rows - 1 - row, col, intensity, intensity, intensity);
			}
		}
	}

	ImageDef* FontImageSource::item()
	{
		int error = FT_Load_Glyph(
		_face,          /* handle to face object */
		_glyphIndex,   /* glyph index           */
		0x0 );  /* load flags, see below */
		if (error)
		{
			std::cerr << "Error loading glyph\n";

			_errod = ERR_LOADING_GLYPH;
			return nullptr;
		}

		// if (_face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
		// {
		// 	std::cout << "Rendering glyph\n";
		//
		// 	error = FT_Render_Glyph( _face->glyph,   /* glyph slot  */
		// 							 FT_RENDER_MODE_NORMAL ); /* render mode */
		// 	if (error)
		// 	{
		// 		std::cerr << "Failed to render glyph, bailing\n";
		//
		// 		// _errod = ERR_FAILED_TO_RENDER_GLYPH;
		// 		return nullptr;
		// 	}
		// }

		auto* image = new GlyphImageDef(_face, _face->glyph->glyph_index, _face->glyph->bitmap.width, _face->glyph->bitmap.rows);//, 1, _face->glyph->bitmap.buffer);
		return image;
	}
	
	void FontImageSource::nextItem()
	{
		if (!_ranges.empty() && _rangeIndex < _ranges.size())
		{
			++_charcode;
			if (_charcode > _ranges[_rangeIndex].second)
			{
				++_rangeIndex;
				if (_rangeIndex<_ranges.size())
				{
					_charcode = _ranges[_rangeIndex].first;
				}
				else
				{
					_charcode = 0;
				}
			}
			_glyphIndex = FT_Get_Char_Index( _face, _charcode );
		}
		else if (_ranges.empty())
			_charcode = FT_Get_Next_Char(_face, _charcode, &_glyphIndex);
	}
	
	const char* FontImageSource::errorToString(Error err)
	{
		switch ( err )
		{
			ENUM_NAME(ERR_OK)
			ENUM_NAME(ERR_UNSUPPORTED_FORMAT)
			ENUM_NAME(ERR_FAILED_TO_OPEN_FONT)
			ENUM_NAME(ERR_LOADING_GLYPH)
			ENUM_NAME(ERR_FAILED_TO_RENDER_GLYPH)
			ENUM_NAME(ERR_UNKNOWN)
		}
		
		return "<error>";
	}
	
	FontImageSource::Error FontImageSource::parseError(const char* str)
	{
		TEST_ENUM(ERR_UNKNOWN, str)
		TEST_ENUM(ERR_OK, str)
		TEST_ENUM(ERR_UNSUPPORTED_FORMAT, str)
		TEST_ENUM(ERR_FAILED_TO_OPEN_FONT, str)
		TEST_ENUM(ERR_LOADING_GLYPH, str)
		TEST_ENUM(ERR_FAILED_TO_RENDER_GLYPH, str)
		
		return ERR_UNKNOWN;
	}

    std::size_t FontImageSource::estimateCount() const
    {
        return _face->num_glyphs>=0?std::size_t(_face->num_glyphs):0;
    }
}
