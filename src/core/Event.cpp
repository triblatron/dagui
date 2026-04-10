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
        dagbase::ConfigurationElement::readConfig<std::bitset<128>>(config, "keys", &parseKey, &keys);
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

    const char * keyToString(Key value)
    {
        switch (value)
        {
            ENUM_NAME(KEY_A)
            ENUM_NAME(KEY_B)
            ENUM_NAME(KEY_C)
            ENUM_NAME(KEY_D)
            ENUM_NAME(KEY_E)
            ENUM_NAME(KEY_F)
            ENUM_NAME(KEY_G)
            ENUM_NAME(KEY_H)
            ENUM_NAME(KEY_I)
            ENUM_NAME(KEY_J)
            ENUM_NAME(KEY_K)
            ENUM_NAME(KEY_L)
            ENUM_NAME(KEY_M)
            ENUM_NAME(KEY_N)
            ENUM_NAME(KEY_O)
            ENUM_NAME(KEY_P)
            ENUM_NAME(KEY_Q)
            ENUM_NAME(KEY_R)
            ENUM_NAME(KEY_S)
            ENUM_NAME(KEY_T)
            ENUM_NAME(KEY_U)
            ENUM_NAME(KEY_V)
            ENUM_NAME(KEY_W)
            ENUM_NAME(KEY_X)
            ENUM_NAME(KEY_Y)
            ENUM_NAME(KEY_Z)
            ENUM_NAME(KEY_SPACE)
            ENUM_NAME(KEY_BACKSPACE)
            ENUM_NAME(KEY_TAB)
            ENUM_NAME(KEY_RETURN)
            ENUM_NAME(KEY_ESCAPE)
            ENUM_NAME(KEY_F1)
            ENUM_NAME(KEY_F2)
            ENUM_NAME(KEY_F3)
            ENUM_NAME(KEY_F4)
            ENUM_NAME(KEY_F5)
            ENUM_NAME(KEY_F6)
            ENUM_NAME(KEY_F7)
            ENUM_NAME(KEY_F8)
            ENUM_NAME(KEY_F9)
            ENUM_NAME(KEY_F10)
            ENUM_NAME(KEY_F11)
            ENUM_NAME(KEY_F12)
            ENUM_NAME(KEY_FN)
            ENUM_NAME(KEY_LEFT_OPTION)
            ENUM_NAME(KEY_RIGHT_OPTION)
            ENUM_NAME(KEY_SEMICOLON)
            ENUM_NAME(KEY_COMMA)
            ENUM_NAME(KEY_PERIOD)
            ENUM_NAME(KEY_SLASH)
            ENUM_NAME(KEY_BACKSLASH)
            ENUM_NAME(KEY_COLON)
            ENUM_NAME(KEY_SINGLE_QUOTE)
            ENUM_NAME(KEY_DOUBLE_QUOTE)
            ENUM_NAME(KEY_PIPE)
            ENUM_NAME(KEY_BACKTICK)
            ENUM_NAME(KEY_TIDLE)
            ENUM_NAME(KEY_1)
            ENUM_NAME(KEY_2)
            ENUM_NAME(KEY_3)
            ENUM_NAME(KEY_4)
            ENUM_NAME(KEY_5)
            ENUM_NAME(KEY_6)
            ENUM_NAME(KEY_7)
            ENUM_NAME(KEY_8)
            ENUM_NAME(KEY_9)
            ENUM_NAME(KEY_0)
            ENUM_NAME(KEY_MINUS)
            ENUM_NAME(KEY_EQUAL)
            ENUM_NAME(KEY_UNDERSCORE)
            ENUM_NAME(KEY_PLUS)
            ENUM_NAME(KEY_BANG)
            ENUM_NAME(KEY_AT)
            ENUM_NAME(KEY_HASH)
            ENUM_NAME(KEY_POUND)
            ENUM_NAME(KEY_DOLLAR)
            ENUM_NAME(KEY_PERCENT)
            ENUM_NAME(KEY_HAT)
            ENUM_NAME(KEY_AMPERSAND)
            ENUM_NAME(KEY_ASTERISK)
            ENUM_NAME(KEY_LEFT_PAREN)
            ENUM_NAME(KEY_RIGHT_PAREN)
            ENUM_NAME(KEY_LEFT_BRACKET)
            ENUM_NAME(KEY_RIGHT_BRACKET)
            ENUM_NAME(KEY_LEFT_BRACE)
            ENUM_NAME(KEY_RIGHT_BRACE)
            ENUM_NAME(KEY_LEFT_SHIT)
            ENUM_NAME(KEY_RIGHT_SHIFT)
            ENUM_NAME(KEY_LEFT_CTRL)
            ENUM_NAME(KEY_RIGHT_CTRL)
            ENUM_NAME(KEY_LEFT_ALT)
            ENUM_NAME(KEY_RIGHT_ALT)
            ENUM_NAME(KEY_LEFT_CMD)
            ENUM_NAME(KEY_RIGHT_CMD)
            ENUM_NAME(KEY_LEFT_ARROW)
            ENUM_NAME(KEY_RIGHT_ARROW)
            ENUM_NAME(KEY_UP_ARROW)
            ENUM_NAME(KEY_DOWN_ARROW)
            ENUM_NAME(KEY_UNKNOWN)
        }

        return "<error>";
    }

    Key parseKey(const char *str)
    {
        TEST_ENUM(KEY_A, str);
        TEST_ENUM(KEY_B, str);
        TEST_ENUM(KEY_C, str);
        TEST_ENUM(KEY_D, str);
        TEST_ENUM(KEY_E, str);
        TEST_ENUM(KEY_F, str);
        TEST_ENUM(KEY_G, str);
        TEST_ENUM(KEY_H, str);
        TEST_ENUM(KEY_I, str);
        TEST_ENUM(KEY_J, str);
        TEST_ENUM(KEY_K, str);
        TEST_ENUM(KEY_L, str);
        TEST_ENUM(KEY_M, str);
        TEST_ENUM(KEY_N, str);
        TEST_ENUM(KEY_O, str);
        TEST_ENUM(KEY_P, str);
        TEST_ENUM(KEY_Q, str);
        TEST_ENUM(KEY_R, str);
        TEST_ENUM(KEY_S, str);
        TEST_ENUM(KEY_T, str);
        TEST_ENUM(KEY_U, str);
        TEST_ENUM(KEY_V, str);
        TEST_ENUM(KEY_W, str);
        TEST_ENUM(KEY_X, str);
        TEST_ENUM(KEY_Y, str);
        TEST_ENUM(KEY_Z, str);
        TEST_ENUM(KEY_SPACE, str);
        TEST_ENUM(KEY_BACKSPACE, str);
        TEST_ENUM(KEY_TAB, str);
        TEST_ENUM(KEY_RETURN, str);
        TEST_ENUM(KEY_ESCAPE, str);
        TEST_ENUM(KEY_F1, str);
        TEST_ENUM(KEY_F2, str);
        TEST_ENUM(KEY_F3, str);
        TEST_ENUM(KEY_F4, str);
        TEST_ENUM(KEY_F5, str);
        TEST_ENUM(KEY_F6, str);
        TEST_ENUM(KEY_F7, str);
        TEST_ENUM(KEY_F8, str);
        TEST_ENUM(KEY_F9, str);
        TEST_ENUM(KEY_F10, str);
        TEST_ENUM(KEY_F11, str);
        TEST_ENUM(KEY_F12, str);
        TEST_ENUM(KEY_FN, str);
        TEST_ENUM(KEY_LEFT_OPTION, str);
        TEST_ENUM(KEY_RIGHT_OPTION, str);
        TEST_ENUM(KEY_SEMICOLON, str);
        TEST_ENUM(KEY_COMMA, str);
        TEST_ENUM(KEY_PERIOD, str);
        TEST_ENUM(KEY_SLASH, str);
        TEST_ENUM(KEY_BACKSLASH, str);
        TEST_ENUM(KEY_COLON, str);
        TEST_ENUM(KEY_SINGLE_QUOTE, str);
        TEST_ENUM(KEY_DOUBLE_QUOTE, str);
        TEST_ENUM(KEY_PIPE, str);
        TEST_ENUM(KEY_BACKTICK, str);
        TEST_ENUM(KEY_TIDLE, str);
        TEST_ENUM(KEY_1, str);
        TEST_ENUM(KEY_2, str);
        TEST_ENUM(KEY_3, str);
        TEST_ENUM(KEY_4, str);
        TEST_ENUM(KEY_5, str);
        TEST_ENUM(KEY_6, str);
        TEST_ENUM(KEY_7, str);
        TEST_ENUM(KEY_8, str);
        TEST_ENUM(KEY_9, str);
        TEST_ENUM(KEY_0, str);
        TEST_ENUM(KEY_MINUS, str);
        TEST_ENUM(KEY_EQUAL, str);
        TEST_ENUM(KEY_UNDERSCORE, str);
        TEST_ENUM(KEY_PLUS, str);
        TEST_ENUM(KEY_BANG, str);
        TEST_ENUM(KEY_AT, str);
        TEST_ENUM(KEY_HASH, str);
        TEST_ENUM(KEY_POUND, str);
        TEST_ENUM(KEY_DOLLAR, str);
        TEST_ENUM(KEY_PERCENT, str);
        TEST_ENUM(KEY_HAT, str);
        TEST_ENUM(KEY_AMPERSAND, str);
        TEST_ENUM(KEY_ASTERISK, str);
        TEST_ENUM(KEY_LEFT_PAREN, str);
        TEST_ENUM(KEY_RIGHT_PAREN, str);
        TEST_ENUM(KEY_LEFT_BRACKET, str);
        TEST_ENUM(KEY_RIGHT_BRACKET, str);
        TEST_ENUM(KEY_LEFT_BRACE, str);
        TEST_ENUM(KEY_RIGHT_BRACE, str);
        TEST_ENUM(KEY_LEFT_SHIT, str);
        TEST_ENUM(KEY_RIGHT_SHIFT, str);
        TEST_ENUM(KEY_LEFT_CTRL, str);
        TEST_ENUM(KEY_RIGHT_CTRL, str);
        TEST_ENUM(KEY_LEFT_ALT, str);
        TEST_ENUM(KEY_RIGHT_ALT, str);
        TEST_ENUM(KEY_LEFT_CMD, str);
        TEST_ENUM(KEY_RIGHT_CMD, str);
        TEST_ENUM(KEY_LEFT_ARROW, str);
        TEST_ENUM(KEY_RIGHT_ARROW, str);
        TEST_ENUM(KEY_UP_ARROW, str);
        TEST_ENUM(KEY_DOWN_ARROW, str);

        return KEY_UNKNOWN;
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

        BITSET_NAME(value, KEY_A, retval);
        BITSET_NAME(value, KEY_B, retval);
        BITSET_NAME(value, KEY_C, retval);
        BITSET_NAME(value, KEY_D, retval);
        BITSET_NAME(value, KEY_E, retval);
        BITSET_NAME(value, KEY_F, retval);
        BITSET_NAME(value, KEY_G, retval);
        BITSET_NAME(value, KEY_H, retval);
        BITSET_NAME(value, KEY_I, retval);
        BITSET_NAME(value, KEY_J, retval);
        BITSET_NAME(value, KEY_K, retval);
        BITSET_NAME(value, KEY_L, retval);
        BITSET_NAME(value, KEY_M, retval);
        BITSET_NAME(value, KEY_N, retval);
        BITSET_NAME(value, KEY_O, retval);
        BITSET_NAME(value, KEY_P, retval);
        BITSET_NAME(value, KEY_Q, retval);
        BITSET_NAME(value, KEY_R, retval);
        BITSET_NAME(value, KEY_S, retval);
        BITSET_NAME(value, KEY_T, retval);
        BITSET_NAME(value, KEY_U, retval);
        BITSET_NAME(value, KEY_V, retval);
        BITSET_NAME(value, KEY_W, retval);
        BITSET_NAME(value, KEY_X, retval);
        BITSET_NAME(value, KEY_Y, retval);
        BITSET_NAME(value, KEY_Z, retval);
        BITSET_NAME(value, KEY_SPACE, retval);
        BITSET_NAME(value, KEY_BACKSPACE, retval);
        BITSET_NAME(value, KEY_TAB, retval);
        BITSET_NAME(value, KEY_RETURN, retval);
        BITSET_NAME(value, KEY_ESCAPE, retval);
        BITSET_NAME(value, KEY_F1, retval);
        BITSET_NAME(value, KEY_F2, retval);
        BITSET_NAME(value, KEY_F3, retval);
        BITSET_NAME(value, KEY_F4, retval);
        BITSET_NAME(value, KEY_F5, retval);
        BITSET_NAME(value, KEY_F6, retval);
        BITSET_NAME(value, KEY_F7, retval);
        BITSET_NAME(value, KEY_F8, retval);
        BITSET_NAME(value, KEY_F9, retval);
        BITSET_NAME(value, KEY_F10, retval);
        BITSET_NAME(value, KEY_F11, retval);
        BITSET_NAME(value, KEY_F12, retval);
        BITSET_NAME(value, KEY_FN, retval);
        BITSET_NAME(value, KEY_LEFT_OPTION, retval);
        BITSET_NAME(value, KEY_RIGHT_OPTION, retval);
        BITSET_NAME(value, KEY_SEMICOLON, retval);
        BITSET_NAME(value, KEY_COMMA, retval);
        BITSET_NAME(value, KEY_PERIOD, retval);
        BITSET_NAME(value, KEY_SLASH, retval);
        BITSET_NAME(value, KEY_BACKSLASH, retval);
        BITSET_NAME(value, KEY_COLON, retval);
        BITSET_NAME(value, KEY_SINGLE_QUOTE, retval);
        BITSET_NAME(value, KEY_DOUBLE_QUOTE, retval);
        BITSET_NAME(value, KEY_PIPE, retval);
        BITSET_NAME(value, KEY_BACKTICK, retval);
        BITSET_NAME(value, KEY_TIDLE, retval);
        BITSET_NAME(value, KEY_1, retval);
        BITSET_NAME(value, KEY_2, retval);
        BITSET_NAME(value, KEY_3, retval);
        BITSET_NAME(value, KEY_4, retval);
        BITSET_NAME(value, KEY_5, retval);
        BITSET_NAME(value, KEY_6, retval);
        BITSET_NAME(value, KEY_7, retval);
        BITSET_NAME(value, KEY_8, retval);
        BITSET_NAME(value, KEY_9, retval);
        BITSET_NAME(value, KEY_0, retval);
        BITSET_NAME(value, KEY_MINUS, retval);
        BITSET_NAME(value, KEY_EQUAL, retval);
        BITSET_NAME(value, KEY_UNDERSCORE, retval);
        BITSET_NAME(value, KEY_PLUS, retval);
        BITSET_NAME(value, KEY_BANG, retval);
        BITSET_NAME(value, KEY_AT, retval);
        BITSET_NAME(value, KEY_HASH, retval);
        BITSET_NAME(value, KEY_POUND, retval);
        BITSET_NAME(value, KEY_DOLLAR, retval);
        BITSET_NAME(value, KEY_PERCENT, retval);
        BITSET_NAME(value, KEY_HAT, retval);
        BITSET_NAME(value, KEY_AMPERSAND, retval);
        BITSET_NAME(value, KEY_ASTERISK, retval);
        BITSET_NAME(value, KEY_LEFT_PAREN, retval);
        BITSET_NAME(value, KEY_RIGHT_PAREN, retval);
        BITSET_NAME(value, KEY_LEFT_BRACKET, retval);
        BITSET_NAME(value, KEY_RIGHT_BRACKET, retval);
        BITSET_NAME(value, KEY_LEFT_BRACE, retval);
        BITSET_NAME(value, KEY_RIGHT_BRACE, retval);
        BITSET_NAME(value, KEY_LEFT_SHIT, retval);
        BITSET_NAME(value, KEY_RIGHT_SHIFT, retval);
        BITSET_NAME(value, KEY_LEFT_CTRL, retval);
        BITSET_NAME(value, KEY_RIGHT_CTRL, retval);
        BITSET_NAME(value, KEY_LEFT_ALT, retval);
        BITSET_NAME(value, KEY_RIGHT_ALT, retval);
        BITSET_NAME(value, KEY_LEFT_CMD, retval);
        BITSET_NAME(value, KEY_RIGHT_CMD, retval);
        BITSET_NAME(value, KEY_LEFT_ARROW, retval);
        BITSET_NAME(value, KEY_RIGHT_ARROW, retval);
        BITSET_NAME(value, KEY_UP_ARROW, retval);
        BITSET_NAME(value, KEY_DOWN_ARROW, retval);

        if (!retval.empty() && retval.back() == ' ')
        {
            retval = retval.substr(0, retval.length()-1);
        }

        return retval;
    }

    KeyMask parseKeyMask(const std::string& str)
    {
        KeyMask value{0};

        TEST_BITSET(KEY_A, str, value);
        TEST_BITSET(KEY_B, str, value);
        TEST_BITSET(KEY_C, str, value);
        TEST_BITSET(KEY_D, str, value);
        TEST_BITSET(KEY_E, str, value);
        TEST_BITSET(KEY_F, str, value);
        TEST_BITSET(KEY_G, str, value);
        TEST_BITSET(KEY_H, str, value);
        TEST_BITSET(KEY_I, str, value);
        TEST_BITSET(KEY_J, str, value);
        TEST_BITSET(KEY_K, str, value);
        TEST_BITSET(KEY_L, str, value);
        TEST_BITSET(KEY_M, str, value);
        TEST_BITSET(KEY_N, str, value);
        TEST_BITSET(KEY_O, str, value);
        TEST_BITSET(KEY_P, str, value);
        TEST_BITSET(KEY_Q, str, value);
        TEST_BITSET(KEY_R, str, value);
        TEST_BITSET(KEY_S, str, value);
        TEST_BITSET(KEY_T, str, value);
        TEST_BITSET(KEY_U, str, value);
        TEST_BITSET(KEY_V, str, value);
        TEST_BITSET(KEY_W, str, value);
        TEST_BITSET(KEY_X, str, value);
        TEST_BITSET(KEY_Y, str, value);
        TEST_BITSET(KEY_Z, str, value);
        TEST_BITSET(KEY_SPACE, str, value);
        TEST_BITSET(KEY_BACKSPACE, str, value);
        TEST_BITSET(KEY_TAB, str, value);
        TEST_BITSET(KEY_RETURN, str, value);
        TEST_BITSET(KEY_ESCAPE, str, value);
        TEST_BITSET(KEY_F1, str, value);
        TEST_BITSET(KEY_F2, str, value);
        TEST_BITSET(KEY_F3, str, value);
        TEST_BITSET(KEY_F4, str, value);
        TEST_BITSET(KEY_F5, str, value);
        TEST_BITSET(KEY_F6, str, value);
        TEST_BITSET(KEY_F7, str, value);
        TEST_BITSET(KEY_F8, str, value);
        TEST_BITSET(KEY_F9, str, value);
        TEST_BITSET(KEY_F10, str, value);
        TEST_BITSET(KEY_F11, str, value);
        TEST_BITSET(KEY_F12, str, value);
        TEST_BITSET(KEY_FN, str, value);
        TEST_BITSET(KEY_LEFT_OPTION, str, value);
        TEST_BITSET(KEY_RIGHT_OPTION, str, value);
        TEST_BITSET(KEY_SEMICOLON, str, value);
        TEST_BITSET(KEY_COMMA, str, value);
        TEST_BITSET(KEY_PERIOD, str, value);
        TEST_BITSET(KEY_SLASH, str, value);
        TEST_BITSET(KEY_BACKSLASH, str, value);
        TEST_BITSET(KEY_COLON, str, value);
        TEST_BITSET(KEY_SINGLE_QUOTE, str, value);
        TEST_BITSET(KEY_DOUBLE_QUOTE, str, value);
        TEST_BITSET(KEY_PIPE, str, value);
        TEST_BITSET(KEY_BACKTICK, str, value);
        TEST_BITSET(KEY_TIDLE, str, value);
        TEST_BITSET(KEY_1, str, value);
        TEST_BITSET(KEY_2, str, value);
        TEST_BITSET(KEY_3, str, value);
        TEST_BITSET(KEY_4, str, value);
        TEST_BITSET(KEY_5, str, value);
        TEST_BITSET(KEY_6, str, value);
        TEST_BITSET(KEY_7, str, value);
        TEST_BITSET(KEY_8, str, value);
        TEST_BITSET(KEY_9, str, value);
        TEST_BITSET(KEY_0, str, value);
        TEST_BITSET(KEY_MINUS, str, value);
        TEST_BITSET(KEY_EQUAL, str, value);
        TEST_BITSET(KEY_UNDERSCORE, str, value);
        TEST_BITSET(KEY_PLUS, str, value);
        TEST_BITSET(KEY_BANG, str, value);
        TEST_BITSET(KEY_AT, str, value);
        TEST_BITSET(KEY_HASH, str, value);
        TEST_BITSET(KEY_POUND, str, value);
        TEST_BITSET(KEY_DOLLAR, str, value);
        TEST_BITSET(KEY_PERCENT, str, value);
        TEST_BITSET(KEY_HAT, str, value);
        TEST_BITSET(KEY_AMPERSAND, str, value);
        TEST_BITSET(KEY_ASTERISK, str, value);
        TEST_BITSET(KEY_LEFT_PAREN, str, value);
        TEST_BITSET(KEY_RIGHT_PAREN, str, value);
        TEST_BITSET(KEY_LEFT_BRACKET, str, value);
        TEST_BITSET(KEY_RIGHT_BRACKET, str, value);
        TEST_BITSET(KEY_LEFT_BRACE, str, value);
        TEST_BITSET(KEY_RIGHT_BRACE, str, value);
        TEST_BITSET(KEY_LEFT_SHIT, str, value);
        TEST_BITSET(KEY_RIGHT_SHIFT, str, value);
        TEST_BITSET(KEY_LEFT_CTRL, str, value);
        TEST_BITSET(KEY_RIGHT_CTRL, str, value);
        TEST_BITSET(KEY_LEFT_ALT, str, value);
        TEST_BITSET(KEY_RIGHT_ALT, str, value);
        TEST_BITSET(KEY_LEFT_CMD, str, value);
        TEST_BITSET(KEY_RIGHT_CMD, str, value);
        TEST_BITSET(KEY_LEFT_ARROW, str, value);
        TEST_BITSET(KEY_RIGHT_ARROW, str, value);
        TEST_BITSET(KEY_UP_ARROW, str, value);
        TEST_BITSET(KEY_DOWN_ARROW, str, value);

        return value;
    }
}
