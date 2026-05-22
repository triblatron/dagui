//
// Created by Tony Horrobin on 02/05/2026.
//

#include "config/config.h"

#include "core/BoolEditor.h"
#include "core/EditorParameters.h"
#include "util/Searchable.h"

namespace dagui
{
    BoolEditor::BoolEditor(const BoolEditor &other)
        :
    _name(other._name),
    _typeName(other._typeName),
    _object(other._object),
    _prop(other._prop)
    {
        // Do nothing.
    }

    BoolEditor & BoolEditor::operator=(const BoolEditor &other)
    {
        if (this != &other)
        {
            _name = other._name;
            _typeName = other._typeName;
            _object = other._object;
            _prop = other._prop;
        }

        return *this;
    }

    BoolEditor::BoolEditor(const EditorParameters &params)
        :
    _typeName(params.typeName)
    {
        // Do nothing.
    }

    void BoolEditor::makeItSo()
    {
        // TODO.
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
