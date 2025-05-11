//
// Created by Tony Horrobin on 02/05/2025.
//

#include "config/config.h"

#include "core/ClipGroup.h"

namespace dagui
{
    ClipGroup::ClipGroup(dagbase::IdentifierGenerator::Identifier id, Widget *widget)
    :
    Group(dagbase::Atom::intern("ClipGroup"), id, widget)
    {
        // Do nothing.
    }

    dagbase::Variant ClipGroup::find(std::string_view path) const
    {
        dagbase::Variant retval = Group::find(path);
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "bounds", dagbase::Variant(_bounds));
        if (retval.has_value())
            return retval;

        return {};
    }
}
