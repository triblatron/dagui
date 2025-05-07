//
// Created by Tony Horrobin on 14/04/2025.
//

#include "config/config.h"

#include "core/Border.h"

namespace dagui
{
    Border::Border(dagbase::IdentifierGenerator::Identifier id, Widget* widget)
        :
    SceneNode(dagbase::Atom::intern("Border"), id, widget)
    {
        // Do nothing.
    }

    void Border::accept(SceneNodeVisitor &visitor)
    {
        visitor.visit(*this);
    }

    dagbase::Variant Border::find(std::string_view path) const
    {
        return dagbase::Variant();
    }
}