//
// Created by Tony Horrobin on 02/05/2026.
//

#pragma once

#include "config/Export.h"

#include "core/Editor.h"

#include "util/SearchableArray.h"
#include <vector>

namespace dagui
{
    class DAGUI_API CompoundEditor : public Editor
    {
    public:
        CompoundEditor() = default;

        CompoundEditor(const CompoundEditor& other);

        CompoundEditor& operator=(const CompoundEditor& other);

        Editor* clone() override;

        dagbase::Variant find(std::string_view path) const override;

        void addChild(Editor* child)
        {
            if (child)
            {
                _children.a.emplace_back(child);
            }
        }
    private:
        using ChildArray = dagbase::SearchableArray<std::vector<Editor*>>;
        ChildArray _children;
    };
}