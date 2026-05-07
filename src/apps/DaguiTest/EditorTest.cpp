//
// Created by Tony Horrobin on 02/05/2026.
//

#include "config/config.h"]

#include "tui/EditorRegistryTUI.h"
#include "core/MetaClassRegistration.h"
#include "core/Editor.h"
#include "core/LuaInterface.h"
#include "core/ConfigurationElement.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "test/TestUtils.h"

class TestEditable
{
public:
    DAGBASE_DEFINE_PROPERTY(TestEditable, asBool, foo, setFoo);

    void setFoo(bool on)
    {
        _foo = on;
    }

    bool foo() const
    {
        return _foo;
    }

    static dagbase::Type& getType();

    static dagbase::MetaClassRegistration<TestEditable> registration;
private:
    bool _foo{false};
};

dagbase::Type& TestEditable::getType()
{
    DAGBASE_BEGIN_COMPOUND(TestEditable)
    DAGBASE_ADD_PROPERTY(TestEditable, foo, dagbase::Boolean)
    DAGBASE_END_COMPOUND()

    return type;
}

dagbase::MetaClassRegistration<TestEditable> registration(dagbase::Atom::intern("TestEditable"));


class EditorRegistry_testCreateEditor : public ::testing::TestWithParam<std::tuple<const char*, bool, const char*, dagbase::Variant, double, dagbase::ConfigurationElement::RelOp>>
{

};

TEST_P(EditorRegistry_testCreateEditor, testExpectedResult)
{
    auto typeName = std::get<0>(GetParam());
    auto editorExists = std::get<1>(GetParam());
    auto path = std::get<2>(GetParam());
    auto value = std::get<3>(GetParam());
    auto tolerance = std::get<4>(GetParam());
    auto op = std::get<5>(GetParam());

    dagui::EditorRegistryTUI sut;
    dagbase::Lua lua;
    auto config = dagbase::ConfigurationElement::fromFile(lua, "data/tests/EditorRegistry/std_editors.lua");
    ASSERT_NE(nullptr, config);
    sut.configure(*config);
    auto type = dagbase::TypeRegistry::getTypeRegistry().findType(dagbase::Atom::intern(typeName));
    ASSERT_NE(nullptr, type);
    auto actual = sut.findOrCreateEditor(*type);
    ASSERT_EQ(editorExists, actual!=nullptr);
    if (editorExists)
    {
        auto actualValue = actual->find(path);
        assertComparison(value, actualValue, tolerance, op);
    }
}

INSTANTIATE_TEST_SUITE_P(EditorRegistry, EditorRegistry_testCreateEditor, ::testing::Values(
    std::make_tuple("TestEditable", true, "children[0].type", std::string("Boolean"), 0.0, dagbase::ConfigurationElement::RELOP_EQ)
    ));