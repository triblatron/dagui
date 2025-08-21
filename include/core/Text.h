//
// Created by Tony Horrobin on 30/04/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Shape.h"

#include <string>

namespace dagui
{
    struct FT_FaceRec_;
    class TextureAtlas;

    class DAGUI_API Text : public Shape
    {
    public:
        explicit Text();

        void setText(const std::string& text)
        {
            _text = text;
        }

        const std::string& text() const
        {
            return _text;
        }

        void configure(dagbase::ConfigurationElement& config, ShapeFactory& shapeFactory) override;

        bool isInside(float x, float y) override;

        void accept(ShapeVisitor& visitor) override;

        dagbase::Variant find(std::string_view path) const override;
    private:
        std::string _text;
        dagbase::Atom _face;
        TextureAtlas* _atlas{nullptr};
    };
}