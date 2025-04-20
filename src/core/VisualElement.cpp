//
// Created by Tony Horrobin on 18/04/2025.
//

#include "config/config.h"

#include "core/VisualElement.h"
#include "util/Searchable.h"

namespace dagui
{
    VisualElement::VisualElement(dagbase::Atom className, Widget* widget)
    :
    _widget(widget),
    _className(className)
    {
        // Do nothing.
    }

    dagbase::ConfigurationElement::ValueType VisualElement::find(std::string_view path) const
    {
        dagbase::ConfigurationElement::ValueType retval;

        retval = dagbase::findEndpoint(path, "widget", _widget!=nullptr);
        if (retval.has_value())
            return retval;

        return {};
    }
}