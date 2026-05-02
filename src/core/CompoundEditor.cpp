//
// Created by Tony Horrobin on 02/05/2026.
//

#include "config/config.h"

#include "core/CompoundEditor.h"

#include "util/Searchable.h"

namespace dagui
{
    CompoundEditor::CompoundEditor(const CompoundEditor &other)
    {
        for (auto child : other._children)
        {
            addChild(child->clone());
        }
    }

    CompoundEditor & CompoundEditor::operator=(const CompoundEditor &other)
    {
        if (this!=&other)
        {
            _children.a.clear();

            for (auto child : other._children)
            {
                addChild(child->clone());
            }
        }

        return *this;
    }

    Editor * CompoundEditor::clone()
    {
        return new CompoundEditor(*this);
    }

    dagbase::Variant CompoundEditor::find(std::string_view path) const
    {
        dagbase::Variant retval;

        retval = dagbase::findInternal(path, "children", _children);
        if (retval.has_value())
            return retval;

        return {};
    }
}
