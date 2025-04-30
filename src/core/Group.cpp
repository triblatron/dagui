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
    SceneNode(dagbase::Atom::intern("Group"), widget)
    {
        // Do nothing.
    }

    dagbase::Variant Group::find(std::string_view path) const
    {
        dagbase::Variant retval = SceneNode::find(path);

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

    void Group::eachChild(std::function<bool(SceneNode *)> f)
    {
        if (f)
        {
            for (auto child : _children.a)
            {
                if (!f(child))
                    break;
            }
        }
    }
}
