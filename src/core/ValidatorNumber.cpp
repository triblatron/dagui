//
// Created by Tony Horrobin on 20/03/2025.
//

#include "config/config.h"

#include "core/ValidatorNumber.h"
#include <cctype>

namespace dagui
{
    void ValidatorNumber::filter(char nextChar)
    {
        switch (_state)
        {
        case STATE_INITIAL:
            if (std::isdigit(nextChar))
            {
                _output += nextChar;
                _state = STATE_INTEGER;
            }
            else if (nextChar == '+' || nextChar == '-')
            {
                _output += nextChar;
                _state = STATE_SIGN;
            }
            break;
        case STATE_SIGN:
            if (std::isdigit(nextChar))
            {
                _output += nextChar;
                _state = STATE_INTEGER;
            }
            break;
        case STATE_INTEGER:
            if (nextChar == '.')
            {
                _output += nextChar;
                _state = STATE_POINT;
            }
            else if (std::isdigit(nextChar))
            {
                _output += nextChar;
            }
            else if (nextChar == 'e' || nextChar == 'E')
            {
                _output += nextChar;
                _state = STATE_EXPONENT_SIGN;
            }
            break;
        case STATE_POINT:
            if (std::isdigit(nextChar))
            {
                _output += nextChar;
                _state = STATE_FRACTION;
            }
            else if (nextChar == 'e' || nextChar == 'E')
            {
                _output += nextChar;
                _state = STATE_EXPONENT_SIGN;
            }
            break;
        case STATE_FRACTION:
            if (std::isdigit(nextChar))
            {
                _output += nextChar;
            }
            else if (nextChar == 'e' || nextChar == 'E')
            {
                _output += nextChar;
                _state = STATE_EXPONENT_SIGN;
            }
            break;
        case STATE_EXPONENT_SIGN:
            if (nextChar == '+' || nextChar == '-')
            {
                _output += nextChar;
                _state = STATE_EXPONENT;
            }
            else if (std::isdigit(nextChar))
            {
                _output += nextChar;
                _state = STATE_EXPONENT;
            }
            break;
        case STATE_EXPONENT:
            if (std::isdigit(nextChar))
            {
                _output += nextChar;
            }
            break;
        default:
            break;
        }

    }

    std::string ValidatorNumber::output() const
    {
        return _output;
    }

    void ValidatorNumber::submit()
    {
        if (!_output.empty())
        {
            switch (_state)
            {
            case STATE_INTEGER:
            case STATE_POINT:
            case STATE_FRACTION:
                _state = STATE_FINISH;
                break;
            case STATE_EXPONENT:
                if (std::isdigit(_output.back()))
                {
                    _state = STATE_FINISH;
                }
                break;
            default:
                break;
            }
        }
    }

    double ValidatorNumber::asDouble() const
    {
        char* endPtr = nullptr;
        double value = std::strtod(_output.c_str(), &endPtr);
        if (endPtr == _output.c_str())
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return value;
    }
}
