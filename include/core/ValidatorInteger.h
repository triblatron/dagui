//
// Created by Tony Horrobin on 30/03/2025.
//

#pragma once

#include <limits>

#include "Validator.h"
#include "config/Export.h"

#include <cstdint>
#include <limits>

#include <string>

namespace dagui
{
    class DAGUI_API ValidatorInteger : public dagui::Validator
    {
    public:
        enum State : std::uint32_t
        {
            STATE_INITIAL,
            STATE_SIGN,
            STATE_INTEGER,
            STATE_FINISH
        };
    public:
        ValidatorInteger() = default;

        void setMinValue(std::int64_t minValue)
        {
            _minValue = minValue;
        }

        void setMaxValue(std::int64_t maxValue)
        {
            _maxValue = maxValue;
        }

        void filter(char nextChar);

        void submit();

        bool valid() const
        {
            return _status == STATUS_OK;
        }

        Status status() const
        {
            return _status;
        }

        std::int64_t asInteger() const;
    private:
        std::int64_t _minValue = std::numeric_limits<std::int64_t>::min();
        std::int64_t _maxValue = std::numeric_limits<std::int64_t>::max();
        State _state{STATE_INITIAL};
        std::string _output;
        Status _status{STATUS_UNKNOWN};
    };
}
