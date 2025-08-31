#include "config/config.h"

#include "core/Event.h"
#include "util/enums.h"

namespace dagui
{
	dagui::Event::Event(Type type, ContentType data)
		:
		_type(type),
		_data(data)
	{
		// Do nothing.
	}

	const char* Event::typeToString(Type type)
	{
		switch (type)
		{
			ENUM_NAME(TYPE_UNKNOWN)
			ENUM_NAME(TYPE_POINTER_MOVE)
			ENUM_NAME(TYPE_POINTER_HOVER)
			ENUM_NAME(TYPE_BUTTON_PRESS)
			ENUM_NAME(TYPE_BUTTON_RELEASE)
			ENUM_NAME(TYPE_BUTTON_CLICK)
			ENUM_NAME(TYPE_KEY_PRESS)
			ENUM_NAME(TYPE_KEY_RELEASE)
			ENUM_NAME(TYPE_ENTER_WIDGET)
			ENUM_NAME(TYPE_LEAVE_WIDGET)
			ENUM_NAME(TYPE_DRAG_START)
			ENUM_NAME(TYPE_DRAGGING)
			ENUM_NAME(TYPE_DRAG_STOP)
		}

		return "<error>";
	}

	Event::Type Event::parseType(const char* str)
	{
		TEST_ENUM(TYPE_UNKNOWN, str);
		TEST_ENUM(TYPE_POINTER_MOVE, str);
		TEST_ENUM(TYPE_POINTER_HOVER, str);
		TEST_ENUM(TYPE_BUTTON_PRESS, str);
		TEST_ENUM(TYPE_BUTTON_RELEASE, str);
		TEST_ENUM(TYPE_BUTTON_CLICK, str);
		TEST_ENUM(TYPE_KEY_PRESS, str);
		TEST_ENUM(TYPE_KEY_RELEASE, str);
		TEST_ENUM(TYPE_ENTER_WIDGET, str);
		TEST_ENUM(TYPE_LEAVE_WIDGET, str);
		TEST_ENUM(TYPE_DRAG_START, str);
		TEST_ENUM(TYPE_DRAGGING, str);
		TEST_ENUM(TYPE_DRAG_STOP, str);

		return TYPE_UNKNOWN;
	}
}