//
// Created by tony on 22/08/24.
//

#pragma once

#include "config/Export.h"

#include <cstdint>
#include <sstream>

namespace dagui
{
    template<typename T>
    class ValidatorInt
    {
    public:
        enum Error : std::uint32_t
        {
            //! The validation was successful.
            ERR_OK,
            //! The value was too low
            ERR_TOO_LOW,
            //! The value was too high
            ERR_TOO_HIGH
        };
    public:
        ValidatorInt(T minValue, T maxValue)
        :
        _minValue(minValue),
        _maxValue(maxValue)
        {

        }

        //! Validate the given input
        //! \retval ERR_OK if the input is valid
        //! \retval ERR_TOO_LOW if the value is too low
        //! \retval ERR_TOO_HIGH if the value is too high
        Error validate(const char* str) const
        {
            T value{0};
            std::istringstream istr(str);

            istr >> value;

            if (value<_minValue)
            {
                return ERR_TOO_LOW;
            }

            if (value>_maxValue)
            {
                return ERR_TOO_HIGH;
            }

            return ERR_OK;
        }
    private:
        T _minValue{0};
        T _maxValue{0};
    };
}