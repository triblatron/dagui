#include "config/config.h"	

#include "core/Event.h"
#include "util/enums.h"
#include "core/ConfigurationElement.h"

#include <iostream>

namespace dagui
{
	dagui::Event::Event(Type type, ContentType data)
		:
		_type(type),
		_data(data)
	{
		// Do nothing.
	}

	bool Event::operator==(const Event& other) const
	{
		return _type == other._type && _data == other._data;
	}

	void Event::configure(dagbase::ConfigurationElement& config)
	{
		dagbase::ConfigurationElement::readConfig<Type>(config, "type", Event::parseType, &_type);
		switch (_type)
		{
		case TYPE_POINTER_MOVE:
			{
				PointerEvent pointerEvent;
				dagbase::ConfigurationElement::readConfig(config, "x", &pointerEvent.x);
				dagbase::ConfigurationElement::readConfig(config, "y", &pointerEvent.y);
				_data = pointerEvent;

				break;
			}
        default:
            break;
		}
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

	bool PointerEvent::operator==(const PointerEvent& other) const
	{
		return x == other.x && y == other.y && buttons == other.buttons;
	}

	bool KeyEvent::operator==(const KeyEvent& other) const
	{
		return code == other.code;
	}

	bool WidgetEvent::operator==(const WidgetEvent& other) const
	{
		return widget == other.widget;
	}

	std::string buttonMaskToString(ButtonMask value)
	{
		std::string retval;

		BIT_NAME(value, BUTTON_PRIMARY_BIT, retval)
		BIT_NAME(value, BUTTON_SECONDARY_BIT, retval)
		BIT_NAME(value, BUTTON_MIDDLE_BIT, retval)

		if (!retval.empty())
		{
			retval = retval.substr(0, retval.length() - 1);

			return retval;
		}

		return "BUTTON_NONE";
	}

	ButtonMask parseButtonMask(const std::string& str)
	{
		ButtonMask retval = BUTTON_NONE;

		TEST_BIT(BUTTON_PRIMARY_BIT, str, retval);
		TEST_BIT(BUTTON_SECONDARY_BIT, str, retval);
		TEST_BIT(BUTTON_MIDDLE_BIT, str, retval);

		return retval;
	}

    std::ostream &operator<<(std::ostream &str, const Event &obj)
    {
        str << "Event {  type: " << Event::typeToString(obj.type()) << " }";

        return str;
    }
}
