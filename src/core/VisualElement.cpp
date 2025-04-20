//
// Created by Tony Horrobin on 18/04/2025.
//

#include "config/config.h"

#include "core/VisualElement.h"

namespace dagui
{
    VisualElement::VisualElement(dagbase::Atom className)
    :
    _className(className)
    {
        // Do nothing.
    }

    dagbase::ConfigurationElement::ValueType VisualElement::find(std::string_view path) const
    {
        return {};
    }
}