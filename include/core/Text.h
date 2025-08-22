//
// Created by Tony Horrobin on 30/04/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Shape.h"

#include <string>

struct FT_FaceRec_;

namespace dagui
{
    class BinImageDef;
    class ImageDef;
    class TextureAtlas;
    class Tessellation;

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

        RenderBinKey renderBinKey() const override
        {
            return {-1, _texture, -1, 0};
        }

        void allocateResources(Batcher& batcher) override;

        void tessellate(ShapeMesh& mesh) override;

        bool isInside(float x, float y) override;

        void accept(ShapeVisitor& visitor) override;

        dagbase::Variant find(std::string_view path) const override;
    private:
        std::string _text;
        FT_FaceRec_* _face{nullptr};
        TextureAtlas* _atlas{nullptr};
        std::int32_t _texture{-1};
        void drawTexturedQuad(Tessellation& tess, float x, float y, const ImageDef* imageDef);
        void generateTextureCoordinates(ImageDef& imageDef, BinImageDef& binImageDef);
    };
}