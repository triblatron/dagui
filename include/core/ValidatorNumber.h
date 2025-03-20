//
// Created by Tony Horrobin on 20/03/2025.
//

#pragma once

#include "config/Export.h"

#include <string>

namespace dagui
{
    class DAGUI_API ValidatorNumber
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

        void filter(char nextChar);

        std::string output() const;

        void submit();

        bool isValid() const
        {
            return _state == STATE_FINISH;
        }
    private:
        State _state{STATE_INITIAL};
        std::string _output;
    };
}
