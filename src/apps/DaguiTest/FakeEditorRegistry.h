//
// Created by Tony Horrobin on 09/05/2026.
//

#pragma once

#include "core/EditorRegistry.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/CompoundEditor.h"
#include "FakeBoolEditor.h"

class FakeEditorRegistry : public dagui::EditorRegistry
{
public:
    FakeEditorRegistry();

    ~FakeEditorRegistry();

    MOCK_METHOD(void, registerEditor, (const dagui::EditorParameters&), (override));
    MOCK_METHOD(void, registerCompoundEditor, (dagbase::Atom, dagui::CompoundEditor*), (override));
    MOCK_METHOD(dagui::Editor*, findEditor, (dagbase::Atom), (override));
    MOCK_METHOD(void, unregisterEditor, (dagbase::Atom), (override));
private:
    using TypeToEditorMap = std::map<dagbase::Atom, dagui::Editor*>;
    TypeToEditorMap _registry;
};
