//
// Created by Tony Horrobin on 12/04/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Widget.h"
#include "ShapeFactory.h"

#include <string>
#include "core/Text.h"

namespace dagui
{
    class DAGUI_API Button : public Widget
    {
    public:
        explicit Button(Widget* parent);

        void setText(const std::string& text)
        {
            _text->setText(text);
        }

        const std::string& text() const
        {
            return _text->text();
        }

        void configure(dagbase::ConfigurationElement &config, WidgetFactory &factory,
                       dagui::ShapeFactory &shapeFactory) override;

        dagbase::ConfigurationElement::ValueType find(std::string_view path) const override;
    private:
        Text* _text{nullptr};
    };
}