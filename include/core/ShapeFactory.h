//
// Created by Tony Horrobin on 05/07/2025.
//

#pragma once

#include "config/Export.h"
#include "core/Atom.h"
#include "gfx/FontImageSource.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class FontImageSource;
    class Shape;
    class TextureAtlas;

    class DAGUI_API ShapeFactory
    {
    public:

    public:
        void setFreeTypeLib(FT_Library lib)
        {
            _freeTypeLib = lib;
        }

        void configure(dagbase::ConfigurationElement& config);

        void addFontImageSource(dagbase::Atom name, FontImageSource* source)
        {
            _fontImageSourceLookup.m.emplace(name, source);
        }

        FontImageSource* lookupFontImageSource(dagbase::Atom name)
        {
            if (auto it=_fontImageSourceLookup.m.find(name); it!=_fontImageSourceLookup.m.end())
            {
                return it->second;
            }

            return nullptr;
        }

        void addAtlas(dagbase::Atom name, TextureAtlas* atlas)
        {
            _atlasLookup.m.emplace(name, atlas);
        }

        TextureAtlas* lookupAtlas(dagbase::Atom name)
        {
            if (auto it=_atlasLookup.m.find(name); it!=_atlasLookup.m.end())
            {
                return it->second;
            }

            return nullptr;
        }
        Shape* createShape(dagbase::ConfigurationElement& config);
    private:
        FT_Library _freeTypeLib{nullptr};
        FontImageSourceLookup _fontImageSourceLookup;
        using AtlasLookup = dagbase::SearchableMapFromAtom<std::unordered_map<dagbase::Atom, TextureAtlas*>>;
        AtlasLookup _atlasLookup;
    };
}