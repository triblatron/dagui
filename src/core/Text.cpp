//
// Created by Tony Horrobin on 30/04/2025.
//

#include "config/config.h"

#include "core/Text.h"
#include "util/Searchable.h"

namespace dagui
{

    Text::Text(Widget *widget)
    :
            SceneNode(dagbase::Atom::intern("Text"), widget)
    {
        // Do nothing.
    }

    dagbase::Variant Text::find(std::string_view path) const
    {
        auto retval = SceneNode::find(path);

        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "text", _text);
        if (retval.has_value())
            return retval;

        return {};
    }
}