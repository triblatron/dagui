//
// Created by Tony Horrobin on 05/07/2025.
//

#include "config/config.h"

#include "core/ShapeFactory.h"
#include "core/ConfigurationElement.h"
#include "core/Rectangle.h"
#include "core/Text.h"
#include "core/LuaInterface.h"
#include "core/BinPackingStrategyFactory.h"
#include "gfx/TextureAtlas.h"

#include <filesystem>

namespace dagui
{

    Shape *ShapeFactory::createShape(dagbase::ConfigurationElement &config)
    {
        Shape* retval = nullptr;
        std::string className;

        if (auto element = config.findElement("class"); element)
        {
            className = element->asString();
        }

        if (className == "Rectangle")
        {
            retval = new Rectangle();
        }
        else if (className == "Text")
        {
            retval = new Text();
        }

        if (retval)
        {
            retval->setFontImageSourceLookup(&_fontImageSourceLookup);
            retval->configure(config, *this);
        }

        return retval;
    }

    void ShapeFactory::configure(dagbase::ConfigurationElement &config)
    {
        dagui::BinPackingStrategyFactory factory;

        std::string strategyName;

        dagbase::ConfigurationElement::readConfig(config, "binPackingStrategyClass", &strategyName);

        auto binPacking = factory.createStrategy(strategyName);

        if (auto element = config.findElement("fontImageSources"); element)
        {
            element->eachChild([this, binPacking](dagbase::ConfigurationElement& child) {
                auto source = new FontImageSource(_freeTypeLib);
                dagbase::Atom name;
                dagbase::ConfigurationElement::readConfig(child, "name", &name);
                source->configure(child);
                addFontImageSource(name, source);
                auto atlas = new TextureAtlas();
                if (auto atlasConfig = child.findElement("atlas"); atlasConfig)
                    atlas->configure(*atlasConfig);
                atlas->setImageSource(source);
                atlas->pack(*binPacking);
                addAtlas(name, atlas);
                return true;
            });
        }
    }
}