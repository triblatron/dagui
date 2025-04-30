//
// Created by Tony Horrobin on 29/04/2025.
//

#include "config/config.h"

#include "core/ConfigurationElement.h"
#include "core/LuaInterface.h"
#include "core/WidgetFactory.h"
#include "core/Widget.h"
#include "test/TestUtils.h"
#include "core/WidgetPattern.h"
#include "core/SceneNode.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class WidgetPattern_testMatch : public ::testing::TestWithParam<std::tuple<const char*, const char*, const char*, dagbase::ConfigurationElement::ValueType, double, dagbase::ConfigurationElement::RelOp>>
{

};

TEST_P(WidgetPattern_testMatch, testExpectedResult)
{
    auto patternConfigStr = std::get<0>(GetParam());
    dagbase::Lua lua;
    auto patternConfig = dagbase::ConfigurationElement::fromFile(lua, patternConfigStr);
    ASSERT_NE(nullptr, patternConfig);
    auto widgetConfigStr = std::get<1>(GetParam());
    auto widgetConfig = dagbase::ConfigurationElement::fromFile(lua, widgetConfigStr);
    ASSERT_NE(nullptr, widgetConfig);
    dagui::WidgetFactory factory;
    auto widgetTree = factory.create(*widgetConfig);
    ASSERT_NE(nullptr, widgetTree);
    dagui::WidgetPattern sut;
    sut.configure(*patternConfig);
    auto scene = sut.match(*widgetTree);
    ASSERT_NE(nullptr, scene);
    auto path = std::get<2>(GetParam());
    auto value = std::get<3>(GetParam());
    auto tolerance = std::get<4>(GetParam());
    auto op = std::get<5>(GetParam());
    auto actualValue = scene->find(path);
    assertComparison(value, actualValue, tolerance, op);
    delete widgetTree;
    delete widgetConfig;
    delete patternConfig;
}

INSTANTIATE_TEST_SUITE_P(WidgetPattern, WidgetPattern_testMatch, ::testing::Values(
        std::make_tuple("data/tests/PatternMatching/Patterns.lua", "data/tests/PatternMatching/Label.lua", "class", std::string("Group"), 0.0, dagbase::ConfigurationElement::RELOP_EQ)
        ));