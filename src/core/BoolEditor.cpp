//
// Created by Tony Horrobin on 02/05/2026.
//

#include "config/config.h"

#include "core/BoolEditor.h"

#include "util/Searchable.h"

namespace dagui
{
    BoolEditor::BoolEditor(const BoolEditor &other)
        :
    _typeName(other._typeName)
    {
        // Do nothing.
    }

    BoolEditor & BoolEditor::operator=(const BoolEditor &other)
    {
        if (this != &other)
        {
            _typeName = other._typeName;
        }

        return *this;
    }

    BoolEditor::BoolEditor(dagbase::Atom typeName)
        :
    _typeName(typeName)
    {
        // Do nothing.
    }

    BoolEditor * BoolEditor::clone()
    {
        return new BoolEditor(*this);
    }

    dagbase::Variant BoolEditor::find(std::string_view path) const
    {
        dagbase::Variant retval;

        retval = dagbase::findEndpoint(path, "type", std::string("Boolean"));
        if (retval.has_value())
            return retval;

        return {};
    }
}
