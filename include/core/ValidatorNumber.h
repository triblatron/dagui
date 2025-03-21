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

        enum Status : std::uint32_t
        {
            STATUS_OK,
            STATUS_ERR_SIGN,
            STATUS_ERR_INTEGER,
            STATUS_ERR_POINT,
            STATUS_ERR_EXPONENT_SIGN,
            STATUS_ERR_EXPONENT,
            STATUS_UNKNOWN
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

        Status status() const
        {
            return _status;
        }

        double asDouble() const;
    private:
        std::string _output;
        State _state{STATE_INITIAL};
        Status _status{STATUS_OK};
    };
}
