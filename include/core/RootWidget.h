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

namespace dagui
{
    class DAGUI_API RootWidget : public dagui::Widget
    {
    public:
        RootWidget();

        void configure(dagbase::ConfigurationElement& config, WidgetFactory& factory) override;

        RootWidget* root() override
        {
            return this;
        }

        void addIdentified(Widget* widget) override;

        Widget* lookup(dagbase::Atom name) override;

        dagbase::ConfigurationElement::ValueType find(std::string_view path) const override;
    private:
        using WidgetLookup = dagbase::SearchableMapFromAtom<std::unordered_map<dagbase::Atom, Widget*>>;
        WidgetLookup _widgetLookup;
        glm::ivec2 _size{};
    };
}
