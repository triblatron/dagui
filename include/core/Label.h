//
// Created by Tony Horrobin on 17/04/2025.
//

#pragma once

#include "core/Widget.h"
#include "ShapeFactory.h"

namespace dagui
{

    class DAGUI_API Label : public Widget
    {
    public:
        explicit Label(Widget *parent);

        ~Label() override = default;

        void setText(const std::string& text)
        {
            _text = text;
        }

        const std::string& text() const
        {
            return _text;
        }

        void configure(dagbase::ConfigurationElement &config, WidgetFactory &factory,
                       dagui::ShapeFactory &shapeFactory) override;

        dagbase::ConfigurationElement::ValueType find(std::string_view path) const override;

        void draw(Batcher& batcher) override;
    private:
        std::string _text;
        dagbase::Vec2 _bounds;
    };
}
