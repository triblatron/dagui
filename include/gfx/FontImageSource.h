#include "config/Export.h"

#include "gfx/ImageSource.h"

#include <string>

#include <ft2build.h>

#include FT_FREETYPE_H

#include <vector>
#include <cstdint>
#include "core/Variant.h"

#include <string_view>

namespace dagbase
{
	class ConfigurationElement;
}

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
		explicit FontImageSource(FT_Library library);

		void configure(dagbase::ConfigurationElement& config);

        dagbase::Variant find(std::string_view path) const;

		bool ok() const
		{
			return _errod == ERR_OK;
		}

		FT_Face face()
		{
			return _face;
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
		using Ranges = std::vector<std::pair<std::uint32_t, std::uint32_t>>;
		Ranges _ranges;
		void addRange(std::uint32_t first, std::uint32_t last)
		{
			_ranges.emplace_back(std::make_pair(first, last));
		}
		size_t _rangeIndex{0};
		void init(FT_Library library, const char* filename);
		FT_Library _library;
	};
}
