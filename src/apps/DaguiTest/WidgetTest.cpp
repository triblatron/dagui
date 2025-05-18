//
// Created by Tony Horrobin on 06/04/2025.
//

#include "config/config.h"

#include "core/WidgetFactory.h"
#include "core/LuaInterface.h"
#include "core/ConfigurationElement.h"
#include "core/Widget.h"
#include "test/TestUtils.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class WidgetFactory_testCreate : public ::testing::TestWithParam<std::tuple<const char*, dagbase::ConfigurationElement*(*)(dagbase::Lua&, const char*), bool, const char*, dagbase::ConfigurationElement::ValueType, double, dagbase::ConfigurationElement::RelOp>>
{

};

TEST_P(WidgetFactory_testCreate, testExpectedTree)
{
    auto configStr = std::get<0>(GetParam());
    dagbase::Lua lua;
    auto createConfigFunc = std::get<1>(GetParam());
    auto config = (*createConfigFunc)(lua, configStr);
    ASSERT_NE(nullptr, config);
    dagui::WidgetFactory sut;
    auto tree = sut.create(*config);
    auto isNotNull = std::get<2>(GetParam());
    ASSERT_EQ(isNotNull, tree!=nullptr);
    auto path = std::get<3>(GetParam());
    auto value = std::get<4>(GetParam());
    auto tolerance = std::get<5>(GetParam());
    auto op = std::get<6>(GetParam());
    if (tree)
    {
        auto actualValue = tree->find(path);
        assertComparison(value, actualValue, tolerance, op);
    }
    delete tree;
    delete config;
}

INSTANTIATE_TEST_SUITE_P(WidgetFactory, WidgetFactory_testCreate, ::testing::Values(
    std::make_tuple("data/tests/WidgetFactory/empty.lua", &dagbase::ConfigurationElement::fromFile, false, "content", dagbase::ConfigurationElement::ValueType(), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/oneWindow.lua", &dagbase::ConfigurationElement::fromFile, true, "id", std::string("test"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/oneWindowWithChild.lua", &dagbase::ConfigurationElement::fromFile, true, "numChildren", std::int64_t(1), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/rootWithNestedChildren.lua", &dagbase::ConfigurationElement::fromFile, true, "lookup.test.title", std::string("Hello, Dagui!"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/rootWithNestedChildren.lua", &dagbase::ConfigurationElement::fromFile, true, "lookup.clickme.text", std::string("Click me"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/rootWithNestedChildren.lua", &dagbase::ConfigurationElement::fromFile, true, "lookup.root.numChildren", std::int64_t(1), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/windowWithLayout.lua", &dagbase::ConfigurationElement::fromFile, true, "lookup.vertical.numChildren", std::int64_t(2), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/windowWithLayout.lua", &dagbase::ConfigurationElement::fromFile, true, "lookup.label.text", std::string("A label"), 0.0, dagbase::ConfigurationElement::RELOP_EQ)
    ));

class Widget_testProperties : public ::testing::TestWithParam<std::tuple<const char*, const char*, dagbase::ConfigurationElement::ValueType, double, dagbase::ConfigurationElement::RelOp>>
{

};

TEST_P(Widget_testProperties, testExpectedValue)
{
    auto configStr = std::get<0>(GetParam());
    dagbase::Lua lua;
    auto config = dagbase::ConfigurationElement::fromFile(lua, configStr);
    ASSERT_NE(nullptr, config);
    dagui::WidgetFactory factory;
    auto sut = factory.create(*config);
    ASSERT_NE(nullptr, sut);
    auto path = std::get<1>(GetParam());
    auto value = std::get<2>(GetParam());
    auto tolerance = std::get<3>(GetParam());
    auto op = std::get<4>(GetParam());
    auto actualValue = sut->find(path);
    assertComparison(value, actualValue, tolerance, op);
}

INSTANTIATE_TEST_SUITE_P(Widget, Widget_testProperties, ::testing::Values(
        std::make_tuple("data/tests/Widget/Label.lua", "text", "test", 0.0, dagbase::ConfigurationElement::RELOP_EQ)
        ));

class WidgetRef_testResolve : public ::testing::TestWithParam<std::tuple<const char*, const char*, bool>>
{

};

TEST_P(WidgetRef_testResolve, testExpectedNotNull)
{
    auto configStr = std::get<0>(GetParam());
    auto id = std::get<1>(GetParam());
    auto notNull = std::get<2>(GetParam());
    dagbase::Lua lua;
    auto config = dagbase::ConfigurationElement::fromFile(lua, configStr);
    ASSERT_NE(nullptr, config);
    dagui::WidgetFactory factory;
    auto widget = factory.create(*config);
    ASSERT_NE(nullptr, widget);
    dagui::WidgetRef sut;
    sut.setId(dagbase::Atom::intern(id));
    sut.resolve(widget);
    auto actual = sut.ref();
    EXPECT_EQ(notNull, actual!=nullptr);
}

INSTANTIATE_TEST_SUITE_P(WidgetRef, WidgetRef_testResolve, ::testing::Values(
        std::make_tuple("data/tests/WidgetRef/Lookup.lua", "spoo", false),
        std::make_tuple("data/tests/WidgetRef/Lookup.lua", "label1", true)
        ));
