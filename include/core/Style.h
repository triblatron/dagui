//
// Created by Tony Horrobin on 08/06/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Variant.h"
#include "core/Reference.h"
#include "core/Atom.h"
#include "util/Searchable.h"
#include "util/SearchableMap.h"

#include <string_view>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class DAGUI_API Style
    {
    public:
        void configure(dagbase::ConfigurationElement& config);

        dagbase::Variant find(std::string_view path) const;
    private:
        float _border{0.0f};
    };

    using StyleLookup = dagbase::SearchableMapFromAtom<std::unordered_map<dagbase::Atom, Style*>>;
    using StyleRef = dagbase::Reference<dagbase::Atom, Style, StyleLookup>;
}
