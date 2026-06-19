//
// Created by Tony Horrobin on 02/05/2026.
//

#include "config/config.h"

#include "core/MetaClassRegistration.h"
#include "core/Editor.h"
#include "core/LuaInterface.h"
#include "core/ConfigurationElement.h"
#include "FakeEditorRegistry.h"
#include "test/TestUtils.h"
#include "core/TestEditable.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class Editor_testEdit : public ::testing::TestWithParam<std::tuple<const char*, bool, bool>>
{

};

TEST_P(Editor_testEdit, testExpectedResult)
{
    auto configFilename = std::get<0>(GetParam());
    auto typeName = dagbase::Atom::intern("TestEditable");
    auto initialValue = std::get<1>(GetParam());
    auto finalValue = std::get<2>(GetParam());
    dagbase::ConfigurationElement* config = nullptr;
    FakeEditorRegistry sut;
    {
        dagbase::Lua lua;

        config = dagbase::ConfigurationElement::fromFile(lua, configFilename);
        ASSERT_NE(nullptr, config);
    }
    EXPECT_CALL(sut, registerEditor(::testing::_)).Times(::testing::AnyNumber());
    EXPECT_CALL(sut, registerCompoundEditor(::testing::_, ::testing::_)).Times(::testing::AnyNumber());
    EXPECT_CALL(sut, findEditor(::testing::_)).Times(::testing::AnyNumber());
    sut.configure(*config);
    auto type = dagbase::TypeRegistry::getTypeRegistry().findType(typeName);
    ASSERT_NE(nullptr, type);
    auto editor = sut.findOrCreateEditor(*type);
    ASSERT_NE(nullptr, editor);
    auto* obj = new dagui::TestEditable;
    editor->setObject(obj);
    obj->setFoo(initialValue);
    editor->makeItSo();
    EXPECT_EQ(finalValue, obj->foo());
    delete editor;
}

INSTANTIATE_TEST_SUITE_P(Editor, Editor_testEdit, ::testing::Values(
    std::make_tuple("data/tests/EditorRegistry/std_editors.lua", false, true)
    ));