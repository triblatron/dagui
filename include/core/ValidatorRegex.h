//
// Created by Tony Horrobin on 20/03/2025.
//

#pragma once

#include "config/Export.h"

#include <string>
#include <regex>

namespace dagui
{
    class DAGUI_API ValidatorRegex
    {
    public:
        ValidatorRegex() = default;

        void setRegex(std::regex regex)
        {
            _regex = std::move(regex);
        }

        void filter(char nextChar);

        std::string output() const
        {
            return _output;
        }

        void submit();

        bool isValid() const;
    private:
        std::regex _regex;
        std::string _output;
        bool _valid{ false };
    };
}