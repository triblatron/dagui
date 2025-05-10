//
// Created by Tony Horrobin on 30/04/2025.
//

#include "config/config.h"

#include "core/SceneNode.h"
#include "util/Searchable.h"

namespace dagui
{
    SceneNode::SceneNode(const dagbase::Atom& className, dagbase::IdentifierGenerator::Identifier id, Widget* widget, SceneNode* parent)
            :
            _className(className),
            _id(id),
            _widget(widget),
            _parent(parent)
    {
        // Do nothing.
    }

    dagbase::Variant SceneNode::find(std::string_view path) const
    {
        dagbase::Variant retval;

        retval = dagbase::findEndpoint(path, "class", std::string(_className.value()));
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "parent", dagbase::Variant(_parent!=nullptr));
        if (retval.has_value())
            return retval;

        return {};
    }
}