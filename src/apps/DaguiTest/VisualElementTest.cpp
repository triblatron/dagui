//
// Created by Tony Horrobin on 18/04/2025.
//

#include "core/VisualElementFactory.h"
#include "core/ConfigurationElement.h"
#include "core/LuaInterface.h"
#include "core/WidgetFactory.h"
#include "core/VisualElementTreeBuilder.h"
#include "core/VisualElement.h"
#include "test/TestUtils.h"

#include <gtest/gtest.h>

class VisualElementFactory_testCreate : public ::testing::TestWithParam<std::tuple<const char*, bool>>
{

};

TEST_P(VisualElementFactory_testCreate, testExpectedReturn)
{
    auto className = dagbase::Atom::intern(std::get<0>(GetParam()));
    dagui::VisualElementFactory sut;
    auto actual = sut.createElement(className, nullptr);
    auto notNull = std::get<1>(GetParam());
    EXPECT_EQ(notNull, actual!=nullptr);
}

INSTANTIATE_TEST_SUITE_P(VisualElementFactory, VisualElementFactory_testCreate, ::testing::Values(
        std::make_tuple("Spoo", false),
        std::make_tuple("Group", true),
        std::make_tuple("Border", true)
        ));

class VisualElementBuilder_testBuild : public ::testing::TestWithParam<std::tuple<const char*, const char*, dagbase::ConfigurationElement::ValueType, double, dagbase::ConfigurationElement::RelOp>>
{
public:
    void SetUp()
    {
        _sut = new dagui::VisualElementTreeBuilder();
        _sut->setFactory(&_factory);
    }

    void TearDown()
    {
        delete _sut;
    }

    void configure(dagbase::ConfigurationElement& config)
    {
        if (_sut)
        {
            config.eachChild([this](dagbase::ConfigurationElement& child) {
                if (auto element=child.findElement("cmd"); element)
                {
                    std::string cmd = element->asString();

                    if (cmd == "beginElement")
                    {
                        std::string className;

                        if (auto classElement=child.findElement("class"); classElement)
                        {
                            className = classElement->asString();
                        }

                        _sut->beginElement(dagbase::Atom::intern(className), nullptr);
                    }
                    else if (cmd == "endElement")
                    {
                        _sut->endElement();
                    }
                }
                return true;
            });
        }
    }
protected:
    dagui::VisualElementTreeBuilder* _sut{nullptr};
    dagui::VisualElementFactory _factory;
};

TEST_P(VisualElementBuilder_testBuild, testExpectedTree)
{
    auto configStr = std::get<0>(GetParam());
    dagbase::Lua lua;
    dagbase::ConfigurationElement* config = dagbase::ConfigurationElement::fromFile(lua, configStr);
    ASSERT_NE(nullptr, config);

    configure(*config);
    auto visualTree = _sut->build();
    ASSERT_NE(nullptr, visualTree);
    auto path = std::get<1>(GetParam());
    auto value = std::get<2>(GetParam());
    auto tolerance = std::get<3>(GetParam());
    auto op = std::get<4>(GetParam());

    auto actualValue = visualTree->find(path);
    assertComparison(value, actualValue, tolerance, op);
}

INSTANTIATE_TEST_SUITE_P(VisualElementBuilder, VisualElementBuilder_testBuild, ::testing::Values(
        std::make_tuple("data/tests/VisualElement/root.lua", "numChildren", std::int64_t(0), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/VisualElement/withChildren.lua", "numChildren", std::int64_t(1), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/VisualElement/withMultipleChildren.lua", "numChildren", std::int64_t(2), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/VisualElement/withNestedChildren.lua", "children[0].numChildren", std::int64_t(1), 0.0, dagbase::ConfigurationElement::RELOP_EQ)
        ));