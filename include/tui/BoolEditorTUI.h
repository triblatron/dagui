//
// Created by Tony Horrobin on 22/05/2026.
//

#pragma once

#include "config/Export.h"

#include "core/BoolEditor.h"

class TCheckBoxes;
class TGroup;

namespace dagui
{
    struct EditorParameters;

    class DAGUI_API BoolEditorTUI : public BoolEditor
    {
    public:
        BoolEditorTUI(TGroup* root, const EditorParameters& params);

        BoolEditorTUI(const BoolEditorTUI& other);

        void makeItSo() override;

        BoolEditorTUI* clone() override;

        dagbase::Variant find(std::string_view path) const override;
    private:
        TGroup* _root{nullptr};
        TCheckBoxes* _checkbox{nullptr};
    };
}