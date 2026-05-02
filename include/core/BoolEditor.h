//
// Created by Tony Horrobin on 02/05/2026.
//

#pragma once

#include "config/Export.h"

#include "core/Atom.h"
#include "core/Editor.h"

namespace dagui
{
    class DAGUI_API BoolEditor : public Editor
    {
    public:
        BoolEditor(const BoolEditor& other);

        BoolEditor& operator= (const BoolEditor& other);

        BoolEditor(dagbase::Atom typeName);

        BoolEditor* clone() override;

        dagbase::Variant find(std::string_view path) const override;
    private:
        dagbase::Atom _typeName;
    };
}
