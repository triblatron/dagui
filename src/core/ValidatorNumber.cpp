//
// Created by Tony Horrobin on 20/03/2025.
//

#include "config/config.h"

#include "core/ValidatorNumber.h"
#include <cctype>
#include <limits>

namespace dagui
{
    void ValidatorNumber::configure(dagbase::ConfigurationElement& config)
    {
    }

    void ValidatorNumber::filter(char nextChar)
    {
        switch (_state)
        {
        case STATE_INITIAL:
            if (std::isdigit(nextChar))
            {
                _state = STATE_INTEGER;
            }
            else if (nextChar == '+' || nextChar == '-')
            {
                _state = STATE_SIGN;
            }
            break;
        case STATE_SIGN:
            if (std::isdigit(nextChar))
            {
                _state = STATE_INTEGER;
            }
            else if (nextChar == '+' || nextChar == '-')
            {
                _status = STATUS_ERR_SIGN;
            }
            break;
        case STATE_INTEGER:
            if (nextChar == '.')
            {
                _state = STATE_POINT;
            }
            else if (nextChar == 'e' || nextChar == 'E')
            {
                _state = STATE_EXPONENT_SIGN;
            }
            break;
        case STATE_POINT:
            if (std::isdigit(nextChar))
            {
                _state = STATE_FRACTION;
            }
            else if (nextChar == 'e' || nextChar == 'E')
            {
                _state = STATE_EXPONENT_SIGN;
            }
            else if (nextChar == '.')
            {
                _status = STATUS_ERR_POINT;
            }
            break;
        case STATE_FRACTION:
            if (nextChar == 'e' || nextChar == 'E')
            {
                _state = STATE_EXPONENT_SIGN;
            }
            break;
        case STATE_EXPONENT_SIGN:
            if (nextChar == '+' || nextChar == '-')
            {
                _state = STATE_EXPONENT;
            }
            else if (std::isdigit(nextChar))
            {
                _state = STATE_EXPONENT;
            }
            else if (nextChar == 'e' || nextChar == 'E')
            {
                _status = STATUS_ERR_EXPONENT_SIGN;
            }
            break;
        case STATE_EXPONENT:
            if (nextChar == '+' || nextChar == '-')
            {
                _status = STATUS_ERR_EXPONENT_SIGN;
            }
            break;
        default:
            break;
        }

    }

    void ValidatorNumber::submit(const std::string& input)
    {
        if (!input.empty())
        {
            switch (_state)
            {
            case STATE_INTEGER:
            case STATE_POINT:
            case STATE_FRACTION:
                if (_status == STATUS_OK)
                    _state = STATE_FINISH;
                break;
            case STATE_EXPONENT:
                if (std::isdigit(input.back()) && _status == STATUS_OK)
                {
                    _state = STATE_FINISH;
                }
                else if (input.back() == '+' || input.back() == '-')
                {
                    _status = STATUS_ERR_EXPONENT;
                }
                break;
            default:
                break;
            }
        }
        else
        {
            _status = STATUS_ERR_EMPTY;
        }
    }

    double ValidatorNumber::asDouble(const std::string& input) const
    {
        char* endPtr = nullptr;
        double value = std::strtod(input.c_str(), &endPtr);
        if (endPtr == input.c_str())
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return value;
    }
}
