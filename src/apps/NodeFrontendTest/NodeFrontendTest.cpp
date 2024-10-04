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
#include "gfx/FontImageSource.h"
#include "gfx/BinImageDef.h"
#include "gfx/Image.h"
#include "gfx/TextureAtlas.h"
#include "gfx/PackingStrategy.h"
#include "core/SpaceTree.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>
#include <cstdint>

#include "gfx/ShelfPackingStrategy.h"

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

class ConfigurationElement_testFindElement : public ::testing::TestWithParam<std::tuple<const char*, const char*, const char*>>
{

};

TEST_P(ConfigurationElement_testFindElement, testFindFromRoot)
{
    auto configStr = std::get<0>(GetParam());
    auto path = std::get<1>(GetParam());
    auto name = std::get<2>(GetParam());
    auto config = nfe::ConfigurationElement::fromString(configStr);
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

    auto config = nfe::ConfigurationElement::fromString(configStr);
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

    auto config = nfe::ConfigurationElement::fromString(configStr);
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

    auto config = nfe::ConfigurationElement::fromString(configStr);
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

    auto config = nfe::ConfigurationElement::fromString(configStr);
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
    auto sut = new nfe::CompletionTrie();
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
    auto sut = new nfe::CompletionSubstring();
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

nfe::CompositeShape* createCompositeShape()
{
    auto parent = new nfe::CompositeShape();

    auto rectangle = new nfe::Rectangle();
    rectangle->setCornerRadius(10.0);
    rectangle->setPos(50.0,50.0);
    rectangle->setSize(100.0,100.0);
    parent->addShape(rectangle);
    auto leftPort = new nfe::Circle();
    leftPort->setPos(50.0,100.0);
    leftPort->setRadius(3.0);
    parent->addShape(leftPort);
    auto rightPort = new nfe::Circle();
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

class MockShape : public nfe::Shape
{
public:
    MockShape()
    {
       // Do nothing.
    }

    MOCK_METHOD(void, accept, (nfe::ShapeVisitor&), (override));
    MOCK_METHOD(bool, isInside, (double,double), (override));
};

class TestShapeVisitor : public nfe::ShapeVisitor
{
public:
    void visitRectangle(nfe::Rectangle& rectangle) override
    {
        // Do nothing.
    }
};
TEST(CompositeShape, testAccept)
{
    auto sut = new nfe::CompositeShape();
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
    auto sut = std::make_unique<nfe::Window>(nullptr);

    EXPECT_TRUE(sut->areFeaturesAvailable(nfe::Window::DEFAULT_FEATURES));
}

class Window_testFeaturesRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, nfe::Window::Features>>
{

};

TEST_P(Window_testFeaturesRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto features = std::get<1>(GetParam());

    EXPECT_EQ(str, nfe::Window::featuresName(features));
    EXPECT_EQ(features, nfe::Window::parseFeatures(str));
}

INSTANTIATE_TEST_SUITE_P(Window, Window_testFeaturesRoundTrip, ::testing::Values(
        std::make_tuple("TITLE_BIT",nfe::Window::TITLE_BIT),
        std::make_tuple("MINIMISE_BIT",nfe::Window::MINIMISE_BIT),
        std::make_tuple("MAXIMISE_BIT",nfe::Window::MAXIMISE_BIT),
        std::make_tuple("CLOSE_BIT",nfe::Window::CLOSE_BIT),
        std::make_tuple("BORDER_BIT",nfe::Window::BORDER_BIT),
        std::make_tuple("RESIZEABLE_BIT", nfe::Window::RESIZEABLE_BIT),
        std::make_tuple("MOVEABLE_BIT",nfe::Window::MOVEABLE_BIT),
        std::make_tuple("TITLE_BIT MINIMISE_BIT MAXIMISE_BIT CLOSE_BIT BORDER_BIT RESIZEABLE_BIT MOVEABLE_BIT", static_cast<nfe::Window::Features>(nfe::Window::DEFAULT_FEATURES))
        ));

class Window_testStatusRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, nfe::Window::Status>>
{

};

TEST_P(Window_testStatusRoundTrip, testRoundTrip)
{
    auto str = std::get<0>(GetParam());
    auto status = std::get<1>(GetParam());

    EXPECT_EQ(str, nfe::Window::statusName(status));
    EXPECT_EQ(status, nfe::Window::parseStatus(str));
}

INSTANTIATE_TEST_SUITE_P(Window, Window_testStatusRoundTrip, ::testing::Values(
        std::make_tuple("VISIBLE_BIT", nfe::Window::VISIBLE_BIT),
        std::make_tuple("MINIMISED_BIT", nfe::Window::MINIMISED_BIT),
        std::make_tuple("MAXIMISED_BIT", nfe::Window::MAXIMISED_BIT)
        ));

class ValidatorInt_testRange : public ::testing::TestWithParam<std::tuple<std::int64_t, std::int64_t, const char*, nfe::ValidatorInt<std::int64_t>::Error>>
{
};

TEST_P(ValidatorInt_testRange, testRange)
{

    auto minValue = std::get<0>(GetParam());
    auto maxValue = std::get<1>(GetParam());
    auto str = std::get<2>(GetParam());
    auto error = std::get<3>(GetParam());

    auto sut = std::make_unique<nfe::ValidatorInt<std::int64_t>>(minValue, maxValue);

    nfe::ValidatorInt<std::int64_t>::Error actual =  sut->validate(str);

    EXPECT_EQ(error, actual);
}

INSTANTIATE_TEST_SUITE_P(ValidatorInt, ValidatorInt_testRange, ::testing::Values(
        std::make_tuple(0, 5, "0", nfe::ValidatorInt<std::int64_t>::ERR_OK),
        std::make_tuple(0, 5, "5", nfe::ValidatorInt<std::int64_t>::ERR_OK),
        std::make_tuple(0, 5, "2", nfe::ValidatorInt<std::int64_t>::ERR_OK),
        std::make_tuple(0, 5, "10", nfe::ValidatorInt<std::int64_t>::ERR_TOO_HIGH),
        std::make_tuple(0, 5, "-1", nfe::ValidatorInt<std::int64_t>::ERR_TOO_LOW)
        ));

class MockImageSource : public nfe::ImageSource
{
public:
	MockImageSource()
	{
		ON_CALL(*this, item).WillByDefault([this]() {
			if (_imageIndex<_images.size())
			{
				return _images[_imageIndex];
			}
			else
			{
				return static_cast<nfe::ImageDef*>(nullptr);
			}
		});
		ON_CALL(*this, hasMore).WillByDefault([this]() {
			return _imageIndex<_images.size();
		});
		ON_CALL(*this, nextItem).WillByDefault([this]() {
			++_imageIndex;
		});
	}

	void configure(nfe::ConfigurationElement& config)
	{
		config.eachChild([this](nfe::ConfigurationElement& child) {
			std::size_t width{0}, height{0};
			
			if (auto widthConfig = child.findElement("width"); widthConfig!=nullptr)
			{
				width = std::size_t(widthConfig->asInteger());
			}
			
			if (auto heightConfig = child.findElement("height"); heightConfig!=nullptr)
			{
				height = std::size_t(heightConfig->asInteger());
			}
			
			auto image = new nfe::BinImageDef(width, height, 1);
			//image->set(0,0,255,255,255);
			_images.emplace_back(image);
			
			return true;
		});
	}
	
	MOCK_METHOD(bool, hasMore, (), (const,override));
	MOCK_METHOD(void, nextItem, (), (override));
	MOCK_METHOD(nfe::ImageDef*, item, (), (override));
private:
	using ImageDefArray = std::vector<nfe::ImageDef*>;
	ImageDefArray _images;
	std::size_t _imageIndex{0};
};

class ShelfPackingStrategy_testPack : public ::testing::TestWithParam<std::tuple<const char*, std::size_t, std::size_t, nfe::PackingStrategy::Error, std::size_t>>
{
};

TEST_P(ShelfPackingStrategy_testPack, testPack)
{
	auto configStr = std::get<0>(GetParam());
	auto width = std::get<1>(GetParam());
	auto height = std::get<2>(GetParam());
	auto error = std::get<3>(GetParam());
	auto numImagesAllocated = std::get<4>(GetParam());
	auto config = nfe::ConfigurationElement::fromString(configStr);
	
	ASSERT_NE(nullptr, config);
	auto sut = new nfe::ShelfPackingStrategy();
	auto source = new MockImageSource();
	sut->setBinRectangle(new nfe::BinImageDef(width, height, 3));
	source->configure(*config);
	sut->setInputSource(source);
	EXPECT_CALL(*source, hasMore()).Times(::testing::AtLeast(1));
	EXPECT_CALL(*source, item()).Times(::testing::AtLeast(1));
	EXPECT_CALL(*source, nextItem()).Times(::testing::AtLeast(0));
	sut->makeItSo();
	EXPECT_EQ(error, sut->error());
	EXPECT_EQ(numImagesAllocated, sut->numAllocations());
	delete source;
	delete sut;
}

INSTANTIATE_TEST_SUITE_P(ShelfPackingStrategy, ShelfPackingStrategy_testPack, ::testing::Values(
	std::make_tuple("root = { [1]={width=512,height=512}, [2]={width=1,height=1} }", 512u, 512u, nfe::PackingStrategy::ERR_FAILED_TO_PACK, 1u),
	std::make_tuple("root = { [1]={width=256,height=256}, [2]={width=16,height=16} }", 512u, 512u, nfe::PackingStrategy::ERR_OK, 2u)
));

class FontImageSource_testNextItem : public ::testing::TestWithParam<std::tuple<const char*>>
{
};

TEST_P(FontImageSource_testNextItem, testNextItem)
{
	auto fontFilename = std::get<0>(GetParam());
	FT_Library library;
	int error = FT_Init_FreeType( &library );
    if ( error )
    {
        FAIL();
	}

	nfe::FontImageSource sut(library, fontFilename);
	ASSERT_TRUE(sut.ok());
	EXPECT_TRUE(sut.hasMore());
	bool found = false;
	ASSERT_TRUE(sut.ok());
	while (sut.hasMore() && !found)
	{
		nfe::ImageDef* item = sut.item();
		ASSERT_NE(nullptr, item);
		nfe::Image* image = item->createImage();
		found = image->find(255,255,255);
		delete image;
		delete item;
		sut.nextItem();
	}
	//EXPECT_TRUE(found);
}

INSTANTIATE_TEST_SUITE_P(FontImageSource, FontImageSource_testNextItem, ::testing::Values(
	std::make_tuple("data/fonts/LiberationSans-Regular.ttf")
));

class FontImageSource_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, nfe::FontImageSource::Error>>
{
};

TEST_P(FontImageSource_testRoundTrip, testRoundTrip)
{
	auto str = std::get<0>(GetParam());
	auto err = std::get<1>(GetParam());
	
	EXPECT_STREQ(str, nfe::FontImageSource::errorToString(err));
	EXPECT_EQ(err, nfe::FontImageSource::parseError(str));
}

INSTANTIATE_TEST_SUITE_P(FontImageSource, FontImageSource_testRoundTrip, ::testing::Values(
	std::make_tuple("ERR_OK", nfe::FontImageSource::ERR_OK),
	std::make_tuple("ERR_UNSUPPORTED_FORMAT", nfe::FontImageSource::ERR_UNSUPPORTED_FORMAT),
	std::make_tuple("ERR_FAILED_TO_OPEN_FONT", nfe::FontImageSource::ERR_FAILED_TO_OPEN_FONT),
	std::make_tuple("ERR_LOADING_GLYPH", nfe::FontImageSource::ERR_LOADING_GLYPH),
	std::make_tuple("ERR_FAILED_TO_RENDER_GLYPH", nfe::FontImageSource::ERR_FAILED_TO_RENDER_GLYPH)
));

class TextureAtlas_testDimensions : public ::testing::TestWithParam<std::tuple<std::size_t, std::size_t, nfe::TextureAtlas::Error>>
{
};

TEST_P(TextureAtlas_testDimensions, testDimensions)
{
	auto width = std::get<0>(GetParam());
	auto height = std::get<1>(GetParam());
	auto err = std::get<2>(GetParam());
	
	nfe::TextureAtlas sut(width,height,3);
	EXPECT_EQ(err, sut.error());
}

INSTANTIATE_TEST_SUITE_P(TextureAtlas, TextureAtlas_testDimensions, ::testing::Values(
	std::make_tuple(512, 512, nfe::TextureAtlas::ERR_OK),
	std::make_tuple(511, 512, nfe::TextureAtlas::ERR_NON_POWER_OF_TWO_DIMS),
	std::make_tuple(512, 511, nfe::TextureAtlas::ERR_NON_POWER_OF_TWO_DIMS)
));

class PackingStrategy_testErrorRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, nfe::PackingStrategy::Error>>
{
};

TEST_P(PackingStrategy_testErrorRoundTrip, testRoundTrip)
{
	auto str = std::get<0>(GetParam());
	auto err = std::get<1>(GetParam());
	
	EXPECT_STREQ(str, nfe::PackingStrategy::errorToString(err));
	EXPECT_EQ(err, nfe::PackingStrategy::parseError(str));
}

INSTANTIATE_TEST_SUITE_P(PackingStrategy, PackingStrategy_testErrorRoundTrip, ::testing::Values(
	std::make_tuple("ERR_UNKNOWN", nfe::PackingStrategy::ERR_UNKNOWN),
	std::make_tuple("ERR_OK", nfe::PackingStrategy::ERR_OK),
	std::make_tuple("ERR_FAILED_TO_PACK", nfe::PackingStrategy::ERR_FAILED_TO_PACK)
));

class SpaceTree_testInsert : public ::testing::TestWithParam<std::tuple<const char*, std::size_t, std::size_t, std::size_t, nfe::SpaceTree::Result>>
{

};

TEST_P(SpaceTree_testInsert, testInsert)
{
	auto configStr = std::get<0>(GetParam());
	auto width = std::get<1>(GetParam());
	auto height = std::get<2>(GetParam());
	auto numNodes = std::get<3>(GetParam());
	auto result = std::get<4>(GetParam());
	auto config = nfe::ConfigurationElement::fromString(configStr);
	ASSERT_NE(nullptr, config);
	auto sut = nfe::SpaceTree::fromConfig(*config);
	ASSERT_NE(nullptr, sut);
	std::size_t actualNumNodes {0};
	sut->traversal([&actualNumNodes](nfe::SpaceTree* node)
	{
		actualNumNodes++;
	});
	ASSERT_EQ(numNodes, actualNumNodes);
	nfe::SpaceTree::Result actualResult = sut->insert(width,height);
	EXPECT_EQ(result, actualResult);
}

INSTANTIATE_TEST_SUITE_P(SpaceTree, SpaceTree_testInsert, ::testing::Values(
	std::make_tuple("root = { nodeType=\"TYPE_FREE\", width=512, height=512 }", 256, 256, 1u, nfe::SpaceTree::RESULT_OK),
	std::make_tuple("root = { nodeType=\"TYPE_FREE\", width=512, height=512, children = { { nodeType=\"TYPE_FREE\", width=256, height=256 } } }", 1024, 256, 2u, nfe::SpaceTree::RESULT_FAILED_TO_INSERT),
	std::make_tuple("root = { nodeType=\"TYPE_FREE\", width=512, height=512, children = { { nodeType=\"TYPE_INTERNAL\", width=256, height=512, children={ { width=256, height=256, nodeType=\"TYPE_FREE\"}, { width=256, height=256, nodeType=\"TYPE_FULL\" } } }, { nodeType=\"TYPE_FREE\", width=256, height=512 } } }", 1024, 256, 5u, nfe::SpaceTree::RESULT_FAILED_TO_INSERT)
	));

class SpaceTreeType_testRoundTrip : public ::testing::TestWithParam<std::tuple<const char*, nfe::SpaceTree::Type>>
{

};

TEST_P(SpaceTreeType_testRoundTrip, testRoundTrip)
{
	auto str = std::get<0>(GetParam());
	auto type = std::get<1>(GetParam());

	EXPECT_STREQ(str, nfe::SpaceTree::typeToString(type));
	EXPECT_EQ(type, nfe::SpaceTree::parseType(str));
}

INSTANTIATE_TEST_SUITE_P(SpaceTree, SpaceTreeType_testRoundTrip, ::testing::Values(
	std::make_tuple("TYPE_UNKNOWN", nfe::SpaceTree::TYPE_UNKNOWN),
	std::make_tuple("TYPE_INTERNAL", nfe::SpaceTree::TYPE_INTERNAL),
	std::make_tuple("TYPE_FREE", nfe::SpaceTree::TYPE_FREE),
	std::make_tuple("TYPE_FULL", nfe::SpaceTree::TYPE_FULL)
	));