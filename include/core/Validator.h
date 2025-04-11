//
// Created by Tony Horrobin on 29/03/2025.
//

#pragma once

#include "config/Export.h"

#include <cstdint>
#include <string>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class DAGUI_API Validator
    {
    public:
        enum Status : std::uint32_t
        {
            STATUS_OK,
            STATUS_ERR_EMPTY,
            STATUS_ERR_SIGN,
            STATUS_ERR_INTEGER,
            STATUS_ERR_POINT,
            STATUS_ERR_EXPONENT_SIGN,
            STATUS_ERR_EXPONENT,
            STATUS_ERR_TOO_LOW,
            STATUS_ERR_TOO_HIGH,
            STATUS_ERR_MINMAX,
            STATUS_UNKNOWN
        };
    public:
        Validator() = default;

        virtual ~Validator() = default;

        virtual void configure(dagbase::ConfigurationElement& config)
        {

        }

        virtual void filter(char nextChar) = 0;

        virtual void submit(const std::string& input) = 0;

        virtual bool isValid() const = 0;

        static const char* statusString(Status status);

        static Status parseStatus(const char* str);
    };
}