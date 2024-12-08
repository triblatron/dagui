#include "config/Export.h"

#include "gfx/ImageSource.h"

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace dagui
{
	class DAGUI_API FontImageSource : public ImageSource
	{
	public:
		enum Error
		{
			ERR_OK,
			ERR_UNSUPPORTED_FORMAT,
			ERR_FAILED_TO_OPEN_FONT,
			ERR_LOADING_GLYPH,
			ERR_FAILED_TO_RENDER_GLYPH,
			ERR_UNKNOWN
		};
	public:
		FontImageSource(FT_Library library, const char* filename);
		
		bool ok() const
		{
			return _errod == ERR_OK;
		}

        std::size_t estimateCount() const override;

		bool hasMore() const override;
		
		ImageDef* item() override;
		
		void nextItem() override;
		
		static const char* errorToString(Error error);
		
		static Error parseError(const char* str);
	private:
		Error _errod{ERR_UNKNOWN};
		FT_Face _face{nullptr};
		FT_UInt _glyphIndex{0};
		FT_ULong _charcode{0};
	};
}
