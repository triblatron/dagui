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
            break;
        case STATE_POINT:
            if (std::isdigit(nextChar))
            {
                _output += nextChar;
                _state = STATE_FRACTION;
            }
            break;
        case STATE_FRACTION:
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
}