//
// Created by Tony Horrobin on 06/04/2025.
//

#include "config/config.h"

#include "core/WidgetFactory.h"
#include "core/LuaInterface.h"
#include "core/ConfigurationElement.h"
#include "core/Widget.h"
#include "core/ResolveWidgetRefsVisitor.h"
#include "core/ConstraintSolverVisitor.h"
#include "core/LayoutConstraintsCollectionVisitor.h"
#include "core/LayoutProperties.h"
#include "core/ShapeFactory.h"
#include "core/Batcher.h"
#include "test/TestUtils.h"
#include "core/GraphicsBackendFactory.h"
#include "gfx/TextureAtlas.h"
#include "gfx/TextureAtlasBackend.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "MockGraphicsBackendFactory.h"
#include "core/DataTemplate.h"

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
    dagui::ShapeFactory shapeFactory;
    FT_Library freeTypeLib = nullptr;
    FT_Init_FreeType(&freeTypeLib);
    sut.setFreeType(freeTypeLib);
    auto tree = sut.create(*config, shapeFactory);
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
    FT_Done_FreeType(freeTypeLib);
}

INSTANTIATE_TEST_SUITE_P(WidgetFactory, WidgetFactory_testCreate, ::testing::Values(
    std::make_tuple("data/tests/WidgetFactory/empty.lua", &dagbase::ConfigurationElement::fromFile, false, "content", dagbase::ConfigurationElement::ValueType(), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/oneWindow.lua", &dagbase::ConfigurationElement::fromFile, true, "id", std::string("test"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/oneWindowWithChild.lua", &dagbase::ConfigurationElement::fromFile, true, "numChildren", std::int64_t(1), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/rootWithNestedChildren.lua", &dagbase::ConfigurationElement::fromFile, true, "lookup.test.title", std::string("Hello, Dagui!"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/rootWithNestedChildren.lua", &dagbase::ConfigurationElement::fromFile, true, "lookup.clickme.text.text", std::string("Click me"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/rootWithNestedChildren.lua", &dagbase::ConfigurationElement::fromFile, true, "lookup.root.numChildren", std::int64_t(1), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/windowWithLayout.lua", &dagbase::ConfigurationElement::fromFile, true, "lookup.vertical.numChildren", std::int64_t(2), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/windowWithLayout.lua", &dagbase::ConfigurationElement::fromFile, true, "lookup.label.text.text", std::string("A label"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/windowWithLayout.lua", &dagbase::ConfigurationElement::fromFile, true, "width", std::int64_t{ 1920 }, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/windowWithLayout.lua", &dagbase::ConfigurationElement::fromFile, true, "height", std::int64_t{ 1080 }, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/rootWithLabel.lua", &dagbase::ConfigurationElement::fromFile, true, "lookup.greeting.x", std::int64_t{ 256 }, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
    std::make_tuple("data/tests/WidgetFactory/rootWithLabel.lua", &dagbase::ConfigurationElement::fromFile, true, "lookup.greeting.y", std::int64_t{ 128 }, 0.0, dagbase::ConfigurationElement::RELOP_EQ)
    ));

class Widget_testProperties : public ::testing::TestWithParam<std::tuple<const char*, const char*, dagbase::ConfigurationElement::ValueType, double, dagbase::ConfigurationElement::RelOp>>
{

};

TEST_P(Widget_testProperties, testExpectedValue)
{
    auto configStr = std::get<0>(GetParam());
    dagbase::Lua lua;
    auto config = dagbase::ConfigurationElement::fromFile(lua, configStr);
    ASSERT_NE(nullptr, config);
    dagui::WidgetFactory factory;
    dagui::ShapeFactory shapeFactory;
    auto sut = factory.create(*config, shapeFactory);
    ASSERT_NE(nullptr, sut);
    auto path = std::get<1>(GetParam());
    auto value = std::get<2>(GetParam());
    auto tolerance = std::get<3>(GetParam());
    auto op = std::get<4>(GetParam());
    dagui::ResolveWidgetRefsVisitor resolver;
    sut->accept(resolver);
    auto actualValue = sut->find(path);
    assertComparison(value, actualValue, tolerance, op);
}

INSTANTIATE_TEST_SUITE_P(Widget, Widget_testProperties, ::testing::Values(
        std::make_tuple("data/tests/Widget/Label.lua", "text.text", "test", 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Widget/WindowWithVerticalLayout.lua", "lookup.test.layoutProperties.horizAlign", std::int64_t(dagui::LayoutProperties::HORIZ_ALIGN_CENTRE), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Widget/WindowWithVerticalLayout.lua", "lookup.test.layoutProperties.verticalAlign", std::int64_t(dagui::LayoutProperties::VERTICAL_ALIGN_CENTRE), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Widget/WindowWithVerticalLayout.lua", "lookup.test.layoutProperties.spacing", 5.0, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Widget/WindowWithVerticalLayout.lua", "lookup.test.layoutProperties.padding[0]", 1.0, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Widget/WindowWithVerticalLayout.lua", "lookup.label1.styleClass", std::string("label"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Widget/WindowWithVerticalLayout.lua", "lookup.label1.shapes[0].class", std::string("Rectangle"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Widget/WindowWithVerticalLayout.lua", "styles.label.border", 4.0, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Widget/WindowWithVerticalLayout.lua", "lookup.label1.shapes[0].cornerRadius", 16.0, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Widget/WindowWithVerticalLayout.lua", "lookup.label1.style.border", 4.0, 0.0, dagbase::ConfigurationElement::RELOP_EQ)
        ));

class WidgetRef_testResolve : public ::testing::TestWithParam<std::tuple<const char*, const char*, bool>>
{

};

TEST_P(WidgetRef_testResolve, testExpectedNotNull)
{
    auto configStr = std::get<0>(GetParam());
    auto id = std::get<1>(GetParam());
    auto notNull = std::get<2>(GetParam());
    dagbase::Lua lua;
    auto config = dagbase::ConfigurationElement::fromFile(lua, configStr);
    ASSERT_NE(nullptr, config);
    dagui::WidgetFactory factory;
    dagui::ShapeFactory shapeFactory;
    auto widget = factory.create(*config, shapeFactory);
    ASSERT_NE(nullptr, widget);
    dagui::WidgetRef sut;
    sut.setId(dagbase::Atom::intern(id));
    sut.resolve(widget);
    auto actual = sut.ref();
    EXPECT_EQ(notNull, actual!=nullptr);
}

INSTANTIATE_TEST_SUITE_P(WidgetRef, WidgetRef_testResolve, ::testing::Values(
        std::make_tuple("data/tests/WidgetRef/Lookup.lua", "spoo", false),
        std::make_tuple("data/tests/WidgetRef/Lookup.lua", "label1", true)
        ));

class ResolveWidgetRefsVisitor_testVisit : public ::testing::TestWithParam<std::tuple<const char*, const char*, dagbase::Variant, double, dagbase::ConfigurationElement::RelOp>>
{

};

TEST_P(ResolveWidgetRefsVisitor_testVisit, testExpectedValue)
{
    auto configStr = std::get<0>(GetParam());
    dagbase::Lua lua;
    auto config = dagbase::ConfigurationElement::fromFile(lua, configStr);
    ASSERT_NE(nullptr, config);
    dagui::ResolveWidgetRefsVisitor sut;
    dagui::WidgetFactory factory;
    dagui::ShapeFactory shapeFactory;
    auto widget = factory.create(*config, shapeFactory);
    ASSERT_NE(nullptr, widget);
    widget->accept(sut);
    auto path = std::get<1>(GetParam());
    auto value = std::get<2>(GetParam());
    auto tolerance = std::get<3>(GetParam());
    auto op = std::get<4>(GetParam());
    auto actualValue = widget->find(path);
    assertComparison(value, actualValue, tolerance, op);
}

INSTANTIATE_TEST_SUITE_P(ResolveWidgetRefsVisitor, ResolveWidgetRefsVisitor_testVisit, ::testing::Values(
        std::make_tuple("data/tests/WidgetRef/WithConstraints.lua", "lookup.label1.constraints[0].firstItem.id", std::string("label1"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/WidgetRef/WithConstraints.lua", "lookup.label1.constraints[0].secondItem.id", std::string("window1"), 0.0, dagbase::ConfigurationElement::RELOP_EQ)
        ));

class ConstraintSolverVisitor_testVisit : public ::testing::TestWithParam<std::tuple<const char*, const char*, dagbase::Variant, double, dagbase::ConfigurationElement::RelOp>>
{

};

TEST_P(ConstraintSolverVisitor_testVisit, testExpectedValue)
{
    auto configStr = std::get<0>(GetParam());
    auto path = std::get<1>(GetParam());
    auto value = std::get<2>(GetParam());
    auto tolerance = std::get<3>(GetParam());
    auto op = std::get<4>(GetParam());

    dagbase::Lua lua;
    auto config = dagbase::ConfigurationElement::fromFile(lua, configStr);
    ASSERT_NE(nullptr, config);
    dagui::WidgetFactory factory;
    dagui::ShapeFactory shapeFactory;
    dagui::Widget* widgetTree = factory.create(*config, shapeFactory);
    ASSERT_NE(nullptr, widgetTree);
    dagui::ResolveWidgetRefsVisitor resolver;
    widgetTree->accept(resolver);
    dagui::ConstraintSolverVisitor sut;

    widgetTree->accept(sut);
    auto actualValue = widgetTree->find(path);
    assertComparison(value, actualValue, tolerance, op);
}

INSTANTIATE_TEST_SUITE_P(ConstraintSolverVisitor, ConstraintSolverVisitor_testVisit, ::testing::Values(
        std::make_tuple("data/tests/ConstraintSolverVisitor/NoConstraints.lua", "x", std::int64_t{0}, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/ConstraintSolverVisitor/NoConstraints.lua", "y", std::int64_t{0}, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/ConstraintSolverVisitor/Position.lua", "lookup.test.x", std::int64_t{100}, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/ConstraintSolverVisitor/Position.lua", "lookup.test.y", std::int64_t{200}, 0.0, dagbase::ConfigurationElement::RELOP_EQ)
        ));

class LayoutConstraintsCollectionVisitor_testVisit : public ::testing::TestWithParam<std::tuple<const char*, const char *, dagbase::Variant, double, dagbase::ConfigurationElement::RelOp>>
{

};

TEST_P(LayoutConstraintsCollectionVisitor_testVisit, testExpectedCount)
{
    auto configStr = std::get<0>(GetParam());
    dagbase::Lua lua;
    auto config = dagbase::ConfigurationElement::fromFile(lua, configStr);
    ASSERT_NE(nullptr, config);
    dagui::LayoutConstraintsCollectionVisitor sut;
    dagui::WidgetFactory factory;
    dagui::ShapeFactory shapeFactory;
    auto widgetTree = factory.create(*config, shapeFactory);
    ASSERT_NE(nullptr, widgetTree);
    widgetTree->accept(sut);
    auto path = std::get<1>(GetParam());
    auto value = std::get<2>(GetParam());
    auto tolerance = std::get<3>(GetParam());
    auto op = std::get<4>(GetParam());
    auto actualValue = sut.find(path);
    assertComparison(value, actualValue, tolerance, op);
}

INSTANTIATE_TEST_SUITE_P(LayoutConstraintsCollectionVisitor, LayoutConstraintsCollectionVisitor_testVisit, ::testing::Values(
        std::make_tuple("data/tests/LayoutConstraintsCollectionVisitor/NoConstraints.lua", "numConstraints", std::int64_t{0}, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/LayoutConstraintsCollectionVisitor/OneConstraint.lua", "numConstraints", std::int64_t{1}, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/LayoutConstraintsCollectionVisitor/OneConstraint.lua", "constraints[0].constant", double{100}, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/LayoutConstraintsCollectionVisitor/Vertical.lua", "constraints[0].firstItem", std::string("test"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/LayoutConstraintsCollectionVisitor/Vertical.lua", "constraints[0].firstAttribute", std::string("Attribute::WIDTH"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/LayoutConstraintsCollectionVisitor/Vertical.lua", "constraints[0].relation", std::string("Relation::GE"), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/LayoutConstraintsCollectionVisitor/Vertical.lua", "constraints[0].constant", 50.0, 0.0, dagbase::ConfigurationElement::RELOP_EQ)
        ));

class Widget_testDraw : public ::testing::TestWithParam<std::tuple<const char*, const char*, const char*, dagbase::Variant, double, dagbase::ConfigurationElement::RelOp>>
{

};

TEST_P(Widget_testDraw, testExpectedValue)
{
    auto widgetConfigStr = std::get<0>(GetParam());
    dagbase::ConfigurationElement* widgetConfig = nullptr;
    {
        dagbase::Lua lua;

        widgetConfig = dagbase::ConfigurationElement::fromFile(lua, widgetConfigStr);
        ASSERT_NE(nullptr, widgetConfig);
    }
    auto batcherConfigStr = std::get<1>(GetParam());
    dagbase::ConfigurationElement* batcherConfig = nullptr;
    {
        dagbase::Lua lua;

        batcherConfig = dagbase::ConfigurationElement::fromFile(lua, batcherConfigStr);
        ASSERT_NE(nullptr, batcherConfig);
    }

    dagui::WidgetFactory widgetFactory;
    dagui::ShapeFactory shapeFactory;
    FT_Library lib = nullptr;
    FT_Init_FreeType(&lib);
    shapeFactory.setFreeTypeLib(lib);
    dagbase::ConfigurationElement* shapeFactoryConfig = nullptr;
    {
        dagbase::Lua lua;

        shapeFactoryConfig = dagbase::ConfigurationElement::fromFile(lua, "etc/ShapeFactory.lua");
        ASSERT_NE(nullptr, shapeFactoryConfig);
    }
    shapeFactory.configure(*shapeFactoryConfig);
    auto widgetTree = widgetFactory.create(*widgetConfig, shapeFactory);
    ASSERT_NE(nullptr, widgetTree);
    dagui::Batcher batcher;
    MockGraphicsBackendFactory backendFactory;
    EXPECT_CALL(backendFactory, createPositionStack()).Times(::testing::AtLeast(1));
    batcher.configure(*batcherConfig, backendFactory);
    EXPECT_CALL(backendFactory, createMesh(::testing::_)).Times(::testing::AtLeast(1));
    EXPECT_CALL(backendFactory, createTextureAtlas(::testing::_)).Times(::testing::AtLeast(1));
    EXPECT_CALL(backendFactory, createBlending()).Times(::testing::AtLeast(1));
    widgetTree->draw(batcher, backendFactory);
    auto path = std::get<2>(GetParam());
    auto value = std::get<3>(GetParam());
    auto tolerance = std::get<4>(GetParam());
    auto op = std::get<5>(GetParam());
    auto actualValue = batcher.find(path);
    assertComparison(value, actualValue, tolerance, op);
    FT_Done_FreeType(lib);
    delete batcherConfig;
    delete widgetTree;
    delete widgetConfig;
}

INSTANTIATE_TEST_SUITE_P(Widget, Widget_testDraw, ::testing::Values(
        std::make_tuple("data/tests/Widget/Label.lua", "data/tests/Widget/TwoBins.lua", "renderBins[0].mesh.numVertices", std::uint32_t(0), 0.0, dagbase::ConfigurationElement::RELOP_GT),
        std::make_tuple("data/tests/Widget/Label.lua", "data/tests/Widget/TwoBins.lua", "renderBins[1].mesh.numTriangles", std::uint32_t(8), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Widget/RootWithLabel.lua", "data/tests/Widget/TwoBins.lua", "renderBins[1].mesh.numTriangles", std::uint32_t(8), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Widget/RootWithLabel.lua", "data/tests/Widget/TwoBins.lua", "renderBins[0].mesh.numVertices", std::uint32_t(1280/32), 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Widget/RootWithLabel.lua", "data/tests/Widget/TwoBins.lua", "renderBins[1].mesh.numVertices", std::uint32_t(1280/32), 0.0, dagbase::ConfigurationElement::RELOP_LT),
        std::make_tuple("data/tests/Widget/RootWithLabel.lua", "data/tests/Widget/TwoBins.lua", "x", 256.0, 0.0, dagbase::ConfigurationElement::RELOP_EQ),
        std::make_tuple("data/tests/Widget/RootWithLabel.lua", "data/tests/Widget/TwoBins.lua", "y", 128.0, 0.0, dagbase::ConfigurationElement::RELOP_EQ)
        ));

class DataTemplate_testInstantiate : public ::testing::TestWithParam<std::tuple<const char*>>
{
public:
    struct Test
    {
        std::string name;
        std::string description;

        void configure(dagbase::ConfigurationElement& config)
        {
            dagbase::ConfigurationElement::readConfig(config, "name", &name);
            dagbase::ConfigurationElement::readConfig(config, "description", &description);
        }

        void setParameters(dagui::DataTemplate& templ)
        {
            templ.setParameter(dagbase::Atom::intern("name"), name);
            templ.setParameter(dagbase::Atom::intern("description"), description);
        }

        dagbase::Variant find(std::string_view path) const
        {
            dagbase::Variant retval;

            retval = dagbase::findEndpoint(path, "name", name);
            if (retval.has_value())
                return retval;

            retval = dagbase::findEndpoint(path, "description", description);
            if (retval.has_value())
                return retval;

            return {};
        }
    };

    struct Assertion
    {
        std::string path;
        dagbase::Variant value;
        double tolerance{0.0};
        dagbase::ConfigurationElement::RelOp op{dagbase::ConfigurationElement::RELOP_EQ};

        void configure(dagbase::ConfigurationElement& config)
        {
            dagbase::ConfigurationElement::readConfig(config, "path", &path);
            dagbase::ConfigurationElement::readConfig(config, "value", &value);
            dagbase::ConfigurationElement::readConfig(config, "tolerance", &tolerance);
            dagbase::ConfigurationElement::readConfig<dagbase::ConfigurationElement::RelOp>(config, "op", &dagbase::ConfigurationElement::parseRelOp, &op);
        }

        void makeItSo(dagui::DataTemplate& sut)
        {
            assertComparison(value, sut.find(path), tolerance, op);
        }
    };
public:
    void configure(dagbase::ConfigurationElement& config)
    {
        if (auto element=config.findElement("assertions"); element)
        {
            element->eachChild([this](dagbase::ConfigurationElement& child) {
                Assertion entry;

                entry.configure(child);
                _assertions.emplace_back(entry);

                return true;
            });
        }
    }

    void makeItSo()
    {
        if (_sut)
            for (auto a : _assertions)
            {
                a.makeItSo(*_sut);
            }
    }

protected:
    dagui::DataTemplate* _sut{nullptr};
    using AssertionArray = std::vector<Assertion>;
    AssertionArray _assertions;
    Test _test;
};

TEST_P(DataTemplate_testInstantiate, testAssertions)
{
    auto configStr = std::get<0>(GetParam());
    dagbase::ConfigurationElement* config = nullptr;
    {
        dagbase::Lua lua;

        config = dagbase::ConfigurationElement::fromFile(lua, configStr);
        ASSERT_NE(nullptr, config);
    }

    configure(*config);
    _sut = new dagui::DataTemplate();
    dagui::ShapeFactory shapeFactory;
    dagui::WidgetFactory widgetFactory;
    if (auto templateConfig = config->findElement("template"); templateConfig)
    {
        _sut->configure(*templateConfig, widgetFactory, shapeFactory);

    }
    else
    {
        FAIL() << "Expected template config to exist";
    }
    Test test;
    if (auto element=config->findElement("instance"); element)
    {
        test.configure(*element);
    }
    test.setParameters(*_sut);
    _sut->resolve();
    makeItSo();
}

INSTANTIATE_TEST_SUITE_P(DataTemplate, DataTemplate_testInstantiate, ::testing::Values(
        std::make_tuple("data/tests/Widget/Template.lua")
        ));