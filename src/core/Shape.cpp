//
// Created by tony on 30/07/24.
//

#include "config/config.h"
#include "core/Shape.h"
#include "util/Searchable.h"

namespace dagui
{
    dagbase::Variant dagui::Shape::find(std::string_view path) const
    {
        dagbase::Variant retval;

        if (!_className.empty()) {
            retval = dagbase::findEndpoint(path, "class", std::string(_className.value()));
            if (retval.has_value())
                return retval;
        }

        return {};
    }

    Shape::Shape(dagbase::Atom className)
    :
    _className(className)
    {
        // Do nothing.
    }
}