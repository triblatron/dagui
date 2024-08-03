//
// Created by tony on 30/07/24.
//


#include "gtest/gtest.h"
#include "core/Rectangle.h"
#include "core/Container.h"
#include "core/Circle.h"
#include "core/ConfigurationElement.h"

class Rectangle_testIsInside : public ::testing::TestWithParam<std::tuple<double, double, double, double, double, double, double, bool>>
{

};

TEST_P(Rectangle_testIsInside, testIsInside)
{
    auto x = std::get<0>(GetParam());
    auto y = std::get<1>(GetParam());
    auto width = std::get<2>(GetParam());
    auto height = std::get<3>(GetParam());
    auto cornerRadius = std::get<4>(GetParam());
    auto hitX = std::get<5>(GetParam());
    auto hitY = std::get<6>(GetParam());
    auto inside = std::get<7>(GetParam());

    nfe::Rectangle sut;
    sut.setPos(x,y);
    sut.setSize(width,height);
    sut.setCornerRadius(cornerRadius);
    EXPECT_EQ(inside, sut.isInside(hitX,hitY));
}

INSTANTIATE_TEST_SUITE_P(Rectangle, Rectangle_testIsInside, ::testing::Values(
        // Outside bottom-right
        std::make_tuple(0.0, 0.0, 100.0, 100.0, 0.0, 200.0, 200.0, false),
        // Inside centre
        std::make_tuple(0.0, 0.0, 100.0, 100.0, 0.0, 50.0, 50.0, true),
        // Outside top-left
        std::make_tuple(0.0, 0.0, 100.0, 100.0, 2.0, 0.0, 0.0, false),
        // Inside top-left corner limit
        std::make_tuple(0.0, 0.0, 100.0, 100.0, 2.0, 2.0, 2.0, true),
        // Inside top-left corner
        std::make_tuple(0.0, 0.0, 100.0, 100.0, 3.0, 3.0, 2.0, true),
        // Top-centre
        std::make_tuple(0.0, 0.0, 100.0, 100.0, 2.0, 5.0, 1.0, true),
        // Left-centre
        std::make_tuple(0.0, 0.0, 100.0, 100.0, 2.0, 1.0, 5.0, true),
        // Right-centre
        std::make_tuple(0.0, 0.0, 100.0, 100.0, 2.0, 98.0, 5.0, true),
        // Bottom-centre
        std::make_tuple(0.0, 0.0, 100.0, 100.0, 2.0, 5.0, 98.0, true),
        // Top-right corner
        std::make_tuple(0.0, 0.0, 100.0, 100.0, 2.0, 98.0, 1.0, true),
        // Bottom-left corner
        std::make_tuple(0.0, 0.0, 100.0, 100.0, 2.0, 1.0, 98.0, true),
        // Bottom-right corner
        std::make_tuple(0.0, 0.0, 100.0, 100.0, 2.0, 98.0, 98.0, true)
        ));

class Circle_testIsInside : public ::testing::TestWithParam<std::tuple<double, double, double, double, double, bool>>
{

};

TEST_P(Circle_testIsInside, testIsInside)
{
    auto x = std::get<0>(GetParam());
    auto y = std::get<1>(GetParam());
    auto radius = std::get<2>(GetParam());
    auto hitX = std::get<3>(GetParam());
    auto hitY = std::get<4>(GetParam());
    auto inside = std::get<5>(GetParam());

    auto sut = new nfe::Circle();
    sut->setPos(x,y);
    sut->setRadius(radius);
    EXPECT_EQ(inside, sut->isInside(hitX,hitY));
    delete sut;
}

INSTANTIATE_TEST_SUITE_P(Cirlce, Circle_testIsInside, ::testing::Values(
        std::make_tuple(0, 0, 10, 10, 10, false),
        std::make_tuple(0, 0, 10, 0, 0, true)
        ));

TEST(Container, testEachChild)
{
    auto* parent = new nfe::Container();
    auto* child = new nfe::Widget();
    parent->addChild(child);
    std::size_t numChildren {0};
    parent->eachChild([&numChildren](nfe::Widget* child) {
       ++numChildren;
    });
    EXPECT_EQ(1,numChildren);

    // Parent owns child and will delete it.
    delete parent;
}

class ConfigurationElement_testFindElement : public ::testing::TestWithParam<std::tuple<const char*, const char*>>
{

};

TEST_P(ConfigurationElement_testFindElement, testFindFromRoot)
{
    auto configStr = std::get<0>(GetParam());
    auto name = std::get<1>(GetParam());

    auto config = nfe::ConfigurationElement::fromString(configStr);
    ASSERT_NE(nullptr, config);
    auto actual = config->findElement(name);
    ASSERT_NE(nullptr, actual);
}

INSTANTIATE_TEST_SUITE_P(ConfigurationElement, ConfigurationElement_testFindElement, ::testing::Values(
        std::make_tuple("root = {}", "$"),
        std::make_tuple("root = { foo = true }", "$.foo"),
        std::make_tuple("root = { foo = { wibble=1.0 } }", "$.foo.wibble"),
        std::make_tuple("root = { foo = { flibble={ spoo=3 } } }", "$.foo.flibble.spoo"),
        std::make_tuple("root = { foo = true }", "foo"),
        std::make_tuple("root = { foo = { bar=\"wibble\" } }", "foo.bar"),
        std::make_tuple("root = { foo = { bar={ baz=2 } } }", "foo.bar.baz")
        ));
