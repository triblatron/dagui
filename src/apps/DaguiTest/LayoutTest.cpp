//
// Created by Tony Horrobin on 17/05/2025.
//

#include "config/config.h"

#include "core/Variant.h"
#include "core/ConfigurationElement.h"
#include "core/LuaInterface.h"
#include "core/WidgetFactory.h"
#include "core/Widget.h"
#include "test/TestUtils.h"

#include <gtest/gtest.h>

class Constraint_testConfigure : public ::testing::TestWithParam<std::tuple<const char*, const char*, dagbase::Variant, double, dagbase::ConfigurationElement::RelOp>>
{

};

TEST_P(Constraint_testConfigure, testExpectedValues)
{
    auto configStr = std::get<0>(GetParam());
    dagbase::Lua lua;
    auto config = dagbase::ConfigurationElement::fromFile(lua, configStr);
    ASSERT_NE(nullptr, config);
    dagui::WidgetFactory factory;
    auto widget = factory.create(*config);
    ASSERT_NE(nullptr, widget);
    auto path = std::get<1>(GetParam());
    auto value = std::get<2>(GetParam());
    auto tolerance = std::get<3>(GetParam());
    auto op = std::get<4>(GetParam());
    auto actualValue = widget->find(path);
    assertComparison(value, actualValue, tolerance, op);
}

INSTANTIATE_TEST_SUITE_P(Constraint, Constraint_testConfigure, ::testing::Values(
        std::make_tuple("data/tests/Constraint/Growth.lua", "lookup.label1.constraints[0].firstItem", std::string("label1"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Constraint/Growth.lua", "lookup.label1.constraints[0].firstAttribute", std::string("Attribute::WIDTH"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Constraint/Growth.lua", "lookup.label1.constraints[0].relation", std::string("Relation::EQ"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Constraint/Growth.lua", "lookup.label1.constraints[0].constant", 1.0, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Constraint/Growth.lua", "lookup.label1.constraints[0].strength", std::string("Strength::REQUIRED"), 0.0, dagbase::ConfigurationElement::RELOP_EQ)
        ));