//
// Created by Tony Horrobin on 17/04/2025.
//

#pragma once

#include "core/Widget.h"

namespace dagui
{

    class DAGUI_API Label : public Widget
    {
    public:
        explicit Label(Widget *parent);

        ~Label() override = default;

        void configure(dagbase::ConfigurationElement& config, WidgetFactory& factory) override;

        dagbase::ConfigurationElement::ValueType find(std::string_view path) const override;
    private:
        std::string _text;
    };
}