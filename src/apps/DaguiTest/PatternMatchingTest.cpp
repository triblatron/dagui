//
// Created by Tony Horrobin on 29/04/2025.
//

#include "core/ConfigurationElement.h"
#include "core/LuaInterface.h"
#include "core/WidgetFactory.h"
#include "test/TestUtils.h"
#include "core/SceneNode.h"
#include "core/SceneNodeFactory.h"
#include "core/WidgetToSceneNodeConverter.h"
#include "core/ShapeFactory.h"

#include <gtest/gtest.h>

class WidgetPattern_testMatch : public ::testing::TestWithParam<std::tuple<const char*, const char*, const char*, dagbase::Variant, double, dagbase::ConfigurationElement::RelOp>>
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
    dagui::ShapeFactory shapeFactory;
    auto widgetTree = factory.create(*widgetConfig, shapeFactory);
    ASSERT_NE(nullptr, widgetTree);
    auto path = std::get<2>(GetParam());
    auto value = std::get<3>(GetParam());
    auto tolerance = std::get<4>(GetParam());
    auto op = std::get<5>(GetParam());
    auto actualValue = widgetTree->find(path);
    assertComparison(value, actualValue, tolerance, op);
    delete widgetTree;
    delete widgetConfig;
    delete patternConfig;
}

INSTANTIATE_TEST_SUITE_P(WidgetPattern, WidgetPattern_testMatch, ::testing::Values(
        std::make_tuple("data/tests/PatternMatching/Patterns.lua", "data/tests/PatternMatching/Label.lua", "class", std::string("Label"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/PatternMatching/Patterns.lua", "data/tests/PatternMatching/Label.lua", "bounds", dagbase::Vec2{20.0,20.0}, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/PatternMatching/Patterns.lua", "data/tests/PatternMatching/Label.lua", "text.class", std::string("Text"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/PatternMatching/Patterns.lua", "data/tests/PatternMatching/Label.lua", "text.text", std::string("Hello, world!"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/PatternMatching/Patterns.lua", "data/tests/PatternMatching/Label.lua", "bounds", dagbase::Vec2{20,20}, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/PatternMatching/Patterns.lua", "data/tests/PatternMatching/WindowWithLabel.lua", "children[0].parent", true, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/PatternMatching/Patterns.lua", "data/tests/PatternMatching/WindowWithLabel.lua", "children[0].text.text", std::string("test"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/PatternMatching/Patterns.lua", "data/tests/PatternMatching/WindowWithNestedChildren.lua", "children[0].class", std::string("Vertical"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/PatternMatching/Patterns.lua", "data/tests/PatternMatching/WindowWithNestedChildren.lua", "children[0].children[0].text.class", std::string("Text"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/PatternMatching/Patterns.lua", "data/tests/PatternMatching/WindowWithNestedChildren.lua", "children[0].children[0].text.text", std::string("test"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/PatternMatching/Patterns.lua", "data/tests/PatternMatching/WindowWithNestedChildren.lua", "children[0].children[1].text.text", std::string("Click me"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/PatternMatching/Patterns.lua", "data/tests/PatternMatching/WindowWithNestedChildren.lua", "children[0].children[1].class", std::string("Button"), 0.0, dagbase::ConfigurationElement::RELOP_EQ)
        ));
