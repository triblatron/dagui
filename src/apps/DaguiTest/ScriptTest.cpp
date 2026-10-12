//
// Created by Tony Horrobin on 11/10/2026.
//

#include "config/config.h"

#include "core/LuaInterface.h"
#include "core/LuaScript.h"

#include <gtest/gtest.h>

#include "../../../ThirdParty/dag/include/MemoryNodeLibrary.h"
#include "core/Node.h"


class Script_testExecute : public ::testing::TestWithParam<std::tuple<const char*, const char*>>
{

};

TEST_P(Script_testExecute, testExpectedResult)
{
    auto scriptFilename = std::get<0>(GetParam());
    auto mainName = std::get<1>(GetParam());
    dagbase::Lua lua;
    lua.execute(scriptFilename);
    ASSERT_TRUE(lua.ok());
    dagui::LuaScript sut(&lua);
    dag::MemoryNodeLibrary nodeLib;
    sut.setMainName(mainName);
    auto node = nodeLib.instantiateEmptyNode(nodeLib, "Boundary");
    ASSERT_EQ(0, node->totalPorts());
    ASSERT_NE(nullptr, node);
    sut.step(node);
    ASSERT_EQ(1, node->totalPorts());
    ASSERT_EQ(1.0, std::get<double>(node->dynamicPort(0)->value().value()));
    sut.step(node);
    EXPECT_TRUE(sut.isFinished());
    sut.reset();
    sut.step(node);
    ASSERT_EQ(2, node->totalPorts());
    ASSERT_EQ(1.0, std::get<double>(node->dynamicPort(1)->value().value()));
    sut.step(node);
    EXPECT_TRUE(sut.isFinished());
}

INSTANTIATE_TEST_SUITE_P(Script, Script_testExecute, ::testing::Values(
    std::make_tuple("data/tests/Script/Resume.lua", "test2")
    ));