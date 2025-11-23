#include "config/config.h"	

#include "core/Event.h"
#include "util/enums.h"
#include "core/ConfigurationElement.h"

#include <iostream>

namespace dagui
{
    void PointerEvent::configure(dagbase::ConfigurationElement &config)
    {
        dagbase::ConfigurationElement::readConfig(config, "x", &pos[0]);
        dagbase::ConfigurationElement::readConfig(config, "y", &pos[1]);
        dagbase::ConfigurationElement::readConfig<ButtonMask>(config, "buttons", parseButtonMask, &buttons);
    }

	dagui::Event::Event(Type type, ContentType data)
		:
		_type(type),
		_data(data)
	{
		// Do nothing.
	}

	bool Event::operator==(const Event& other) const
	{
		return _timestamp==other._timestamp && _type == other._type && _data == other._data;
	}

	void Event::configure(dagbase::ConfigurationElement& config)
	{
		dagbase::ConfigurationElement::readConfig<Type>(config, "type", Event::parseType, &_type);
        dagbase::ConfigurationElement::readConfig(config, "timestamp", &_timestamp);

		switch (_type)
		{
		case TYPE_POINTER_MOVE:
		case TYPE_POINTER_HOVER:
        case TYPE_BUTTON_PRESS:
        case TYPE_BUTTON_RELEASE:
        case TYPE_BUTTON_CLICK:
        case TYPE_BUTTON_DOUBLE_CLICK:
			{
				PointerEvent pointerEvent;

                pointerEvent.configure(config);
                _dataType = DATA_POINTER;
				_data = pointerEvent;

				break;
			}
        case TYPE_KEY_PRESS:
        case TYPE_KEY_RELEASE:
        case TYPE_KEY_CHORD:
            {
                KeyEvent keyEvent;

                keyEvent.configure(config);
                _dataType = DATA_KEY;
                _data = keyEvent;

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
			ENUM_NAME(TYPE_BUTTON_DOUBLE_CLICK)
			ENUM_NAME(TYPE_KEY_PRESS)
			ENUM_NAME(TYPE_KEY_RELEASE)
            ENUM_NAME(TYPE_KEY_CHORD)
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
		TEST_ENUM(TYPE_UNKNOWN, str)
		TEST_ENUM(TYPE_POINTER_MOVE, str)
		TEST_ENUM(TYPE_POINTER_HOVER, str)
		TEST_ENUM(TYPE_BUTTON_PRESS, str)
		TEST_ENUM(TYPE_BUTTON_RELEASE, str)
		TEST_ENUM(TYPE_BUTTON_CLICK, str)
		TEST_ENUM(TYPE_BUTTON_DOUBLE_CLICK, str)
		TEST_ENUM(TYPE_KEY_PRESS, str)
		TEST_ENUM(TYPE_KEY_RELEASE, str)
        TEST_ENUM(TYPE_KEY_CHORD, str)
		TEST_ENUM(TYPE_ENTER_WIDGET, str)
		TEST_ENUM(TYPE_LEAVE_WIDGET, str)
		TEST_ENUM(TYPE_DRAG_START, str)
		TEST_ENUM(TYPE_DRAGGING, str)
		TEST_ENUM(TYPE_DRAG_STOP, str)

		return TYPE_UNKNOWN;
	}

    std::string Event::typeMaskToString(Event::TypeMask value)
    {
        std::string retval;

        if (value == EVENT_NONE)
            return "EVENT_NONE";

        BIT_NAME(value, POINTER_MOVE_BIT, retval)
        BIT_NAME(value, POINTER_HOVER_BIT, retval)
        BIT_NAME(value, BUTTON_PRESS_BIT, retval)
        BIT_NAME(value, BUTTON_RELEASE_BIT, retval)
        BIT_NAME(value, BUTTON_CLICK_BIT, retval)
        BIT_NAME(value, BUTTON_DOUBLE_CLICK_BIT, retval)
        BIT_NAME(value, KEY_PRESS_BIT, retval)
        BIT_NAME(value, KEY_RELEASE_BIT, retval)
        BIT_NAME(value, KEY_CHORD_BIT, retval)
        BIT_NAME(value, ENTER_WIDGET_BIT, retval)
        BIT_NAME(value, LEAVE_WIDGET_BIT, retval)
        BIT_NAME(value, DRAG_START_BIT, retval)
        BIT_NAME(value, DRAGGING_BIT, retval)
        BIT_NAME(value, DRAG_STOP_BIT, retval)

        if (!retval.empty())
            retval = retval.substr(0, retval.length()-1);

        return retval;
    }

    Event::TypeMask Event::parseTypeMask(const std::string &str)
    {
        TypeMask retval = EVENT_NONE;

        TEST_BIT(POINTER_MOVE_BIT, str, retval)
        TEST_BIT(POINTER_HOVER_BIT, str, retval)
        TEST_BIT(BUTTON_PRESS_BIT, str, retval)
        TEST_BIT(BUTTON_RELEASE_BIT, str, retval)
        TEST_BIT(BUTTON_CLICK_BIT, str, retval)
        TEST_BIT(BUTTON_DOUBLE_CLICK_BIT, str, retval)
        TEST_BIT(KEY_PRESS_BIT, str, retval)
        TEST_BIT(KEY_RELEASE_BIT, str, retval)
        TEST_BIT(KEY_CHORD_BIT, str, retval)
        TEST_BIT(ENTER_WIDGET_BIT, str, retval)
        TEST_BIT(LEAVE_WIDGET_BIT, str, retval)
        TEST_BIT(DRAG_START_BIT, str, retval)
        TEST_BIT(DRAGGING_BIT, str, retval)
        TEST_BIT(DRAG_STOP_BIT, str, retval)

        return retval;
    }

    bool Event::matches(const Event &other) const
    {
        if (_type != other._type)
            return false;

        if (_dataType != other._dataType)
            return false;

        switch (_dataType)
        {
            case DATA_POINTER:
                break;
            case DATA_KEY:
            {
                auto& thisKeyEvent = std::get<KeyEvent>(_data);
                auto& otherKeyEvent = std::get<KeyEvent>(other._data);

                if ((thisKeyEvent.keys & otherKeyEvent.keys) == 0)
                    return false;

                break;
            }
        }
        return true;
    }

    bool PointerEvent::operator==(const PointerEvent& other) const
	{
		return x() == other.x() && y() == other.y() && buttons == other.buttons;
	}

	bool KeyEvent::operator==(const KeyEvent& other) const
	{
		return keys == other.keys;
	}

    void KeyEvent::configure(dagbase::ConfigurationElement &config)
    {
        dagbase::ConfigurationElement::readConfig<KeyMask>(config, "keys", &parseKeyMask, &keys);
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
        str << "Event { type: " << Event::typeToString(obj.type());
        str << ", timestamp: " << obj.timestamp();
        switch (obj.dataType())
        {
            case Event::DATA_POINTER:
            {
                const auto& data = std::get<PointerEvent>(obj.data());
                str << ", x: " << data.x() << ", y: " << data.y();
                str << ", buttonMask: " << buttonMaskToString(data.buttons);
                break;
            }
            case Event::DATA_KEY:
            {
                const auto& data = std::get<KeyEvent>(obj.data());
                str << ", keys: " << keyMaskToString(data.keys);
                break;
            }
            default:
                break;
        }
        str << " }";

        return str;
    }

    std::string keyMaskToString(KeyMask value)
    {
        std::string retval;

        BIT_NAME(value, KEY_LEFT_CTRL_BIT, retval)
        BIT_NAME(value, KEY_LEFT_SHIFT_BIT, retval)
        BIT_NAME(value, KEY_A_BIT, retval)

        if (!retval.empty() && retval.back() == ' ')
        {
            retval = retval.substr(0, retval.length()-1);
        }

        return retval;
    }

    KeyMask parseKeyMask(const std::string &str)
    {
        KeyMask value{0};

        TEST_BIT(KEY_LEFT_CTRL_BIT, str, value)
        TEST_BIT(KEY_LEFT_SHIFT_BIT, str, value)
        TEST_BIT(KEY_A_BIT, str, value)

        return value;
    }
}
