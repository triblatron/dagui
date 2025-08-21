//
// Created by Tony Horrobin on 17/04/2025.
//

#pragma once

#include "core/Widget.h"
#include "core/ShapeFactory.h"
#include "core/Text.h"

namespace dagui
{
    class Rectangle;

    class DAGUI_API Label : public Widget
    {
    public:
        explicit Label(Widget *parent);

        ~Label() override = default;

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

        void draw(Batcher& batcher, GraphicsBackendFactory& factory) override;
    private:
        Shape* _face{nullptr};
        Text* _text{nullptr};
        dagbase::Vec2 _bounds;
    };
}
