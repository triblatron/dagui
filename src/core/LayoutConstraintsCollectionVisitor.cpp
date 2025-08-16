//
// Created by Tony Horrobin on 01/06/2025.
//

#include "config/config.h"

#include "core/LayoutConstraintsCollectionVisitor.h"
#include "util/Searchable.h"
#include "core/Widget.h"

namespace dagui
{
    dagbase::Variant LayoutConstraintsCollectionVisitor::find(std::string_view path) const
    {
        dagbase::Variant retval;

        retval = dagbase::findEndpoint(path, "numConstraints", std::int64_t(numConstraints()));
        if (retval.has_value())
            return retval;

        retval = dagbase::findInternal(path, "constraints", _constraints);
        if (retval.has_value())
            return retval;
        
        return {};
    }

    LayoutConstraintsCollectionVisitor::LayoutConstraintsCollectionVisitor()
    {
        registerHandler(dagbase::Atom::intern("*"), [this](Widget& widget) {
            widget.eachConstraint([this](Constraint& constraint) {
                _constraints.a.emplace_back(constraint);
                return true;
            });
        });
    }
}
