//
// Created by Tony Horrobin on 12/04/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Widget.h"

#include <string>

namespace dagui
{
    class DAGUI_API Button : public Widget
    {
    public:
        explicit Button(Widget* parent);

        void configure(dagbase::ConfigurationElement& config, WidgetFactory& factory) override;

        dagbase::ConfigurationElement::ValueType find(std::string_view path) const override;
    private:
        std::string _text;
    };
}