//
// Created by Tony Horrobin on 20/03/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Validator.h"

#include <string>

namespace dagui
{
    class DAGUI_API ValidatorNumber : public dagui::Validator
    {
    public:
        enum State : std::uint32_t
        {
            STATE_INITIAL,
            STATE_SIGN,
            STATE_INTEGER,
            STATE_POINT,
            STATE_FRACTION,
            STATE_EXPONENT_SIGN,
            STATE_EXPONENT,
            STATE_FINISH
        };


    public:
        ValidatorNumber() = default;

        void setMinValue(double minValue)
        {
            _minValue = minValue;
        }

        void setMaxValue(double maxValue)
        {
            _maxValue = maxValue;
        }

        void filter(char nextChar);

        std::string output() const;

        void submit();

        bool isValid() const
        {
            return _state == STATE_FINISH;
        }

        Status status() const
        {
            return _status;
        }

        double asDouble() const;
    private:
        std::string _output;
        double _minValue{std::numeric_limits<double>::lowest()};
        double _maxValue{std::numeric_limits<double>::max()};
        State _state{STATE_INITIAL};
        Status _status{STATUS_OK};
    };
}
