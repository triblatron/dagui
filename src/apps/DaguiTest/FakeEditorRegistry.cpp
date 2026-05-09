//
// Created by Tony Horrobin on 09/05/2026.
//

#include "FakeEditorRegistry.h"

FakeEditorRegistry::FakeEditorRegistry()
{
    ON_CALL(*this, registerEditor(::testing::_)).WillByDefault([this](const dagui::EditorParameters& params) {
        FakeBoolEditor* editor = nullptr;

        if (params.className == "BoolEditor")
        {
            editor = new FakeBoolEditor(params);
        }

        if (editor)
        {
            _registry.emplace(params.typeName, editor);
            EXPECT_CALL(*editor, clone()).Times(1);
            EXPECT_CALL(*editor, setProperty(::testing::_)).Times(::testing::AnyNumber());
        }
    });
    ON_CALL(*this, registerCompoundEditor(::testing::_,::testing::_)).WillByDefault([this](dagbase::Atom typeName, dagui::CompoundEditor* editor) {
        if (editor)
            _registry.emplace(typeName, editor);
    });
    ON_CALL(*this, findEditor(::testing::_)).WillByDefault([this](dagbase::Atom typeName) {
        if (auto it=_registry.find(typeName); it != _registry.end())
            return it->second;

        return static_cast<dagui::Editor*>(nullptr);
    });
    ON_CALL(*this, unregisterEditor(::testing::_)).WillByDefault([this](dagbase::Atom typeName) {
        if (auto it=_registry.find(typeName); it != _registry.end())
            _registry.erase(it);
    });
}

FakeEditorRegistry::~FakeEditorRegistry()
{
    for (auto it=_registry.begin(); it!=_registry.end(); ++it)
        delete it->second;
}
