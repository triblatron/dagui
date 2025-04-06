//
// Created by Tony Horrobin on 06/04/2025.
//

#include "config/config.h"

#include "core/WidgetFactory.h"
#include "core/LuaInterface.h"
#include "core/ConfigurationElement.h"
#include "core/Widget.h"
#include "TestUtils.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class WidgetFactory_testCreate : public ::testing::TestWithParam<std::tuple<const char*, dagbase::ConfigurationElement*(*)(dagbase::Lua&, const char*), const char*, dagbase::ConfigurationElement::ValueType, double, dagbase::ConfigurationElement::RelOp>>
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
    ASSERT_NE(nullptr, tree);
    auto path = std::get<2>(GetParam());
    auto value = std::get<3>(GetParam());
    auto tolerance = std::get<4>(GetParam());
    auto op = std::get<5>(GetParam());
    auto actualValue = tree->find(path);
    assertComparison(value, actualValue, tolerance, op);
    delete tree;
    delete config;
}

INSTANTIATE_TEST_SUITE_P(WidgetFactory, WidgetFactory_testCreate, ::testing::Values(
    std::make_tuple("data/tests/WidgetFactory/empty.lua", &dagbase::ConfigurationElement::fromFile, "content", dagbase::ConfigurationElement::ValueType(), 0.0, dagbase::ConfigurationElement::RELOP_EQ)
    ));