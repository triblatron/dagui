//
// Created by Tony Horrobin on 20/03/2025.
//

#include "config/config.h"

#include "core/ValidatorRegex.h"

namespace dagui
{
    void ValidatorRegex::filter(char nextChar)
    {
        _output += nextChar;
    }

    void ValidatorRegex::submit()
    {
        std::smatch match;
        if (std::regex_match(_output, match, _regex))
        {
            _valid = true;
        }
    }

    bool ValidatorRegex::isValid() const
    {
        return _valid;
    }
}
