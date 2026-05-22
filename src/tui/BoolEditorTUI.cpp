//
// Created by Tony Horrobin on 22/05/2026.
//

#include "config/config.h"

#include "tui/BoolEditorTUI.h"
#include "core/EditorParameters.h"

#define Uses_TCheckBoxes
#define Uses_TSItem
#define Uses_TRect
#define Uses_TGroup

#include <tvision/tv.h>

namespace dagui
{
    BoolEditorTUI::BoolEditorTUI(TGroup *root, const EditorParameters &params)
        :
    BoolEditor(params),
    _root(root)
    {
    }

    BoolEditorTUI::BoolEditorTUI(const BoolEditorTUI &other)
        :
    BoolEditor(other),
    _root(other._root),
    _checkbox(other._checkbox)
    {
        // Do nothing.
    }

    void BoolEditorTUI::makeItSo()
    {
        TRect bounds(0, 0, 10, 1);
        _checkbox = new TCheckBoxes(bounds, new TSItem(name().value(), nullptr));
        _root->insert(_checkbox);
    }

    BoolEditorTUI * BoolEditorTUI::clone()
    {
        return new BoolEditorTUI(*this);
    }

    dagbase::Variant BoolEditorTUI::find(std::string_view path) const
    {
        return BoolEditor::find(path);
    }
}
