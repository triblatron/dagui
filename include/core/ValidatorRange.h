//
// Created by Tony Horrobin on 30/03/2025.
//

#pragma once

#include "config/Export.h"
#include "Validator.h"
#include "core/ConfigurationElement.h"

#include <limits>
#include <string>

namespace dagui
{
    template<class T>
    class ValidatorRange : public dagui::Validator
    {
    public:
        void configure(dagbase::ConfigurationElement& config) override
        {
            if (auto element=config.findElement("minValue"); element)
            {
                _min = element->as<T>();
            }

            if (auto element=config.findElement("maxValue"); element)
            {
                _max = element->as<T>();
            }

            if (_max<_min)
            {
                _status = STATUS_ERR_MINMAX;
            }
        }

        void filter(char nextChar) override;

        bool isValid() const override;

        void submit(const std::string& input) override
        {
            std::istringstream iss(input);

            T value{0};

            iss >> value;

            if (value<_min)
            {
                _status = STATUS_ERR_TOO_LOW;
            }
            else if (value>_max)
            {
                _status = STATUS_ERR_TOO_HIGH;
            }

            _status = STATUS_OK;
        }
    private:
        T _min{std::numeric_limits<T>::lowest()};
        T _max{std::numeric_limits<T>::max()};
        Status _status{STATUS_UNKNOWN};
        Validator* _decoratee{ nullptr };
    };

    template <class T>
    void ValidatorRange<T>::filter(char nextChar)
    {
        if (_decoratee != nullptr)
        {
            _decoratee->filter(nextChar);
            if (!_decoratee->isValid())
            {
                return;
            }
        }
    }
}
