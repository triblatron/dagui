//
// Created by Tony Horrobin on 30/04/2025.
//

#include "config/config.h"

#include "core/Text.h"

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
        return SceneNode::find(path);
    }
}