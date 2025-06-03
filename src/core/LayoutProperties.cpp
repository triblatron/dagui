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
        dagbase::ConfigurationElement::readConfig<VerticalAlignment>(config, "vertAlign", [](const char* str) {
            return LayoutProperties::parseVerticalAlignment(str);
        }, &verticalAlignment);
        dagbase::ConfigurationElement::readConfig(config, "spacing", &spacing);
        if (auto element = config.findElement("padding"); element)
        {
            if (element->numChildren()==4)
            {
                padding.a[0].padding = float(element->child(0)->asDouble());
                padding.a[1].padding = float(element->child(1)->asDouble());
                padding.a[2].padding = float(element->child(2)->asDouble());
                padding.a[3].padding = float(element->child(3)->asDouble());
            }
        }
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

        retval = dagbase::findEndpoint(path, "spacing", spacing);
        if (retval.has_value())
            return retval;

        retval = dagbase::findInternal(path, "padding", padding);
        if (retval.has_value())
            return retval;

        return {};
    }

    LayoutProperties::VerticalAlignment LayoutProperties::parseVerticalAlignment(const char *str)
    {
        TEST_ENUM(VERTICAL_ALIGN_TOP, str)
        TEST_ENUM(VERTICAL_ALIGN_CENTRE, str)
        TEST_ENUM(VERTICAL_ALIGN_BOTTOM, str)

        return LayoutProperties::VERTICAL_ALIGN_UNKNOWN;
    }

    dagbase::Variant Padding::find(std::string_view path) const
    {
        dagbase::Variant retval;

        retval = dagbase::findEndpoint(path, "padding", padding);
        if (retval.has_value())
            return retval;

        return {};
    }
}
