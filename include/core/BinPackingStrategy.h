#pragma once

#include "config/Export.h"

namespace dagui
{
	class Atlas;
	class ImageSource;

	class DAGUI_API BinPackingStrategy
	{
	public:
		enum Result
		{
			//! The algorithm completed successfully.
			RESULT_OK,
			//! The algorithm failed to find space for an input item.
			RESULT_FAILED_TO_FIND_SPACE,
			//! Allocation of a rectangle in the atlas failed.
			RESULT_FAILED_TO_ALLOCATE,
			//! The algorithm has not yet been run.
			RESULT_UNKNOWN
		};
	public:
		virtual ~BinPackingStrategy() = default;

		//! \param[in] imageSource : ImageSource The source of input rectangles
		//! \param[in] atlas : Atlas The destination for allocated rectangles
		virtual void pack(ImageSource& imageSource, Atlas& atlas) = 0;

		Result result() const
		{
			return _result;
		}

		bool ok() const
		{
			return _result == RESULT_OK || _result == RESULT_UNKNOWN;
		}
	protected:
		void setResult(Result result)
		{
			_result = result;
		}
	private:
		Result _result{ RESULT_UNKNOWN };
	};
}
