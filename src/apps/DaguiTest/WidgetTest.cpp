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
    std::make_tuple("data/tests/WidgetFactory/oneWindowWithChild.lua", &dagbase::ConfigurationElement::fromFile, true, "numChildren", std::uint32_t(1), 0.0, dagbase::ConfigurationElement::RELOP_EQ)
    ));
