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
#include "util/enums.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/NodeEditorScriptProtocol.h"
#include "test/TestUtils.h"
#include "util/CharConv.h"


class MockScriptProtocol : public dagbase::ScriptProtocol
{
public:
    MOCK_METHOD(void, request, (dagbase::Node*, std::uint32_t, const std::pmr::vector<dagbase::Variant>&, std::vector<dagbase::Variant>*), (override));
};

//! An implementation of ScriptProtocol that records calls and forwards to another implementation to do the work.
class FakeScriptProtocol : public dagbase::ScriptProtocol
{
public:
    FakeScriptProtocol(ScriptProtocol* proto)
        :
    _proto(proto)
    {
        ON_CALL(*this, request).WillByDefault([this](dagbase::Node* self, std::uint32_t opCode, const std::pmr::vector<dagbase::Variant>& args, std::vector<dagbase::Variant>* results) {
            if (_proto)
            {
                _proto->request(self, opCode, args, results);
            }
        });
    }

    MOCK_METHOD(void, request, (dagbase::Node*, std::uint32_t, const std::pmr::vector<dagbase::Variant>&, std::vector<dagbase::Variant>*), (override));
private:
    ScriptProtocol* _proto{nullptr};
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

class HelloService
{
public:
    void hello()
    {
        ++_helloCount;
    }
private:
    int _helloCount{0};
};

struct ScriptAssertion
{
    void configure(dagbase::ConfigurationElement& config)
    {
        dagbase::ConfigurationElement::readConfig(config, "path", &path);
        dagbase::ConfigurationElement::readConfig(config, "value", &value);
        dagbase::ConfigurationElement::readConfig<dagbase::ConfigurationElement::RelOp>(config, "op", &dagbase::ConfigurationElement::parseRelOp, &op);
        dagbase::ConfigurationElement::readConfig(config, "tolerance", &tolerance);
    }

    void makeItSo(dagbase::Script& sut)
    {
        auto actual = sut.find(path);
        assertComparison(value, actual, tolerance, op, path.c_str());
    }

    std::string path;
    dagbase::Variant value;
    dagbase::ConfigurationElement::RelOp op{dagbase::ConfigurationElement::RELOP_EQ};
    double tolerance{0.0};
};

struct ScriptCommand
{
    enum Command : std::uint32_t
    {
        CMD_UNKNOWN,
        CMD_CONFIGURE,
        CMD_STEP_UNTIL_FINISHED,
        CMD_RESET,
        CMD_UPDATE
    };

    void configure(dagbase::ConfigurationElement& config)
    {
        dagbase::ConfigurationElement::readConfig<Command>(config, "cmd", &parseCommand, &cmd);
        dagbase::ConfigurationElement::readConfigVector(config, "existing", &existing);
        dagbase::ConfigurationElement::readConfigVector(config, "assertions", &assertions);
        _scriptConfig = &config;
    }

    void makeItSo(dagbase::Script& sut, dagbase::Node* self, dag::NodeEditorLive& nodeEditor)
    {
        switch (cmd)
        {
            case CMD_CONFIGURE:
            {
                if (_scriptConfig)
                {
                    sut.configure(*_scriptConfig);
                }
                else
                {
                    FAIL() << "Expected valid scriptConfig";
                }
            }
            case CMD_STEP_UNTIL_FINISHED:
            {
                while (!sut.isFinished())
                {
                    sut.step(self);
                }
                break;
            }
            case CMD_RESET:
            {
                sut.reset();

                break;
            }
            case CMD_UPDATE:
            {
                self->update();

                break;
            }
            default:
                FAIL() << "Got into handler for unknown command " << commandToString(cmd);
                break;
        }

        for (auto& a : assertions)
        {
            a.makeItSo(sut);
        }
        for (auto& e : existing)
        {
            e.makeItSo(nodeEditor);
        }
    }

    Command cmd{CMD_UNKNOWN};
    using ExistingItemArray = std::vector<ScriptExistingItem>;
    ExistingItemArray existing;

    using AssertionArray = std::vector<ScriptAssertion>;
    AssertionArray assertions;
    dagbase::ConfigurationElement* _scriptConfig{nullptr};

    static const char* commandToString(Command value)
    {
        switch (value)
        {
            ENUM_NAME(CMD_UNKNOWN)
            ENUM_NAME(CMD_CONFIGURE)
            ENUM_NAME(CMD_STEP_UNTIL_FINISHED)
            ENUM_NAME(CMD_RESET)
            ENUM_NAME(CMD_UPDATE)
        }

        return "<error>";
    }

    static Command parseCommand(const char* str)
    {
        TEST_ENUM(CMD_UNKNOWN, str)
        TEST_ENUM(CMD_CONFIGURE, str)
        TEST_ENUM(CMD_STEP_UNTIL_FINISHED, str)
        TEST_ENUM(CMD_RESET, str)
        TEST_ENUM(CMD_UPDATE, str)

        return CMD_UNKNOWN;
    }
};

class Script_testExecute : public ::testing::TestWithParam<std::tuple<const char*>>
{
public:
    void configure(dagbase::ConfigurationElement& config)
    {
        if (auto element = config.findElement("cmds"); element)
        {
            element->eachChild([this](dagbase::ConfigurationElement& child) {
                ScriptCommand item;
                item.configure(child);
                _cmds.emplace_back(item);
                return true;
            });
        }
    }

    void makeItSo(dagbase::Script& sut, dagbase::Node* self, dag::NodeEditorLive& nodeEditor)
    {
        for (auto cmd : _cmds)
        {
            cmd.makeItSo(sut, self, nodeEditor);
        }
    }
protected:
    using CommandArray = std::vector<ScriptCommand>;
    CommandArray _cmds;
};

TEST_P(Script_testExecute, testExpectedResult)
{
    auto scriptFilename = std::get<0>(GetParam());
    dagbase::Lua lua;
    lua.eval("require \"luadag\"");
    auto config = dagbase::ConfigurationElement::fromFile(lua, scriptFilename);
    ASSERT_NE(nullptr, config);
    ASSERT_TRUE(lua.ok());
    dag::NodeEditorLive nodeEditor;
    dagui::NodeEditorScriptProtocol proto(&nodeEditor);
    FakeScriptProtocol fakeProto(&proto);
    int numRequests{0};
    dagbase::ConfigurationElement::readConfig(*config, "numRequests", &numRequests);
    EXPECT_CALL(fakeProto, request(::testing::_, 1, ::testing::_, ::testing::_)).Times(numRequests);
    dagui::LuaScript sut(&lua, &fakeProto);
    configure(*config);

    auto service = new HelloService();
    nodeEditor.setService(service);
    auto status = nodeEditor.createNode("Boundary", "test1");
    ASSERT_EQ(dagbase::Status::STATUS_OK, status.status);
    auto self = nodeEditor.activeGraph()->node(std::get<dagbase::NodeID>(status.result.value()));
    self->setScript(&sut);
    ASSERT_NE(nullptr, self);
    makeItSo(sut, self, nodeEditor);
}

INSTANTIATE_TEST_SUITE_P(Script, Script_testExecute, ::testing::Values(
    std::make_tuple("data/tests/Script/Resume.lua"),
    std::make_tuple("data/tests/Script/ResumeError.lua"),
    std::make_tuple("data/tests/Script/NonExistentFunctionError.lua"),
    std::make_tuple("data/tests/Script/ResumeErrorThenNonExistentFunctionError.lua"),
    std::make_tuple("data/tests/Script/Update.lua")
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
    sut.pushObj(node, nodeType, 0);
    dagbase::Node* node2 = nullptr;
    sut.convertObj((void**)&node2, nodeType, 0);
    ASSERT_EQ(node, node2);
}

class Script_testConfigure : public ::testing::TestWithParam<std::tuple<const char*, bool>>
{

};

TEST_P(Script_testConfigure, testExpectedResult)
{
    dagbase::Lua lua;
    lua.eval("require \"luadag\"");
    auto configFilename = std::get<0>(GetParam());
    auto exists = std::get<1>(GetParam());
    auto config = dagbase::ConfigurationElement::fromFile(lua, configFilename);
    ASSERT_NE(nullptr, config);
    MockScriptProtocol proto;
    dagui::LuaScript sut(&lua, &proto);
    sut.configure(*config);
    ASSERT_EQ(exists, nullptr!=sut.thread());
}

INSTANTIATE_TEST_SUITE_P(LuaScript, Script_testConfigure, ::testing::Values(
    std::make_tuple("data/tests/Script/FromFunction.lua", true),
    std::make_tuple("data/tests/Script/FromBadFunction.lua", false),
    std::make_tuple("data/tests/Script/FromNamedFunction.lua", true)
    ));
