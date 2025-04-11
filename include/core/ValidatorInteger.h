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

        void filter(char nextChar) override;

        void submit(const std::string& input) override;

        bool isValid() const override
        {
            return _status == STATUS_OK;
        }

        Status status() const
        {
            return _status;
        }

        std::int64_t asInteger(const std::string& input) const;
    private:
        State _state{STATE_INITIAL};
        Status _status{STATUS_UNKNOWN};
    };
}
