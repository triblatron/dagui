//
// Created by Tony Horrobin on 18/04/2025.
//

#include "config/config.h"

#include "core/Group.h"
#include "util/Searchable.h"

namespace dagui
{

    Group::Group(Widget* widget)
    :
    VisualElement(dagbase::Atom::intern("Group"), widget)
    {
        // Do nothing.
    }

    dagbase::ConfigurationElement::ValueType Group::find(std::string_view path) const
    {
        dagbase::ConfigurationElement::ValueType retval = VisualElement::find(path);

        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "numChildren", std::int64_t(_children.a.size()));
        if (retval.has_value())
            return retval;

        retval = dagbase::findInternal(path, "children", _children);
        if (retval.has_value())
            return retval;

        return {};
    }
}
