//
// Created by tony on 30/07/24.
//


#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "core/Rectangle.h"
#include "core/Container.h"
#include "core/Circle.h"
#include "core/ConfigurationElement.h"
#include "util/CompletionTrie.h"
#include "util/CompletionSubstring.h"
#include "core/CompositeShape.h"
#include "core/ShapeVisitor.h"
#include "core/Window.h"
#include "core/ValidatorInt.h"

#include <memory>
#include <cstdint>

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

    dagui::Rectangle sut;
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

    auto sut = new dagui::Circle();
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
    auto* parent = new dagui::Container();
    auto* child = new dagui::Widget();
    parent->addChild(child);
    std::size_t numChildren {0};
    parent->eachChild([&numChildren](dagui::Widget* child) {
       ++numChildren;
    });
    EXPECT_EQ(1,numChildren);

    // Parent owns child and will delete it.
    delete parent;
}

class ConfigurationElement_testFindElement : public ::testing::TestWithParam<std::tuple<const char*, const char*, const char*>>
{

};

TEST_P(ConfigurationElement_testFindElement, testFindFromRoot)
{
    auto configStr = std::get<0>(GetParam());
    auto path = std::get<1>(GetParam());
    auto name = std::get<2>(GetParam());
    auto config = dagui::ConfigurationElement::fromString(configStr);
    ASSERT_NE(nullptr, config);
    auto actual = config->findElement(path);
    ASSERT_NE(nullptr, actual);
    ASSERT_EQ(name, actual->name());

    delete config;
}

INSTANTIATE_TEST_SUITE_P(ConfigurationElement, ConfigurationElement_testFindElement, ::testing::Values(
        std::make_tuple("root = {}", "$", "root"),
        std::make_tuple("root = { foo = true }", "$.foo", "foo"),
        std::make_tuple("root = { foo = { wibble=1.0 } }", "$.foo.wibble", "wibble"),
        std::make_tuple("root = { foo = { flibble={ spoo=3 } } }", "$.foo.flibble.spoo", "spoo"),
        std::make_tuple("root = { foo = true }", "foo", "foo"),
        std::make_tuple("root = { foo = { bar=\"wibble\" } }", "foo.bar", "bar"),
        std::make_tuple("root = { foo = { bar={ baz=2 } } }", "foo.bar.baz", "baz"),
        std::make_tuple("root = { \"wibble\" }", "$[0]", ""),
        std::make_tuple("root = { wibble={ true } }", "$.wibble[0]", ""),
        std::make_tuple("root = { wibble={ { foo=true } } }", "$.wibble[0].foo", "foo"),
        std::make_tuple("root = { wibble={ { foo={true} } } }", "$.wibble[0].foo[0]", ""),
        std::make_tuple("root = { wibble={ foo={ true } } }", "$.wibble.foo[0]", ""),
        std::make_tuple("root = { wibble={ foo={ { bar=1.0 } } } }", "$.wibble.foo[0].bar", "bar"),
        std::make_tuple("root = { wibble={ foo={ { bar=1.0 }, { baz=\"baz\" }, } } }", "$.wibble.foo[1].baz", "baz"),
        std::make_tuple("root = { wibble={ foo={ true } }, flibble={ tribble=1.0 } }", "$.flibble.tribble", "tribble"),
        std::make_tuple("root = { wibble={ foo={ true } }, flibble={ tribble=1.0 } }", "wibble.foo", "foo"),
        std::make_tuple("root = { wibble={ { foo=true } }, { tribble=1.0 } }", "wibble[0].foo", "foo"),
        std::make_tuple("root = { { foo=true }, { tribble=1.0 } }", "$[1].tribble", "tribble"),
        std::make_tuple("root = { { foo=true }, { tribble=1.0 } }", "[1].tribble", "tribble"),
        std::make_tuple("root = { wibble={ { foo=true }, { tribble=1.0 }, } }", "wibble[1].tribble", "tribble")
        ));

class ConfigurationElement_testAsInteger : public ::testing::TestWithParam<std::tuple<const char*, const char*, std::int64_t>>
{

};

TEST_P(ConfigurationElement_testAsInteger, testAsInteger)
{
    auto configStr = std::get<0>(GetParam());
    auto path = std::get<1>(GetParam());
    auto value = std::get<2>(GetParam());

    auto config = dagui::ConfigurationElement::fromString(configStr);
    ASSERT_NE(nullptr, config);
    auto element = config->findElement(path);
    ASSERT_NE(nullptr, element);
    EXPECT_EQ(value, element->asInteger());
    delete config;
}

INSTANTIATE_TEST_SUITE_P(ConfigurationElement, ConfigurationElement_testAsInteger, ::testing::Values(
        std::make_tuple("root={ foo=1 }", "foo", 1),
        std::make_tuple("root={ foo={ 1 } }", "foo[0]", 1)
        ));

class ConfigurationElement_testAsDouble : public ::testing::TestWithParam<std::tuple<const char*, const char*, double>>
{

};

TEST_P(ConfigurationElement_testAsDouble, testAsDouble)
{
    auto configStr = std::get<0>(GetParam());
    auto path = std::get<1>(GetParam());
    auto value = std::get<2>(GetParam());

    auto config = dagui::ConfigurationElement::fromString(configStr);
    ASSERT_NE(nullptr, config);
    auto element = config->findElement(path);
    ASSERT_NE(nullptr, element);
    EXPECT_EQ(value, element->asDouble());
    delete config;
}

INSTANTIATE_TEST_SUITE_P(ConfigurationElement, ConfigurationElement_testAsDouble, ::testing::Values(
        std::make_tuple("root={ foo=1.5 }", "foo", 1.5),
        std::make_tuple("root={ foo= { 1.5 } }", "foo[0]", 1.5)
        ));

class ConfigurationElement_testAsBool : public ::testing::TestWithParam<std::tuple<const char*, const char*, bool>>
{

};

TEST_P(ConfigurationElement_testAsBool, testAsBool)
{
    auto configStr = std::get<0>(GetParam());
    auto path = std::get<1>(GetParam());
    auto value = std::get<2>(GetParam());

    auto config = dagui::ConfigurationElement::fromString(configStr);
    ASSERT_NE(nullptr, config);
    auto element = config->findElement(path);
    ASSERT_NE(nullptr, element);
    EXPECT_EQ(value, element->asBool());
    delete config;
}

INSTANTIATE_TEST_SUITE_P(ConfigurationElement, ConfigurationElement_testAsBool, ::testing::Values(
        std::make_tuple("root={ foo=true }", "foo", true),
        std::make_tuple("root={ foo= { true } }", "foo[0]", true)
        ));

class ConfigurationElement_testAsString : public ::testing::TestWithParam<std::tuple<const char*, const char*, const char*>>
{

};

TEST_P(ConfigurationElement_testAsString, testAsString)
{
    auto configStr = std::get<0>(GetParam());
    auto path = std::get<1>(GetParam());
    auto value = std::get<2>(GetParam());

    auto config = dagui::ConfigurationElement::fromString(configStr);
    ASSERT_NE(nullptr, config);
    auto element = config->findElement(path);
    ASSERT_NE(nullptr, element);
    EXPECT_EQ(value, element->asString());
    delete config;
}

INSTANTIATE_TEST_SUITE_P(ConfigurationElement, ConfigurationElement_testAsString, ::testing::Values(
        std::make_tuple("root={ foo=\"true\" }", "foo", "true"),
        std::make_tuple("root={ foo= { \"true\" } }", "foo[0]", "true")
        ));

//@ParameterizedTest
//public void testAddWord(String word, String substring, String matchString) {
//    CompletionTrie sut = new CompletionTrie();
//    sut.addWord(word);
//    assertEquals(1, sut.numWords());
//    ArrayList<String> matches = new ArrayList<>();
//    sut.search(substring, matches);
//    assertFalse(matches.isEmpty());
//    assertEquals(matchString, matches.get(0));
//}
//
class CompletionTrie_testAddWord : public ::testing::TestWithParam<std::tuple<const char*, const char*, const char*>>
{

};

TEST_P(CompletionTrie_testAddWord, testAddWord)
{
    auto word = std::get<0>(GetParam());
    auto substring = std::get<1>(GetParam());
    auto matchString = std::get<2>(GetParam());
    auto sut = new dagui::CompletionTrie();
    sut->addWord(word);
    ASSERT_EQ(std::size_t{1}, sut->numWords());
    std::vector<std::string> matches;
    sut->search(substring, matches);
    EXPECT_FALSE(matches.empty());
    EXPECT_EQ(matchString, matches[0]);
    delete sut;
}
//@CsvSource({"Graph,Gr,Graph","Graph|Add Node|Math|Trig,Add,Graph|Add Node|Math|Trig"})
INSTANTIATE_TEST_SUITE_P(CompletionTrie, CompletionTrie_testAddWord, ::testing::Values(
        std::make_tuple("Graph","Gr","Graph"),
        std::make_tuple("Graph|Add Node|Math|Trig","Add","Graph|Add Node|Math|Trig")
        ));

class CompletionSubstring_testAddWord : public ::testing::TestWithParam<std::tuple<const char*, const char*, const char*>>
{

};

TEST_P(CompletionSubstring_testAddWord, testAddWord)
{
    auto word = std::get<0>(GetParam());
    auto substring = std::get<1>(GetParam());
    auto matchString = std::get<2>(GetParam());
    auto sut = new dagui::CompletionSubstring();
    sut->addWord(word);
    ASSERT_EQ(std::size_t{1}, sut->numWords());
    std::vector<std::string> matches;
    sut->search(substring, matches);
    ASSERT_FALSE(matches.empty());
    EXPECT_EQ(matchString, matches[0]);
    delete sut;
}
//@CsvSource({"Graph,Gr,Graph","Graph|Add Node|Math|Trig,Add,Graph|Add Node|Math|Trig"})
INSTANTIATE_TEST_SUITE_P(CompletionSubstring, CompletionSubstring_testAddWord, ::testing::Values(
        std::make_tuple("Graph","Gr","Graph"),
        std::make_tuple("Graph|Add Node|Math|Trig","Add","Graph|Add Node|Math|Trig")
        ));

//@ParameterizedTest
//@CsvSource({
//// Match at middle of multiple words
//"a,6,cat",
//"a,6,cab",
//"a,6,cabin",
//"b,2,cab",
//"b,2,cabin",
//"cat,2,cat",
//"cat,2,catamaran",
//"at,4,cat",
//"at,4,catamaran",
//"ta,1,catamaran",
//// Match in middle
//"Add,2,Node|Add|Math|Trig",
//"Add,2,Node|Add|Math|RelOp",
//// Match at end
//"RelOp,1,Node|Add|Math|RelOp",
//})
//public void testSearchMultipleWords(String substring, int numMatches, String match) {
//    CompletionTrie sut = new CompletionTrie();
//
//    addMultipleWords(sut);
//    ArrayList<String> matches = new ArrayList<>();
//    sut.search(substring, matches);
//    assertEquals(numMatches, matches.size());
//    assertTrue(matches.contains(match));
//}
//
//@ParameterizedTest
//@CsvSource({
//"Vector"
//})
//public void testNoMatches(String substring) {
//    CompletionTrie sut = new CompletionTrie();
//    addNoMatches(sut);
//    ArrayList<String> matches = new ArrayList<>();
//    sut.search(substring, matches);
//    assertEquals(0, matches.size());
//}

dagui::CompositeShape* createCompositeShape()
{
    auto parent = new dagui::CompositeShape();

    auto rectangle = new dagui::Rectangle();
    rectangle->setCornerRadius(10.0);
    rectangle->setPos(50.0,50.0);
    rectangle->setSize(100.0,100.0);
    parent->addShape(rectangle);
    auto leftPort = new dagui::Circle();
    leftPort->setPos(50.0,100.0);
    leftPort->setRadius(3.0);
    parent->addShape(leftPort);
    auto rightPort = new dagui::Circle();
    rightPort->setPos(150.0, 100.0);
    rightPort->setRadius(3.0);
    parent->addShape(rightPort);

    return parent;
}

class CompositeShape_testIsInside : public ::testing::TestWithParam<std::tuple<double, double, bool>>
{

};

TEST_P(CompositeShape_testIsInside, testIsInside)
{
    auto x = std::get<0>(GetParam());
    auto y = std::get<1>(GetParam());
    auto inside = std::get<2>(GetParam());
    auto sut = createCompositeShape();

    EXPECT_EQ(inside, sut->isInside(x,y));
    delete sut;
}

INSTANTIATE_TEST_SUITE_P(CompositeShape, CompositeShape_testIsInside, ::testing::Values(
        std::make_tuple(0, 0, false),
        std::make_tuple(100,100,true),
        std::make_tuple(49.0,100,true),
        std::make_tuple(51,100,true)
        ));

class MockShape : public dagui::Shape
{
public:
    MockShape()
    {
       // Do nothing.
    }

    MOCK_METHOD(void, accept, (dagui::ShapeVisitor&), (override));
    MOCK_METHOD(bool, isInside, (double,double), (override));
};

class TestShapeVisitor : public dagui::ShapeVisitor
{
public:
    void visitRectangle(dagui::Rectangle& rectangle) override
    {
        // Do nothing.
    }
};
TEST(CompositeShape, testAccept)
{
    auto sut = new dagui::CompositeShape();
    auto mockShape = new MockShape();

    EXPECT_CALL(*mockShape, accept(::testing::_));
    sut->addShape(mockShape);
    TestShapeVisitor visitor;
    sut->accept(visitor);
    delete mockShape;
    delete sut;
}

TEST(Window, testAfterConstructionThenAllFeaturesAreAvailable)
{
    auto sut = std::make_unique<dagui::Window>(nullptr);

    EXPECT_TRUE(sut->areFeaturesAvailable(dagui::Window::DEFAULT_FEATURES));
}

class Window_testFeaturesRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::Window::Features>>
{

};

TEST_P(Window_testFeaturesRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto features = std::get<1>(GetParam());

    EXPECT_EQ(str, dagui::Window::featuresName(features));
    EXPECT_EQ(features, dagui::Window::parseFeatures(str));
}

INSTANTIATE_TEST_SUITE_P(Window, Window_testFeaturesRoundTrip, ::testing::Values(
        std::make_tuple("TITLE_BIT",dagui::Window::TITLE_BIT),
        std::make_tuple("MINIMISE_BIT",dagui::Window::MINIMISE_BIT),
        std::make_tuple("MAXIMISE_BIT",dagui::Window::MAXIMISE_BIT),
        std::make_tuple("CLOSE_BIT",dagui::Window::CLOSE_BIT),
        std::make_tuple("BORDER_BIT",dagui::Window::BORDER_BIT),
        std::make_tuple("RESIZEABLE_BIT", dagui::Window::RESIZEABLE_BIT),
        std::make_tuple("MOVEABLE_BIT",dagui::Window::MOVEABLE_BIT),
        std::make_tuple("TITLE_BIT MINIMISE_BIT MAXIMISE_BIT CLOSE_BIT BORDER_BIT RESIZEABLE_BIT MOVEABLE_BIT", static_cast<dagui::Window::Features>(dagui::Window::DEFAULT_FEATURES))
        ));

class Window_testStatusRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, dagui::Window::Status>>
{

};

TEST_P(Window_testStatusRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto status = std::get<1>(GetParam());

    EXPECT_EQ(str, dagui::Window::statusName(status));
    EXPECT_EQ(status, dagui::Window::parseStatus(str));
}

INSTANTIATE_TEST_SUITE_P(Window, Window_testStatusRoundTrip, ::testing::Values(
        std::make_tuple("VISIBLE_BIT", dagui::Window::VISIBLE_BIT),
        std::make_tuple("MINIMISED_BIT", dagui::Window::MINIMISED_BIT),
        std::make_tuple("MAXIMISED_BIT", dagui::Window::MAXIMISED_BIT)
        ));

class ValidatorInt_testRange : public ::testing::TestWithParam<std::tuple<std::int64_t, std::int64_t, const char*, dagui::ValidatorInt<std::int64_t>::Error>>
{

};

TEST_P(ValidatorInt_testRange, testRange)
{
    auto minValue = std::get<0>(GetParam());
    auto maxValue = std::get<1>(GetParam());
    auto str = std::get<2>(GetParam());
    auto error = std::get<3>(GetParam());

    auto sut = std::make_unique<dagui::ValidatorInt<std::int64_t>>(minValue, maxValue);

    dagui::ValidatorInt<std::int64_t>::Error actual =  sut->validate(str);

    EXPECT_EQ(error, actual);
}

INSTANTIATE_TEST_SUITE_P(ValidatorInt, ValidatorInt_testRange, ::testing::Values(
        std::make_tuple(0, 5, "0", dagui::ValidatorInt<std::int64_t>::ERR_OK),
        std::make_tuple(0, 5, "5", dagui::ValidatorInt<std::int64_t>::ERR_OK),
        std::make_tuple(0, 5, "2", dagui::ValidatorInt<std::int64_t>::ERR_OK),
        std::make_tuple(0, 5, "10", dagui::ValidatorInt<std::int64_t>::ERR_TOO_HIGH),
        std::make_tuple(0, 5, "-1", dagui::ValidatorInt<std::int64_t>::ERR_TOO_LOW)
        ));
