//
// Created by Tony Horrobin on 11/10/2026.
//

#include <lua_runtime.h>

#include "config/config.h"

#include "core/LuaInterface.h"
#include "core/LuaScript.h"
#include "MemoryNodeLibrary.h"
#include "core/Node.h"
#include "core/ScriptProtocol.h"
#include "core/Graph.h"
#include "NodeEditorLive.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


class MockScriptProtocol : public dagbase::ScriptProtocol
{
public:
    MOCK_METHOD(void, request, (std::uint32_t, const std::pmr::vector<dagbase::Variant>&), (override));
};

struct ScriptExistingItem
{
    using PropertyMap = dagbase::VectorMap<std::string, dagbase::Variant>;

    void configure(dagbase::ConfigurationElement& config)
    {
        if (auto element = config.findElement("properties"); element)
        {
            element->eachChild([this](dagbase::ConfigurationElement& child) {
                std::string name = child.name();
                dagbase::Variant entry = child.value();

                properties.emplace(name, entry);

                return true;
            });
        }

        dagbase::ConfigurationElement::readConfig(config, "count", &count);
        dagbase::ConfigurationElement::readConfig(config, "class", &className);
    }

    template<typename Obj>
    static bool areAllTrue(const PropertyMap& properties, const Obj* obj)
    {
        bool allTrue = true;
        for (auto p : properties)
        {
            auto actual = obj->find(p.first);
            auto casted = p.second.cast(actual.index());
            if (actual.has_value() != casted.has_value() ||  (actual.has_value() && actual != casted))
            {
                allTrue = false;
            }
        }

        return allTrue;
    }

    void makeItSo(dag::NodeEditorLive& nodeEditor)
    {
        std::uint32_t actualCount{0};
        // Check that properties exist
        if (className == "Node")
        {
            nodeEditor.activeGraph()->eachNode([this, &actualCount](const dagbase::Node* node) {
                bool allTrue = true;
                bool checked = false;
                if (!properties.empty())
                {
                    checked = true;
                    allTrue = allTrue && areAllTrue(properties, node);
                }

                if (checked && allTrue)
                {
                    ++actualCount;
                }

                return true;
            });
        }
        else if (className == "Port")
        {
            nodeEditor.activeGraph()->eachPort([this, &actualCount](const dagbase::Port* port) {
                bool allTrue = true;
                bool checked = false;
                if (!properties.empty())
                {
                    checked = true;
                    allTrue = allTrue && areAllTrue(properties, port);
                }

                if (checked && allTrue)
                {
                    ++actualCount;
                }

                return true;
            });
        }

        std::ostringstream propertyStr;
        for (const auto& prop : properties)
        {
            propertyStr << prop.first << " = " << prop.second << '\n';
        }
        ASSERT_EQ(count, actualCount) << "Expected class " << className << " to have properties:\n" << propertyStr.str();

    }

    PropertyMap properties;
    std::uint32_t count{1};
    std::string className;
};

class Script_testExecute : public ::testing::TestWithParam<std::tuple<const char*, const char*>>
{
public:
    void configure(dagbase::ConfigurationElement& config)
    {
        dagbase::ConfigurationElement::readConfigVector(config, "existing", &_existing);
    }

    void makeItSo(dag::NodeEditorLive& nodeEditor)
    {
        for (auto& e : _existing)
        {
            e.makeItSo(nodeEditor);
        }
    }
protected:
    using ExistingItemArray = std::vector<ScriptExistingItem>;
    ExistingItemArray _existing;
};

TEST_P(Script_testExecute, testExpectedResult)
{
    auto scriptFilename = std::get<0>(GetParam());
    auto mainName = std::get<1>(GetParam());
    dagbase::Lua lua;
    auto config = dagbase::ConfigurationElement::fromFile(lua, scriptFilename);
    ASSERT_NE(nullptr, config);
    configure(*config);
    ASSERT_TRUE(lua.ok());
    MockScriptProtocol proto;
    EXPECT_CALL(proto, request(1, ::testing::_)).Times(1);
    dag::NodeEditorLive nodeEditor;
    dagui::LuaScript sut(&lua, &nodeEditor, &proto);

    sut.setMainName(mainName);
    auto status = nodeEditor.createNode("Boundary", "test1");
    sut.step(nodeEditor.activeGraph()->node(std::get<dagbase::NodeID>(status.result.value())));
    makeItSo(nodeEditor);
    // ASSERT_EQ(1, node->totalPorts());
    // ASSERT_EQ(1.0, std::get<double>(node->dynamicPort(0)->value().value()));
    // sut.step(node);
    // EXPECT_TRUE(sut.isFinished());
    // sut.reset();
    // sut.step(node);
    // ASSERT_EQ(2, node->totalPorts());
    // ASSERT_EQ(1.0, std::get<double>(node->dynamicPort(1)->value().value()));
    // sut.step(node);
    // EXPECT_TRUE(sut.isFinished());
}

INSTANTIATE_TEST_SUITE_P(Script, Script_testExecute, ::testing::Values(
    std::make_tuple("data/tests/Script/Resume.lua", "test2")
    ));

TEST(Script, testConvertObj)
{
    dagbase::Lua sut;
    sut.eval("require \"luadag\"");
    dag::NodeEditorLive nodeEditor;
    auto status = nodeEditor.createNode("Boundary", "test1");
    ASSERT_EQ(dagbase::Status::STATUS_OK, status.status);
    auto node = nodeEditor.activeGraph()->node(std::get<dagbase::NodeID>(status.result.value()));
    auto nodeType = SWIG_TypeQuery(sut.get(),"_p_dagbase__Node");
    ASSERT_NE(nullptr, nodeType);
    sut.pushObj(node, nodeType);
    dagbase::Node* node2 = nullptr;
    sut.convertObj((void**)&node2, nodeType);
    ASSERT_EQ(node, node2);
}
