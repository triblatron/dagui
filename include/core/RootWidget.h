//
// Created by Tony Horrobin on 09/04/2025.
//

#pragma once

#include "Widget.h"
#include "config/Export.h"
#include "util/Searchable.h"
#include <unordered_map>
#include <memory>

#include "gfx/ArrayDescriptor.h"
#include "gfx/ArrayDescriptor.h"
#include "util/SearchableMap.h"
#include "core/Atom.h"
#include "core/Style.h"
#include "ShapeFactory.h"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace dagui
{
    class FontImageSource;
    class Style;

    class DAGUI_API RootWidget : public dagui::Widget
    {
    public:
        RootWidget();

        void configure(dagbase::ConfigurationElement &config, WidgetFactory &factory,
                       dagui::ShapeFactory &shapeFactory) override;

        void setFreeTypeLib(FT_Library freeTypeLib)
        {
            _freeTypeLib = freeTypeLib;
        }

        RootWidget* root() override
        {
            return this;
        }

        void addIdentified(Widget* widget) override;

        void addStyle(dagbase::Atom name, Style* style)
        {
            _styles.m.emplace(name, style);
        }

        void addFontImageSource(dagbase::Atom name, FontImageSource* source)
        {
            _fontImageSourceLookup.m.emplace(name, source);
        }

        Widget* lookup(dagbase::Atom name) override;

        StyleLookup* styleLookup() override
        {
            return &_styles;
        }

        dagbase::ConfigurationElement::ValueType find(std::string_view path) const override;
    private:
        using WidgetLookup = dagbase::SearchableMapFromAtom<std::unordered_map<dagbase::Atom, Widget*>>;
        WidgetLookup _widgetLookup;
        FT_Library _freeTypeLib{nullptr};
        using FontImageSourceLookup = dagbase::SearchableMapFromAtom<std::unordered_map<dagbase::Atom, FontImageSource*>>;
        FontImageSourceLookup _fontImageSourceLookup;
        StyleLookup _styles;
        glm::ivec2 _size{};
    };
}
