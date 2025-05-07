//
// Created by Tony Horrobin on 07/05/2025.
//

#include "core/SceneNodeFactory.h"
#include "core/SceneNode.h"
#include "core/LuaInterface.h"

#include <gtest/gtest.h>

class SceneNodeFactory_testCreate : public ::testing::TestWithParam<std::tuple<const char*, dagbase::IdentifierGenerator::Identifier, bool, const char*>>
{

};

TEST_P(SceneNodeFactory_testCreate, testExpectedIdentifier)
{
    auto configStr = std::get<0>(GetParam());
    dagbase::Lua lua;
    auto config = dagbase::ConfigurationElement::fromFile(lua, configStr);
    ASSERT_NE(nullptr, config);
    dagui::SceneNodeFactory sut;
    auto typeName = dagbase::Atom::intern(std::get<3>(GetParam()));
    config->eachChild([this, &sut, &typeName](dagbase::ConfigurationElement& child) {
        std::string cmd;
        if (auto element = child.findElement("cmd"); element)
        {
            cmd = element->asString();
        }

        std::string className;
        if (auto element = child.findElement("class"); element)
        {
            className = element->asString();
        }

        dagbase::IdentifierGenerator::Identifier expectedId{std::numeric_limits<dagbase::IdentifierGenerator::Identifier>::max()};

        if (auto element=child.findElement("expectedId"); element)
        {
            expectedId = element->asInteger();
        }

        dagbase::IdentifierGenerator::Identifier id{std::numeric_limits<dagbase::IdentifierGenerator::Identifier>::max()};

        if (auto element=child.findElement("id"); element)
        {
            id = element->asInteger();
        }

        if (cmd == "create")
        {
            auto actual = sut.createNode(dagbase::Atom::intern(className), nullptr);
            EXPECT_NE(nullptr, actual);
            EXPECT_EQ(expectedId, actual->id());
            EXPECT_EQ(dagbase::Atom::intern(className), actual->typeName());
        }
        else if (cmd == "delete")
        {
            sut.deleteNode(id);
        }

        return true;
    });
    auto id = std::get<1>(GetParam());
    auto notNull = std::get<2>(GetParam());
    auto actual = sut.nodeByID(id);
    EXPECT_EQ(notNull, actual!= nullptr);
}

INSTANTIATE_TEST_SUITE_P(SceneNodeFactory, SceneNodeFactory_testCreate, ::testing::Values(
        std::make_tuple("data/tests/SceneNodeFactory/None.lua", 0, false, ""),
        std::make_tuple("data/tests/SceneNodeFactory/One.lua", 0, true, "Group"),
        std::make_tuple("data/tests/SceneNodeFactory/Two.lua", 1, true, "Group"),
        std::make_tuple("data/tests/SceneNodeFactory/Recycle.lua", 0, true, "Text")
        ));