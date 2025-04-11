//
// Created by Tony Horrobin on 30/03/2025.
//

#include "config/config.h"

#include "core/ValidatorInteger.h"
#include <cctype>
#include <cstdlib>

namespace dagui
{
    void ValidatorInteger::filter(char nextChar)
    {
        switch (_state)
        {
        case STATE_INITIAL:
            if (std::isdigit(nextChar))
            {
                _state = STATE_INTEGER;
            }
            else if (nextChar == '-' || nextChar == '+')
            {
                _state = STATE_SIGN;
            }
            break;
        case STATE_SIGN:
            if (std::isdigit(nextChar))
            {
                _state = STATE_INTEGER;
            }
            break;
        case STATE_INTEGER:
            if (nextChar == '-' || nextChar == '+')
            {
                _status = STATUS_ERR_SIGN;
            }
            else if (nextChar == '.')
            {
                _status = STATUS_ERR_POINT;
            }
            else if (!isdigit(nextChar))
            {
                _status = STATUS_ERR_INTEGER;
            }
            break;
        default:
            break;
        }
    }

    void ValidatorInteger::submit(const std::string& input)
    {
        if (!input.empty())
        {
            switch (_state)
            {
            case STATE_SIGN:
                _status = STATUS_ERR_SIGN;
                break;
            case STATE_INTEGER:
                if (std::isdigit(input.back()))
                {
                    _status = STATUS_OK;
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

    std::int64_t ValidatorInteger::asInteger(const std::string& input) const
    {
        std::int64_t result = strtoll(input.c_str(), NULL, 10);
        return result;
    }
}
