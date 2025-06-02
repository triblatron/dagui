//
// Created by Tony Horrobin on 17/05/2025.
//

#include "config/config.h"

#include "core/LayoutProperties.h"
#include "core/ConfigurationElement.h"
#include "util/enums.h"
#include "util/Searchable.h"

namespace dagui
{

    void LayoutProperties::configure(dagbase::ConfigurationElement &config)
    {
        dagbase::ConfigurationElement::readConfig<LayoutProperties::HorizontalAlignment>(config, "horizAlign", [](const char* str) {
            return LayoutProperties::parseHorizontalAlignment(str);
        }, &horizontalAlignment);
        }

    LayoutProperties::HorizontalAlignment LayoutProperties::parseHorizontalAlignment(const char *str)
    {
        TEST_ENUM(HORIZ_ALIGN_LEFT, str)
        TEST_ENUM(HORIZ_ALIGN_CENTRE, str)
        TEST_ENUM(HORIZ_ALIGN_RIGHT, str)

        return HORIZ_ALIGN_UNKNOWN;
    }

    dagbase::Variant LayoutProperties::find(std::string_view path) const
    {
        dagbase::Variant retval;

        retval = dagbase::findEndpoint(path, "horizAlign", std::int64_t(horizontalAlignment));
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "verticalAlign", std::int64_t(verticalAlignment));
        if (retval.has_value())
            return retval;

        return {};
    }
}
