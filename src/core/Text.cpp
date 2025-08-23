//
// Created by Tony Horrobin on 30/04/2025.
//

#include "config/config.h"

#include "core/Text.h"
#include "util/Searchable.h"
#include "core/ShapeVisitor.h"
#include "gfx/TextureAtlas.h"
#include "core/Tessellation.h"
#include "core/ShapeFactory.h"
#include "core/Batcher.h"
#include "core/GraphicsBackendFactory.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace dagui
{
    dagbase::Variant Text::find(std::string_view path) const
    {
        auto retval = Shape::find(path);

        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "text", _text);
        if (retval.has_value())
            return retval;

        return {};
    }

    Text::Text()
    :
    Shape(dagbase::Atom::intern("Text"))
    {
        // Do nothing.s
    }

    bool Text::isInside(float x, float y)
    {
        return false;
    }

    void Text::accept(ShapeVisitor &visitor)
    {
        //visitor.(*this);
    }

    void Text::configure(dagbase::ConfigurationElement &config, ShapeFactory& shapeFactory)
    {
        if (auto element = config.findElement("atlas"); element)
        {
            _atlas = new TextureAtlas();
            _atlas->configure(*element);
        }

        dagbase::Atom faceName;

        if (auto element = config.findElement("fontFace"); element)
        {
            faceName = dagbase::Atom::intern(element->asString());
        }

        if (!faceName.empty())
        {
            auto source = lookupFontImageSource(faceName);
            if (source)
            {
                _face = source->face();
            }
            _atlas = shapeFactory.lookupAtlas(faceName);
        }
        dagbase::ConfigurationElement::readConfig(config, "text", &_text);
    }

    void Text::drawTexturedQuad(Tessellation& tess, float x, float y, const ImageDef* imageDef)
    {
        tess.addVertex(x, y, 1.0f, 1.0f, 1.0f, 1.0, imageDef->p0.x, imageDef->p0.y);
        tess.addVertex(x+imageDef->width, y, 1.0f, 1.0f, 1.0f, 1.0f, imageDef->p1.x, imageDef->p1.y);
        tess.addVertex(x+imageDef->width, y+imageDef->height, 1.0f, 1.0f, 1.0f, 1.0, imageDef->p2.x, imageDef->p2.y);
        tess.addTriangle();

        tess.addVertex(x+imageDef->width, y+imageDef->height, 1.0f, 1.0f, 1.0f, 1.0f, imageDef->p2.x, imageDef->p2.y);
        tess.addVertex(x,y+imageDef->height, 1.0f, 1.0f, 1.0f, 1.0f, imageDef->p3.x, imageDef->p3.y);
        tess.addVertex(x, y, 1.0f, 1.0f, 1.0f, 1.0f, imageDef->p0.x, imageDef->p0.y);
        tess.addTriangle();
//        glTexCoord2f(imageDef->p2.x, imageDef->p2.y);
//        glVertex2f(x+imageDef->width,y+imageDef->height);
//        glTexCoord2f(imageDef->p3.x, imageDef->p3.y);
//        glVertex2f(x,y+imageDef->height);
//        glTexCoord2f(imageDef->p0.x, imageDef->p0.y);
//        glVertex2f(x,y);
    }

    void Text::generateTextureCoordinates(ImageDef& imageDef, BinImageDef& binImageDef)
    {
        imageDef.p0.x = float(imageDef.x)/float(binImageDef.width);
        imageDef.p0.y = float(imageDef.y)/float(binImageDef.height);
        imageDef.p1.x = float(imageDef.x+imageDef.width)/float(binImageDef.width);
        imageDef.p1.y = float(imageDef.y)/float(binImageDef.height);
        imageDef.p2.x = float(imageDef.x+imageDef.width)/float(binImageDef.width);
        imageDef.p2.y = float(imageDef.y+imageDef.height)/float(binImageDef.height);
        imageDef.p3.x = float(imageDef.x)/float(binImageDef.width);
        imageDef.p3.y = float(imageDef.y+imageDef.height)/float(binImageDef.height);
    }

    void Text::tessellate(ShapeMesh &mesh)
    {
        if (!isFlagSet(FLAGS_DIRTY_TESSELLATION_BIT))
            return;
        Tessellation tess;
        float x = 0, y = 0;
        tess.begin(Tessellation::PRIMTYPE_TRIANGLES);
        // For each glyph in the string
        for (std::size_t i = 0; i < _text.size(); i++)
        {
            FT_UInt glyphIndex = FT_Get_Char_Index(_face, _text[i]);
            ImageDef* imageDef = _atlas->imageForGlyphIndex(glyphIndex);
            if (imageDef)
            {
                generateTextureCoordinates(*imageDef, *_atlas->binImageDef());
                drawTexturedQuad(tess, x, y, imageDef);
                x+=imageDef->advance();
            }
        }
        // Get the texture coordinates for the glyph
        tess.end();
        tess.write(mesh);
        clearFlag(FLAGS_DIRTY_TESSELLATION_BIT);
    }

    void Text::allocateResources(Batcher &batcher, GraphicsBackendFactory& factory)
    {
        if (!isFlagSet(FLAGS_DIRTY_RESOURCES_BIT))
            return;

        _texture = batcher.allocateTexture();
        _atlas->setBackend(factory.createTextureAtlas(_atlas));

        clearFlag(FLAGS_DIRTY_RESOURCES_BIT);
    }
}