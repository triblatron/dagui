//
// Created by Tony Horrobin on 25/01/2025.
//

#include "gfx/OpenGLMesh2D.h"
#include "gfx/Image.h"
#include "gfx/ImageDef.h"
#include "gfx/OpenGLRenderer.h"
#include "gfx/BinImageDef.h"
#include "core/LuaInterface.h"
#include "core/ConfigurationElement.h"
#include "core/BinPackingStrategyFactory.h"
#include "core/BinPackingStrategy.h"
#include "gfx/FontImageSource.h"
#include "gfx/TextureAtlas.h"
#include "gfx/GenericAttributeArray.h"
#include "gfx/GenericMesh.h"
#include "gfx/Layer.h"
#include "gfx/OpenGL.h"
#include "gfx/RendererFactory.h"
#include "core/WidgetFactory.h"
#include "core/Shape.h"
#include "core/DrawCommandBuffer.h"
#include "test/TestUtils.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


class MockAtlas : public dagui::Atlas
{
public:
	MockAtlas()
	{
		ON_CALL(*this, width).WillByDefault([]() {
			return std::size_t{ 512 };
			});
		ON_CALL(*this, height).WillByDefault([]() {
			return std::size_t{ 512 };
			});
		ON_CALL(*this, imageForGlyphIndex).WillByDefault([](unsigned long) -> dagui::ImageDef*
		{
			return nullptr;
		});
	}

	MOCK_METHOD(std::size_t, width, (), (const, override));
	MOCK_METHOD(std::size_t, height, (), (const, override));
	MOCK_METHOD(void, allocateImage, (unsigned long, dagui::ImageDef*), (override));
	MOCK_METHOD(dagui::ImageDef*, imageForGlyphIndex, (unsigned long), (override));
};

class MockImageSource : public dagui::ImageSource
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
				return static_cast<dagui::ImageDef*>(nullptr);
			}
		});
		ON_CALL(*this, hasMore).WillByDefault([this]() {
			return _imageIndex<_images.size();
		});
		ON_CALL(*this, nextItem).WillByDefault([this]() {
			++_imageIndex;
		});
        ON_CALL(*this, estimateCount).WillByDefault([this]() {
           return _images.size();
        });
	}

	void configure(dagbase::ConfigurationElement& config)
	{
		config.eachChild([this](dagbase::ConfigurationElement& child) {
			std::size_t width{0}, height{0};

			if (auto widthConfig = child.findElement("width"); widthConfig!=nullptr)
			{
				width = std::size_t(widthConfig->asInteger());
			}

			if (auto heightConfig = child.findElement("height"); heightConfig!=nullptr)
			{
				height = std::size_t(heightConfig->asInteger());
			}

			auto image = new dagui::BinImageDef(width, height, 1);
			//image->set(0,0,255,255,255);
			_images.emplace_back(image);

			return true;
		});
	}

    MOCK_METHOD(std::size_t, estimateCount, (), (const,override));
	MOCK_METHOD(bool, hasMore, (), (const,override));
	MOCK_METHOD(void, nextItem, (), (override));
	MOCK_METHOD(dagui::ImageDef*, item, (), (override));
private:
	using ImageDefArray = std::vector<dagui::ImageDef*>;
	ImageDefArray _images;
	std::size_t _imageIndex{0};
};

class FontImageSource_testNextItem : public ::testing::TestWithParam<std::tuple<const char*, int>>
{
};

TEST_P(FontImageSource_testNextItem, testNextItem)
{
	auto configStr = std::get<0>(GetParam());
	auto count = std::get<1>(GetParam());
	dagbase::Lua lua;
	auto config = dagbase::ConfigurationElement::fromString(lua,configStr);
	ASSERT_NE(nullptr, config);
	FT_Library library;
	int error = FT_Init_FreeType( &library );
    if ( error )
    {
        FAIL();
	}

	dagui::FontImageSource sut(library);
	sut.configure(*config);

	ASSERT_TRUE(sut.ok());
	EXPECT_TRUE(sut.hasMore());
	bool found = false;
	ASSERT_TRUE(sut.ok());
	int actualCount = 0;
	while (sut.hasMore() && !found)
	{
		dagui::ImageDef* item = sut.item();
		ASSERT_NE(nullptr, item);
		delete item;
		sut.nextItem();
		++actualCount;
	}
	EXPECT_EQ(count, actualCount);
	FT_Done_FreeType( library );
}

INSTANTIATE_TEST_SUITE_P(FontImageSource, FontImageSource_testNextItem, ::testing::Values(
	std::make_tuple("root = { fontFilename=\"data/liberation-fonts-ttf-2.1.5/LiberationSans-Regular.ttf\", ranges={ { first=32, last=32 }, { first=65, last=65+25 } } }", 27)
));

class BinPackingStrategy_testPack : public ::testing::TestWithParam<std::tuple<const char*, const char*, int, dagui::BinPackingStrategy::Result>>
{
public:

    void SetUp()
    {
        _configStr = std::get<1>(GetParam());
        _config = dagbase::ConfigurationElement::fromString(_lua, _configStr);
        _imageSource = new MockImageSource();
        _imageSource->configure(*_config);
        _atlas = new MockAtlas();
    }

    void TearDown()
    {
        delete _atlas;
        delete _imageSource;
    }
protected:
    dagbase::Lua _lua;
    const char* _configStr{ nullptr };
    dagbase::ConfigurationElement* _config{ nullptr };
    MockImageSource* _imageSource{ nullptr };
    MockAtlas* _atlas{ nullptr };
};

TEST_P(BinPackingStrategy_testPack, testPack)
{
    auto className = std::get<0>(GetParam());
    auto numAllocations = std::get<2>(GetParam());
    auto result = std::get<3>(GetParam());
    dagui::BinPackingStrategyFactory factory;
    auto strategy = factory.createStrategy(className);
    ASSERT_NE(nullptr, strategy);
    EXPECT_CALL(*_imageSource, hasMore()).Times(::testing::AtLeast(1));
    EXPECT_CALL(*_imageSource, item).Times(::testing::AtLeast(1));
    EXPECT_CALL(*_imageSource, nextItem).Times(::testing::AnyNumber());
    EXPECT_CALL(*_atlas, width).Times(::testing::AnyNumber());
    EXPECT_CALL(*_atlas, height).Times(::testing::AnyNumber());
    EXPECT_CALL(*_atlas, allocateImage).Times(numAllocations);
    strategy->pack(*_imageSource, *_atlas);
    EXPECT_EQ(result, strategy->result());
}

INSTANTIATE_TEST_SUITE_P(BinPackingStrategy, BinPackingStrategy_testPack, ::testing::Values(
    std::make_tuple("Shelf", "root={ { width=256, height=256 } }", std::size_t{ 1 }, dagui::BinPackingStrategy::RESULT_OK),
    std::make_tuple("Shelf", "root={ { width=1024, height=1024 } }", std::size_t{ 0 }, dagui::BinPackingStrategy::RESULT_FAILED_TO_FIND_SPACE),
    std::make_tuple("MaxRects", "root={ { width=256, height=256 } }", std::size_t{ 1 }, dagui::BinPackingStrategy::RESULT_OK),
    std::make_tuple("MaxRects", "root={ { width=1024, height=1024 } }", std::size_t{ 0 }, dagui::BinPackingStrategy::RESULT_FAILED_TO_FIND_SPACE)
));

class TextureAtlas_testDimensions : public ::testing::TestWithParam<std::tuple<std::size_t, std::size_t, dagui::TextureAtlas::Error>>
{
};

TEST_P(TextureAtlas_testDimensions, testDimensions)
{
	auto width = std::get<0>(GetParam());
	auto height = std::get<1>(GetParam());
	auto err = std::get<2>(GetParam());

	dagui::TextureAtlas sut(width,height,3);
	EXPECT_EQ(err, sut.error());
}

INSTANTIATE_TEST_SUITE_P(TextureAtlas, TextureAtlas_testDimensions, ::testing::Values(
	std::make_tuple(512, 512, dagui::TextureAtlas::ERR_OK),
	std::make_tuple(511, 512, dagui::TextureAtlas::ERR_NON_POWER_OF_TWO_DIMS),
	std::make_tuple(512, 511, dagui::TextureAtlas::ERR_NON_POWER_OF_TWO_DIMS)
));

class MockImageDef : public dagui::ImageDef
{
public:
	MockImageDef()
	{

	}
	MOCK_METHOD((dagui::Image*), createImage, (), (const, override));
};

class Renderer_testGenerateTextureCoordinates : public ::testing::TestWithParam<std::tuple<std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, dagui::Vec2f, dagui::Vec2f, dagui::Vec2f, dagui::Vec2f>>
{

};

TEST_P(Renderer_testGenerateTextureCoordinates, testGenerateTextureCoordinates)
{
	auto binWidth = std::get<0>(GetParam());
	auto binHeight = std::get<1>(GetParam());
	auto x = std::get<2>(GetParam());
	auto y = std::get<3>(GetParam());
	auto width = std::get<4>(GetParam());
	auto height = std::get<5>(GetParam());
	auto p0 = std::get<6>(GetParam());
	auto p1 = std::get<7>(GetParam());
	auto p2 = std::get<8>(GetParam());
	auto p3 = std::get<9>(GetParam());

	dagui::OpenGLRenderer sut;
	MockImageDef def;
	def.x = x;
	def.y = y;
	def.width = width;
	def.height = height;
	dagui::BinImageDef binImageDef(binWidth, binHeight, 1);

	sut.generateTextureCoordinates(def, binImageDef);
	EXPECT_EQ(p0, def.p0);
	EXPECT_EQ(p1, def.p1);
	EXPECT_EQ(p2, def.p2);
	EXPECT_EQ(p3, def.p3);
}

INSTANTIATE_TEST_SUITE_P(Renderer, Renderer_testGenerateTextureCoordinates, ::testing::Values(
	std::make_tuple(256, 256, 0, 0, 256, 256, dagui::Vec2f(0.0f, 0.0f), dagui::Vec2f(1.0f, 0.0f), dagui::Vec2f(1.0f,1.0f), dagui::Vec2f(0.0f,1.0f))
	));

class TextureAtlas_testPack : public ::testing::TestWithParam<std::tuple<const char*, const char*, std::uint32_t, bool>>
{

};

TEST_P(TextureAtlas_testPack, testPack)
{
	auto configFilename = std::get<0>(GetParam());
	dagbase::Lua lua;
	auto config = dagbase::ConfigurationElement::fromFile(lua, configFilename);
	ASSERT_NE(nullptr, config);
	auto atlasConfig = config->findElement("atlas");
	ASSERT_NE(nullptr, atlasConfig);
	dagui::TextureAtlas sut;
	sut.configure(*atlasConfig);
	dagui::BinPackingStrategyFactory factory;
	auto strategyClass = std::get<1>(GetParam());
	auto strategy = factory.createStrategy(strategyClass);
	ASSERT_NE(nullptr, strategy);
	FT_Library library;
	int error = FT_Init_FreeType( &library );
	if ( error )
	{
		FAIL();
	}
	auto sourceConfig = config->findElement("source");
	dagui::FontImageSource source(library);
	source.configure(*sourceConfig);
	sut.setImageSource(&source);
	sut.pack(*strategy);
	ASSERT_TRUE(sut.ok());

	EXPECT_TRUE(sut.binImage()->find(255,255,255));
	EXPECT_GT(sut.binImage()->count(255,255,255),100);
	auto glyphIndex = std::get<2>(GetParam());
	auto notNull = std::get<3>(GetParam());
	auto actualImage = sut.imageForGlyphIndex(glyphIndex);
	EXPECT_EQ(notNull, actualImage!=nullptr);
	FT_Done_FreeType(library);
}

INSTANTIATE_TEST_SUITE_P(TextureAtlas, TextureAtlas_testPack, ::testing::Values(
	std::make_tuple("data/tests/TextureAtlas/testPack.lua", "MaxRects", 36, true)
	));

class FontImageSource_testEstimateCount : public ::testing::TestWithParam<std::tuple<const char*, std::size_t>>
{

};

TEST_P(FontImageSource_testEstimateCount, testEstimateCount)
{
	auto configFilename = std::get<0>(GetParam());
	auto count = std::get<1>(GetParam());

	FT_Library library;
	int error = FT_Init_FreeType( &library );
	if ( error )
		FAIL();
	dagui::FontImageSource source(library);
	dagbase::Lua lua;
	auto config = dagbase::ConfigurationElement::fromFile(lua, configFilename);
	ASSERT_NE(nullptr, config);
	dagui::FontImageSource sut(library);
	sut.configure(*config);
	EXPECT_EQ(count, sut.estimateCount());
	FT_Done_FreeType( library );
}

INSTANTIATE_TEST_SUITE_P(FontImageSource, FontImageSource_testEstimateCount, ::testing::Values(
	std::make_tuple("data/tests/FontImageSource/testEstimateCount.lua", 95)
	));

class Image_testCopyFrom : public ::testing::TestWithParam<std::tuple<std::uint32_t, std::uint32_t, std::uint32_t, dagui::Image::Origin, std::uint8_t, std::uint8_t, std::uint8_t>>
{

};

TEST_P(Image_testCopyFrom, testCopyFrom)
{
	auto width = std::get<0>(GetParam());
	auto height = std::get<1>(GetParam());
	auto numComponents = std::get<2>(GetParam());
	auto origin = std::get<3>(GetParam());
	auto red = std::get<4>(GetParam());
	auto green = std::get<5>(GetParam());
	auto blue = std::get<6>(GetParam());
	dagui::Image* sut = new dagui::Image(width, height, numComponents, origin);
	dagui::Image* source = new dagui::Image(width, height, 1, dagui::Image::ORIGIN_TOP_LEFT);
	source->set(0, 0, 255,	255, 255);
	sut->copyFrom(0, 0, source);
	std::uint8_t actualRed{0}, actualGreen{0}, actualBlue{0};
	sut->get(0, 0, &actualRed, &actualGreen, &actualBlue);
	EXPECT_EQ(red, actualRed);
	EXPECT_EQ(green, actualGreen);
	EXPECT_EQ(blue, actualBlue);
	delete source;
	delete sut;
}

INSTANTIATE_TEST_SUITE_P(Image, Image_testCopyFrom, ::testing::Values(
	std::make_tuple(1, 2, 3, dagui::Image::ORIGIN_BOTTOM_LEFT, 0, 0, 0),
	std::make_tuple(1, 2, 3, dagui::Image::ORIGIN_TOP_LEFT, 255, 255, 255)
	));

class RendererFactory_testCreateRenderer : public ::testing::TestWithParam<std::tuple<const char*, int, int, int>>
{

};

TEST_P(RendererFactory_testCreateRenderer, testCreateRenderer)
{
	auto apiName = std::get<0>(GetParam());
	auto majorVersion = std::get<1>(GetParam());
	auto minorVersion = std::get<2>(GetParam());
	auto patchVersion = std::get<3>(GetParam());
	dagui::APIVersion version;
	version.major = majorVersion;
	version.minor = minorVersion;
	version.patch = patchVersion;
	dagui::RendererFactory sut;
	auto actual = sut.createRenderer(apiName, version);
	ASSERT_NE(nullptr, actual);
	EXPECT_EQ(version.major, actual->apiVersion().major);
	EXPECT_GE(actual->apiVersion().minor, version.minor);
	EXPECT_GE(actual->apiVersion().patch, version.patch);
}

INSTANTIATE_TEST_SUITE_P(RendererFactory, RendererFactory_testCreateRenderer, ::testing::Values(
	std::make_tuple("OpenGL", 1, 0, 0)
	));

class MockOpenGL : public dagui::gl::OpenGL
{
public:
	MockOpenGL()
	{
		ON_CALL(*this, drawArray).WillByDefault([this](const dagui::gl::VertexBuffer& vertexBuffer)->void
		{
			EXPECT_EQ(_numVertices*_numComponentsPerVertex, vertexBuffer.numElements());
		});
	}

	void setNumVertices(unsigned int numVertices)
	{
		_numVertices = numVertices;
	}

	void setNumComponentsPerVertex(unsigned int numComponentsPerVertex)
	{
		_numComponentsPerVertex = numComponentsPerVertex;
	}
	MOCK_METHOD(void, drawArray, (const dagui::gl::VertexBuffer&), (override));
	MOCK_METHOD(void, drawElements, (const dagui::gl::VertexBuffer&, const dagui::gl::IndexBuffer&), (override));
private:
	unsigned int _numVertices{0};
	unsigned int _numComponentsPerVertex{0};
};

class OpenGL_testDrawElements : public ::testing::TestWithParam<std::tuple<const char*, unsigned int, unsigned int>>
{

};

TEST_P(OpenGL_testDrawElements, testDrawElements)
{
	auto configStr = std::get<0>(GetParam());
	auto numVertices = std::get<1>(GetParam());
	auto numComponentsPerVertex = std::get<2>(GetParam());
	dagbase::Lua lua;
	auto config = dagbase::ConfigurationElement::fromString(lua, configStr);
	ASSERT_NE(nullptr, config);
	dagui::OpenGLRenderer renderer;
	dagui::gl::OpenGLMesh2D mesh;
	mesh.configure(*config);
	MockOpenGL sut;
	sut.setNumVertices(numVertices);
	sut.setNumComponentsPerVertex(numComponentsPerVertex);
	renderer.setOpenGL(&sut);
	EXPECT_CALL(sut, drawArray(::testing::_));
	renderer.drawMesh2D(mesh);
}

INSTANTIATE_TEST_SUITE_P(OpenGL, OpenGL_testDrawElements, ::testing::Values(
	std::make_tuple("root = { primitiveType=\"PRIMITIVE_TRIANGLE\", vertices={ { 0, 0 }, { 1, 0 }, { 1, 1 } } }", 3, 2)
	));

class DrawingCommand_testMakeItSo : public ::testing::TestWithParam<std::tuple<const char*, const char*, const char*, dagbase::ConfigurationElement::ValueType, double, dagbase::ConfigurationElement::RelOp>>
{

};

TEST_P(DrawingCommand_testMakeItSo, testMakeItSo)
{
	auto meshConfigStr = std::get<0>(GetParam());
	dagbase::Lua lua;
	auto meshConfig = dagbase::ConfigurationElement::fromString(lua, meshConfigStr);
	ASSERT_NE(nullptr, meshConfig);
	dagui::Mesh2D mesh;
	mesh.configure(*meshConfig);
	auto rendererConfigStr = std::get<1>(GetParam());
	auto rendererConfig = dagbase::ConfigurationElement::fromString(lua, rendererConfigStr);
	ASSERT_NE(nullptr, rendererConfig);
	dagui::OpenGLRenderer sut;
	sut.configure(*rendererConfig);
	auto path = std::get<2>(GetParam());
	auto value = std::get<3>(GetParam());
	auto tolerance = std::get<4>(GetParam());
	auto op = std::get<5>(GetParam());
	auto actualValue = sut.find(path);
	ASSERT_TRUE(actualValue.has_value());
	assertComparison(value, actualValue, tolerance, op);
}

INSTANTIATE_TEST_SUITE_P(DrawingCommand, DrawingCommand_testMakeItSo, ::testing::Values(
	std::make_tuple("root = { primitiveType=\"PRIMITIVE_TRIANGLE\", vertices={ { 0, 0 }, { 1, 0 }, { 1, 1 } } }", "root = { commandClass=\"DrawArrays\" }", "numCommands", dagbase::ConfigurationElement::ValueType(std::int64_t(1)), 0.0, dagbase::ConfigurationElement::RELOP_EQ)
	));

struct TestVertex
{
	float x{0.0f};
	float y{0.0f};
	float r{0.0f};
	float g{0.0f};
	float b{0.0f};
	float a{0.0f};

	void configure(dagbase::ConfigurationElement& config)
	{
		if (auto element = config.findElement("x"); element)
			x = static_cast<float>(element->asDouble());

		if (auto element = config.findElement("y"); element)
			y = static_cast<float>(element->asDouble());

		if (auto element = config.findElement("r"); element)
			r = static_cast<float>(element->asDouble());

		if (auto element = config.findElement("g"); element)
			g = static_cast<float>(element->asDouble());

		if (auto element = config.findElement("b"); element)
			b = static_cast<float>(element->asDouble());

		if (auto element = config.findElement("a"); element)
			a = static_cast<float>(element->asDouble());

	}

	bool operator==(const TestVertex& other) const
	{
		return x==other.x && y==other.y && r==other.r && g==other.g && b==other.b && a==other.a;
	}
};

typedef float TestVertex::*TestVertexField;

class GenericAttributeArray_testAddVertex : public ::testing::TestWithParam<std::tuple<const char*, std::size_t, TestVertexField, float>>
{

};

TEST_P(GenericAttributeArray_testAddVertex, testDataIsPresentAfterAdd)
{
	auto configStr = std::get<0>(GetParam());
	dagbase::Lua lua;
	auto config = dagbase::ConfigurationElement::fromFile(lua, configStr);
	ASSERT_NE(nullptr, config);
	auto index = std::get<1>(GetParam());
	auto func = std::get<2>(GetParam());
	auto value = std::get<3>(GetParam());
	TestVertex v;
	if (auto vertexConfig = config->findElement("vertex"); vertexConfig)
		v.configure(*vertexConfig);

	dagui::GenericAttributeArray<TestVertex> sut;
	dagui::ArrayDescriptor arrayDescriptor;
	if (auto arrayConfig = config->findElement("array"); arrayConfig)
		arrayDescriptor.configure(*arrayConfig);

	sut.setDescriptor(arrayDescriptor);
	sut.addVertex(v);
	v = sut.vertex(index);
	float actualValue = v.*func;
	EXPECT_EQ(value, actualValue);
}

INSTANTIATE_TEST_SUITE_P(GenericAttributeArray, GenericAttributeArray_testAddVertex, ::testing::Values(
	std::make_tuple("data/tests/GenericAttributeArray/testAddVertex.lua", 0, &TestVertex::x, 1.0f),
	std::make_tuple("data/tests/GenericAttributeArray/testAddVertex.lua", 0, &TestVertex::y, 0.4f),
	std::make_tuple("data/tests/GenericAttributeArray/testAddVertex.lua", 0, &TestVertex::r, 1.0f),
	std::make_tuple("data/tests/GenericAttributeArray/testAddVertex.lua", 0, &TestVertex::g, 0.5f),
	std::make_tuple("data/tests/GenericAttributeArray/testAddVertex.lua", 0, &TestVertex::b, 0.2f),
	std::make_tuple("data/tests/GenericAttributeArray/testAddVertex.lua", 0, &TestVertex::a, 1.0f)
	));

class AttributeLayout_testStride : public ::testing::TestWithParam<std::tuple<const char*, const char*, std::size_t, std::uint32_t>>
{

};

TEST_P(AttributeLayout_testStride, testValue)
{
	auto configStr = std::get<0>(GetParam());
	dagbase::Lua lua;
	auto config = dagbase::ConfigurationElement::fromFile(lua, configStr);
	ASSERT_NE(nullptr, config);
	auto layoutName = std::get<1>(GetParam());
	auto attrIndex = std::get<2>(GetParam());
	size_t stride = std::get<3>(GetParam());
	dagui::ArrayDescriptor sut;
	auto layoutConfig = config->findElement(layoutName);
	ASSERT_NE(nullptr, layoutConfig);
	sut.configure(*layoutConfig);
	auto actualStride = sut.attributes[attrIndex].stride;
	EXPECT_EQ(stride, actualStride);
}

INSTANTIATE_TEST_SUITE_P(AttributeLayout, AttributeLayout_testStride, ::testing::Values(
	std::make_tuple("data/tests/AttributeLayout/testStride.lua", "packed", 0, 2*sizeof(float)),
	std::make_tuple("data/tests/AttributeLayout/testStride.lua", "interleaved", 0, 6*sizeof(float)),
	std::make_tuple("data/tests/AttributeLayout/testStride.lua", "interleaved", 1, 6*sizeof(float))
	));

// class MockAttributeArray : public dagui::AttributeArray
// {
// public:
// 	~MockAttributeArray()
// 	{
// 		destroy();
// 	}
// 	MOCK_METHOD(void, destroy, (), ());
// 	MOCK_METHOD(const void*, data, (), (const, override));
// 	MOCK_METHOD(std::size_t, size, (), (const, override));
// 	MOCK_METHOD(std::size_t, elementSize, (), (const, override));
// };
//
// class GenericMesh_testTakesOwnershipOfArrays : public ::testing::TestWithParam<std::tuple<const char*>>
// {
//
// };
//
// TEST_P(GenericMesh_testTakesOwnershipOfArrays, testDestructorCalledOnDelete)
// {
// 	auto configStr = std::get<0>(GetParam());
// 	dagbase::Lua lua;
// 	auto config = dagbase::ConfigurationElement::fromFile(lua, configStr);
// 	ASSERT_NE(nullptr, config);
// 	dagui::GenericMesh<TestVertex> sut;
// 	auto array = new MockAttributeArray();
// 	dagui::ArrayDescriptor arrayConfig;
// 	arrayConfig.configure(*config);
// 	array->setDescriptor(arrayConfig);
// 	sut.addData(0, array);
// 	EXPECT_CALL(*array, destroy).Times(1);
// }
//
// INSTANTIATE_TEST_SUITE_P(GenericMesh, GenericMesh_testTakesOwnershipOfArrays, ::testing::Values(
// 	std::make_tuple("data/tests/GenericMesh/testTakesOwnershipOfArrays.lua")
// 	));

class OpenGLDataType_testFromAttribute : public ::testing::TestWithParam<std::tuple<dagui::AttributeDescriptor::DataType, GLenum>>
{
};

TEST_P(OpenGLDataType_testFromAttribute, testValue)
{
	auto attrDataType = std::get<0>(GetParam());
	auto glDataType = std::get<1>(GetParam());

	EXPECT_EQ(glDataType, dagui::gl::Buffer::dataTypeToGL(attrDataType));
}

INSTANTIATE_TEST_SUITE_P(OpenGLDataType, OpenGLDataType_testFromAttribute, ::testing::Values(
	std::make_tuple(dagui::AttributeDescriptor::TYPE_UNKNOWN, 0),
	std::make_tuple(dagui::AttributeDescriptor::TYPE_BYTE, GL_UNSIGNED_BYTE),
	std::make_tuple(dagui::AttributeDescriptor::TYPE_INT32, GL_INT),
	std::make_tuple(dagui::AttributeDescriptor::TYPE_UINT32, GL_UNSIGNED_INT),
	std::make_tuple(dagui::AttributeDescriptor::TYPE_FLOAT, GL_FLOAT),
	std::make_tuple(dagui::AttributeDescriptor::TYPE_DOUBLE, GL_DOUBLE)
	));

class LayerAttributes_testSort : public ::testing::TestWithParam<std::tuple<const char*, const char*, bool>>
{

};

TEST_P(LayerAttributes_testSort, testCorrectOrder)
{
	auto configStr = std::get<0>(GetParam());
	dagbase::Lua lua;
	auto config = dagbase::ConfigurationElement::fromFile(lua, configStr);
	ASSERT_NE(nullptr, config);
	auto testConfigName = std::get<1>(GetParam());
	dagbase::ConfigurationElement* testConfig = config->findElement(testConfigName);
	ASSERT_NE(nullptr, testConfig);
	dagui::LayerAttributes op1;
	if (auto element = testConfig->findElement("op1"); element)
		op1.configure(*element);
	dagui::LayerAttributes op2;
	if (auto element = testConfig->findElement("op2"); element)
		op2.configure(*element);
	auto order = std::get<2>(GetParam());
	EXPECT_EQ(order, op1 < op2);
}

INSTANTIATE_TEST_SUITE_P(LayerAttributes, LayerAttributes_testSort, ::testing::Values(
	std::make_tuple("data/tests/LayerAttributes/testSort.lua", "texture", true)
	));

class GenericMesh_addVertex : public ::testing::TestWithParam<std::tuple<const char*, const char*, TestVertexField, std::size_t, float>>
{

};

TEST_P(GenericMesh_addVertex, testValue)
{
	auto configStr = std::get<0>(GetParam());
	dagbase::Lua lua;
	auto config = dagbase::ConfigurationElement::fromFile(lua, configStr);
	ASSERT_NE(nullptr, config);
	auto testConfigName = std::get<1>(GetParam());
	auto testConfig = config->findElement(testConfigName);
	ASSERT_NE(nullptr, testConfig);
	auto field = std::get<2>(GetParam());
	dagui::GenericMesh<TestVertex> sut;
	if (auto element = testConfig->findElement("mesh"); element)
		sut.configure(*element);
	if (auto element = config->findElement("vertices"); element)
	{
		element->eachChild([&sut](dagbase::ConfigurationElement& child)
		{
			TestVertex v;

			v.configure(child);
			sut.addVertex(v);

			return true;
		});
	}

	TestVertex actualVertex;
	auto index = std::get<3>(GetParam());
	auto value = std::get<4>(GetParam());
	ASSERT_LT(index, sut.size());
	sut.getVertex(index, &actualVertex);
	EXPECT_EQ(value, actualVertex.*field);
}

INSTANTIATE_TEST_SUITE_P(GenericMesh, GenericMesh_addVertex, ::testing::Values(
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "separate", &TestVertex::x, 0, 1.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "separate", &TestVertex::y, 0, 2.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "separate", &TestVertex::r, 0, 1.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "separate", &TestVertex::g, 0, 0.5f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "separate", &TestVertex::b, 0, 0.2f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "separate", &TestVertex::a, 0, 0.8f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "separate", &TestVertex::x, 2, 2.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "separate", &TestVertex::y, 2, 3.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "separate", &TestVertex::r, 2, 1.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "separate", &TestVertex::g, 2, 0.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "separate", &TestVertex::b, 2, 1.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "separate", &TestVertex::a, 2, 1.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "interleaved", &TestVertex::x, 0, 1.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "interleaved", &TestVertex::y, 0, 2.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "interleaved", &TestVertex::r, 0, 1.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "interleaved", &TestVertex::g, 0, 0.5f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "interleaved", &TestVertex::b, 0, 0.2f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "interleaved", &TestVertex::a, 0, 0.8f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "interleaved", &TestVertex::x, 2, 2.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "interleaved", &TestVertex::y, 2, 3.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "interleaved", &TestVertex::r, 2, 1.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "interleaved", &TestVertex::g, 2, 0.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "interleaved", &TestVertex::b, 2, 1.0f),
	std::make_tuple("data/tests/GenericMesh/addVertex.lua", "interleaved", &TestVertex::a, 2, 1.0f)
));

class DrawCommandBuffer_testShapeGeneratesGeometry : public ::testing::TestWithParam<std::tuple<const char*, const char*, dagbase::Variant, double, dagbase::ConfigurationElement::RelOp>>
{

};
/*
class MockDrawCommandBuffer : public dagui::DrawCommandBuffer
{
public:
    MockDrawCommandBuffer()
    {
        ON_CALL(*this, find).WillByDefault([](std::string_view path) {
            dagbase::Variant retval;

            retval = dagbase::findEndpoint(path, "numPoints", _points.size());
            return dagbase::Variant();
        });
    }
public:
    MOCK_METHOD(void, drawRect, (const dagui::Rectangle&), (override));
    MOCK_METHOD(dagbase::Variant, find, (std::string_view), (const,override));
private:
};
*/

TEST_P(DrawCommandBuffer_testShapeGeneratesGeometry, testExpectedValue)
{
    auto configStr = std::get<0>(GetParam());
    dagbase::Lua lua;
    auto config = dagbase::ConfigurationElement::fromFile(lua, configStr);
    ASSERT_NE(nullptr, config);
    dagui::WidgetFactory factory;
    auto shape = factory.createShape(*config);
    ASSERT_NE(nullptr, shape);
    auto path = std::get<1>(GetParam());
    auto value = std::get<2>(GetParam());
    auto tolerance = std::get<3>(GetParam());
    auto op = std::get<4>(GetParam());
    auto sut = new dagui::DrawCommandBuffer();
    shape->render(*sut);
    auto actualValue = sut->find(path);

    assertComparison(value, actualValue, tolerance, op);
}

INSTANTIATE_TEST_SUITE_P(DrawCommandBuffer, DrawCommandBuffer_testShapeGeneratesGeometry, ::testing::Values(
        std::make_tuple("data/tests/DrawCommandBuffer/rectangle.lua", "numCommands", std::int64_t(1), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/DrawCommandBuffer/rectangle.lua", "commands[0].op", std::string("OP_RECTANGLE"), 0.0, dagbase::ConfigurationElement::RELOP_EQ)
        ));
