//
// Created by Tony Horrobin on 08/06/2025.
//

#include "config/config.h"

#include "core/Style.h"

#include "core/ConfigurationElement.h"
#include "util/Searchable.h"

namespace dagui
{

    void Style::configure(dagbase::ConfigurationElement &config)
    {
        dagbase::ConfigurationElement::readConfig(config, "border", &_border);
    }

    dagbase::Variant Style::find(std::string_view path) const
    {
        dagbase::Variant retval;

        retval = dagbase::findEndpoint(path, "border", _border);
        if (retval.has_value())
            return retval;

        return {};
    }

    dagbase::Variant StyleLookup::find(std::string_view path) const
    {
        return dagbase::findMapFromAtom(path, _lookup.m);
    }
}