//
// Created by Tony Horrobin on 10/06/2026.
//

#ifndef DAGUI_BOOLEDITORIMGUI_H
#define DAGUI_BOOLEDITORIMGUI_H

#include "config/Export.h"

#include "core/BoolEditor.h"

namespace dagui
{
    class DAGUI_API BoolEditorImGui : public BoolEditor
    {
    public:
        BoolEditorImGui(const EditorParameters& params);

        BoolEditorImGui(const BoolEditorImGui& other);

        void makeItSo() override;

        BoolEditorImGui* clone() override;

        dagbase::Variant find(std::string_view path) const override;
    private:
        bool _value{false};
    };
}
#endif //DAGUI_BOOLEDITORIMGUI_H
