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

        void configure(dagbase::ConfigurationElement& config) override;

        void filter(char nextChar) override;

        void submit(const std::string& input) override;

        bool isValid() const override
        {
            return _state == STATE_FINISH;
        }

        Status status() const
        {
            return _status;
        }

        double asDouble(const std::string& input) const;
    private:
        State _state{STATE_INITIAL};
        Status _status{STATUS_OK};
    };
}
